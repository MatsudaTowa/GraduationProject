//===========================================================================================================================================================
// 
// カード情報 [card_info.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "card_info.h"
#include "httplib.h"
#include <iostream>
#include <regex>
#include <cctype>
#include <string>
#include <vector>
#include <stdexcept>

using nlohmann::ordered_json;

// 小さなユーティリティ
static std::string ToLower(std::string s) {
    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

// base64 ノイズ除去
static std::string NormalizeBase64(const std::string& s) {
    std::string out; out.reserve(s.size());
    for (unsigned char c : s) if (!std::isspace(c)) out.push_back(c);
    return out;
}

// 簡易 base64 デコード（実運用では既存ライブラリへ差し替え推奨）
static std::vector<uint8_t> Base64Decode(const std::string& in) {
    static const std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int val = 0, valb = -8;
    std::vector<uint8_t> out;
    out.reserve((in.size() * 3) / 4);
    for (unsigned char c : in) {
        if (std::isspace(c)) continue;
        if (c == '=') break;
        size_t idx = b64.find(c);
        if (idx == std::string::npos) continue;
        val = (val << 6) + static_cast<int>(idx);
        valb += 6;
        if (valb >= 0) {
            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

// raw.githubusercontent URL 判定
static bool IsRawGithubUrl(const std::string& url) {
    return url.find("raw.githubusercontent.com/") != std::string::npos;
}

// blob URL 解析
static bool ParseBlobGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path) {
    static const std::regex re(R"(https?://github\.com/([^/]+)/([^/]+)/blob/([^/]+)/(.+))");
    std::smatch m;
    if (std::regex_match(url, m, re)) {
        owner = m[1]; repo = m[2]; branch = m[3]; path = m[4];
        return true;
    }
    return false;
}

// raw URL 解析: 
static bool ParseRawGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path) {
    static const std::regex re(R"(https?://raw\.githubusercontent\.com/([^/]+)/([^/]+)/([^/]+)/(.+))");
    std::smatch m;
    if (std::regex_match(url, m, re)) {
        owner = m[1]; repo = m[2]; branch = m[3]; path = m[4];
        return true;
    }
    return false;
}

// GitHub Contents API endpoint 作成
static std::string MakeContentsEndpoint(const std::string& owner, const std::string& repo, const std::string& path, const std::string& branch) {
    std::string ep = "https://api.github.com/repos/" + owner + "/" + repo + "/contents/" + path;
    if (!branch.empty()) ep += "?ref=" + branch;
    return ep;
}

static size_t CurlWriteCallBack(void* contents, size_t size, size_t nmemb, void* userp)
{
    size_t total = size * nmemb;
    std::string* s = static_cast<std::string*>(userp);
    s->append(static_cast<char*>(contents), total);
    return total;
}


/**
* @brief コンストラクタ
*/
My::CCardInfo::CCardInfo()
{

}

/** 
* @brief デストラクタ
*/
My::CCardInfo::~CCardInfo()
{
    m_Param = {};
}

/**
* @brief 初期化処理
* @return 成功したかどうか
*/
HRESULT My::CCardInfo::Init()
{
	return S_OK;
}

/**
* @brief JSONからカードデータの構築
* @param [in]JSONオブジェクト
*/
bool My::CCardInfo::LoadJson(const nlohmann::json& j)
{
    try
    {
        if (!j.is_object())
        {
            std::cerr << "オブジェクトが空です" << std::endl;
            return false;
        }

        auto parseOne = [&](const nlohmann::ordered_json& item, CardRecord& outRec)->bool
        {
            if (!item.is_object())
            {
                return false;
            }

            if (!item.contains("Pack ID") || !item.contains("Card ID") || !item.contains("Card Name"))
            {// タグが見つからなかった場合
                return false;
            }

            // BaseParam組み立て
            try
            {
                outRec.baseParam.nPackID = item["Pack ID"].get<int>();  // パック番号
                outRec.baseParam.nCardID = item["Card ID"].get<int>();  // カード番号
            }
            catch (const nlohmann::json::exception& e) {
                // JSON ライブラリ由来のエラー（キー存在はしていたが型が違う等）
                std::cerr << "JSON parse error for Pack ID/Card ID: " << e.what() << '\n';
                return false;
            }
            catch (const std::exception& e) {
                // その他の標準例外
                std::cerr << "unexpected error reading Pack ID/Card ID: " << e.what() << '\n';
                return false;
            }

            outRec.baseParam.Name = item.value("Card Name", item.value("name", std::string{}));                             // カード名
            outRec.baseParam.Ruby = item.value("Card Name Ruby", item.value("ruby", std::string{}));                        // カード名の読み方
            outRec.baseParam.nCost = item.value("cost", 0);                                                                 // コスト
            outRec.baseParam.ImagePath = item.value("image", item.value("imagepath", std::string{}));                       // イラスト画像
            outRec.baseParam.Maintype = static_cast<CardType>(item.value("type", static_cast<int>(CardType::NONE)));        // カードの種類
            outRec.baseParam.Raritytype = static_cast<RARITY>(item.value("rarity", static_cast<int>(RARITY::NONE_RARITY))); // レアリティ
            outRec.type = outRec.baseParam.Maintype;

            // 個別パラメーター設定
            switch (outRec.type)
            {
            case ATTACK:    // 攻撃タイプのとき
                try
                {
                    outRec.attackParam.BaseParam = outRec.baseParam;
                    outRec.attackParam.nDamage = item.value("power", item.value("nDamage", 0));
                    outRec.attackParam.Attacktype = static_cast<AttackType>(item["attacktype"].get<int>());

                }
                catch (const nlohmann::json::exception& e)
                {
                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
                    return false;
                }
                break;
            case DEFENSE:   // 守備タイプののとき
                try
                {
                    outRec.defenseParam.BaseParam = outRec.baseParam;
                    outRec.defenseParam.nGuard = item.value("guard", item.value("nGuard", 0));
                    outRec.defenseParam.nCounter = item.value("counter", item.value("nCounter", 0));
                    outRec.defenseParam.Defensetype = static_cast<DefenseType>(item["defensetype"].get<int>());
                }
                catch (const nlohmann::json::exception& e)
                {
                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
                    return false;
                }
                break;
            case ASSIST:
                try
                {
                    outRec.assistParam.BaseParam = outRec.baseParam;
                    outRec.assistParam.bIsOneTime = item.value("isOneTime", false);                         // 効果が単発かどうか

                    if (!outRec.assistParam.bIsOneTime)
                    {// 単発じゃないときに時間の設定
                        outRec.assistParam.nTime = item.value("time", 0);
                    }

                    outRec.assistParam.AssistType = static_cast<AssistType>(item["assisttype"].get<int>()); // アシストの種類
                    switch (outRec.assistParam.AssistType)
                    {// アシストの種類
                    case OBSTRUCT:  // 妨害
                        break;
                    case BUFF:      // バフ
                        outRec.assistParam.Bufftype = static_cast<BuffType>(item["bufftype"].get<int>());   // バフの種類
                        if (outRec.assistParam.Bufftype == HEAL)
                        {// 回復の場合
                            outRec.assistParam.Healtype = static_cast<HealType>(item["healtype"].get<int>());   // 回復対象
                            outRec.assistParam.nHeal = item.value("heal", 0);
                        }
                        break;
                    case DEBUFF:    // デバフ
                        break;
                    }
                }
                catch (const nlohmann::json::exception& e)
                {
                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
                    return false;
                }
                break;

            default:
                break;
            }

            return true;
        };

        CardRecord newRec{};
        if (!parseOne(j, newRec))
        {
             std::cerr << "パースに失敗" << std::endl;
             return false;
        }

        m_Record = std::move(newRec);
        m_Param = m_Record.baseParam;

        // 型を合わせるために変換
        CCard_Client::Param clientparam{};
        clientparam.nPackID = m_Record.baseParam.nPackID;                                                       // パック番号
        clientparam.nCardID = m_Record.baseParam.nCardID;                                                       // カード番号
        clientparam.Name = m_Record.baseParam.Name;                                                             // カード名
        clientparam.Ruby = m_Record.baseParam.Ruby;                                                             // カード名の読み方
        clientparam.nCost = m_Record.baseParam.nCost;                                                           // コスト
        clientparam.Maintype = static_cast<CCard_Client::CardType>(m_Record.baseParam.Maintype);                // カードのタイプ
        clientparam.Raritytype = static_cast<CCard_Client::RARITY>(m_Record.baseParam.Raritytype);              // カードレアリティ
        clientparam.ImagePath = m_Record.baseParam.ImagePath;                                                   // カードイラスト画像

        switch (m_Record.baseParam.Maintype)
        {
        case ATTACK:    // 攻撃タイプ
            clientparam.Cardparam.attackParam.nDamage = m_Record.attackParam.nDamage;         // 攻撃力
            clientparam.Cardparam.attackParam.Attacktype = static_cast<CCard_Client::AttackType>(m_Record.attackParam.Attacktype); // 攻撃対象
            break;
        case DEFENSE:   // 守備タイプ
            clientparam.Cardparam.defenseParam.nGuard = m_Record.defenseParam.nGuard;   // ガード値
            clientparam.Cardparam.defenseParam.Defensetype = static_cast<CCard_Client::DefenseType>(m_Record.defenseParam.Defensetype); // カウンターの有無
            if (clientparam.Cardparam.defenseParam.Defensetype == COUNTER)
            {// カウンターするカードのみカウンター値を設定
                clientparam.Cardparam.defenseParam.nCounter = m_Record.defenseParam.nCounter;
            }

            break;
        case ASSIST:    // アシストタイプ
            clientparam.Cardparam.assistParam.bIsOneTime = m_Record.assistParam.bIsOneTime; // 効果が単発かどうか
            if (!clientparam.Cardparam.assistParam.bIsOneTime)
            {// カードの効果が単発じゃないとき
                clientparam.Cardparam.assistParam.nTime = clientparam.Cardparam.assistParam.nTime;  // 効果時間
            }
            clientparam.Cardparam.assistParam.Assisttype = static_cast<CCard_Client::AssistType>(m_Record.assistParam.AssistType);  // アシストの種類

            switch (clientparam.Cardparam.assistParam.Assisttype)
            {
            case OBSTRUCT:  // 妨害カードのとき
                break;
            case BUFF:      // バフカードのとき
                clientparam.Cardparam.assistParam.Bufftype = static_cast<CCard_Client::BuffType>(m_Record.assistParam.Bufftype);    // バフの種類

                switch (clientparam.Cardparam.assistParam.Bufftype)
                {
                case HEAL:  // 回復カードのとき
                    clientparam.Cardparam.assistParam.nHeal = clientparam.Cardparam.assistParam.nHeal;  // 回復量
                    clientparam.Cardparam.assistParam.Healtype = static_cast<CCard_Client::HealType>(m_Record.assistParam.Healtype);    // 回復対象
                    break;
                }
                break;
            case DEBUFF:    // デバフカードのとき
                break;
            }
            break;
        }

        // 読み込んだカード情報を登録
        My::CCardManager::GetInstance()->RegistCardList(clientparam);

        return true;
    }
    catch (const nlohmann::json::exception& je)
    {
        std::cerr << "LoadJson json exception: " << je.what() << '\n';
        return false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "LoadJson exception: " << e.what() << '\n';
        return false;
    }

}

/**
* @brief バイト列からカードデータの構築
* @param [in]バイト列
* @return 成功したかどうか
*/
bool My::CCardInfo::LoadBytes(const std::vector<uint8_t>& bytes)
{
    if (bytes.empty())
    {// 情報がない場合
        std::cerr << "LoadBytes:input bytes empty" << std::endl;

        return false;
    }

    try
    {
        nlohmann::ordered_json root = nlohmann::ordered_json::from_cbor(bytes);

        if (root.is_array())
        {
            bool any = false;
            for (const auto& elem : root)
            {
                if (!elem.is_object())
                {
                    continue;
                }

                My::CCardInfo tmp;
                if (!tmp.LoadJson(elem))
                {
                    std::cerr << "LoadBytes: LoadJson failed for an array element; skipping" << std::endl;
                    continue;
                }
                any = true;
            }
            return any;
        }
        else if (root.is_object())
        {
            if (!this->LoadJson(root)) 
            {
                std::cerr << "LoadBytes: LoadJson failed for object\n";
                return false;
            }
            return true;
        }
        else
        {
            std::cerr << "LoadBytes: unexpected CBOR root type\n";
            return false;
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "LoadBytes exception: " << e.what() << '\n';
        return false;
    }

    return true;
}

/**
* @brief URLからデータ取得
* @param [in]githubのURL
* @param [in]認証トークン
* @return 成功したかどうか
*/
bool My::CCardInfo::LoadUrl(const std::string& url, const std::string& token, const std::shared_ptr<IHttpClient>& client)
{
    //std::cerr << "DEBUG LoadUrl start: url=" << url << " token=" << (token.empty() ? "<empty>" : "<set>") << '\n';

    //try
    //{
    //    std::vector<uint8_t> bytes;

    //    // Helper: normalize a github.com-style url into owner/repo/rest (handles "https://" etc.)
    //    auto ExtractOwnerRepoRest = [](const std::string& u) -> std::tuple<std::string, std::string, std::string> {
    //        std::string s = u;
    //        const std::string https_prefix = "https://";
    //        const std::string http_prefix = "http://";
    //        if (s.rfind(https_prefix, 0) == 0) s = s.substr(https_prefix.size());
    //        else if (s.rfind(http_prefix, 0) == 0) s = s.substr(http_prefix.size());
    //        // remove leading github.com/ if present
    //        const std::string host_prefix = "github.com/";
    //        if (s.rfind(host_prefix, 0) == 0) s = s.substr(host_prefix.size());
    //        size_t p1 = s.find('/');
    //        if (p1 == std::string::npos) return { "", "", "" };
    //        size_t p2 = s.find('/', p1 + 1);
    //        if (p2 == std::string::npos) return { "", "", "" };
    //        std::string owner = s.substr(0, p1);
    //        std::string repo = s.substr(p1 + 1, p2 - p1 - 1);
    //        std::string rest = s.substr(p2 + 1);
    //        return { owner, repo, rest };
    //    };

    //    // --- client 経路（既存 IHttpClient を使う） ---
    //    if (client)
    //    {
    //        if (IsRawGithubUrl(url))
    //        {
    //            std::string body;
    //            std::vector<std::pair<std::string, std::string>> headers;
    //            headers.emplace_back("Accept", "application/octet-stream");
    //            if (!token.empty())
    //            {
    //                headers.emplace_back("Authorization", std::string("token ") + token);
    //            }

    //            int status = client->Get(url, body, headers);
    //            std::cerr << "DEBUG client raw GET status=" << status << " url=" << url << '\n';
    //            if (status != 200) 
    //            {
    //                std::cerr << "LoadUrl: raw GET failed status=" << status << " url=" << url << '\n';
    //                if (!body.empty()) 
    //                {
    //                    std::ofstream ofs("debug_raw_response_client.html");
    //                    if (ofs) ofs << body;
    //                }
    //                return false;
    //            }
    //            bytes.assign(body.begin(), body.end());
    //        }
    //        else
    //        {
    //            std::string owner, repo, branch, path, endpoint;
    //            if (ParseBlobGithubUrl(url, owner, repo, branch, path)) 
    //            {
    //                endpoint = MakeContentsEndpoint(owner, repo, path, branch);
    //            }
    //            else {
    //                // try normalized extraction
    //                auto t = ExtractOwnerRepoRest(url);
    //                owner = std::get<0>(t);
    //                repo = std::get<1>(t);
    //                path = std::get<2>(t);
    //                if (owner.empty() || repo.empty() || path.empty()) {
    //                    std::cerr << "LoadUrl: unsupported URL format (client): " << url << '\n';
    //                    return false;
    //                }
    //                endpoint = MakeContentsEndpoint(owner, repo, path, "");
    //            }

    //            std::string body;
    //            std::vector<std::pair<std::string, std::string>> headers;
    //            headers.emplace_back("Accept", "application/vnd.github.v3+json");
    //            if (!token.empty()) headers.emplace_back("Authorization", std::string("token ") + token);

    //            int status = client->Get(endpoint, body, headers);
    //            std::cerr << "DEBUG client Contents API GET status=" << status << " endpoint=" << endpoint << '\n';
    //            if (status != 200) {
    //                std::cerr << "LoadUrl: Contents API GET failed status=" << status << " endpoint=" << endpoint << '\n';
    //                std::cerr << "LoadUrl: Contents API response head: " << (body.size() ? body.substr(0, std::min<size_t>(body.size(), 256)) : "<empty>") << '\n';
    //                std::ofstream ofs("debug_contents_response_client.txt");
    //                if (ofs) ofs << body;
    //                return false;
    //            }

    //            // quick check: if body starts with '<', it's likely an HTML error page
    //            if (!body.empty() && body.front() == '<') {
    //                std::cerr << "LoadUrl: Contents API returned HTML (likely error) endpoint=" << endpoint << '\n';
    //                std::ofstream ofs("debug_contents_response_client.html");
    //                if (ofs) ofs << body;
    //                return false;
    //            }

    //            // parse JSON safely
    //            nlohmann::ordered_json cj;
    //            try {
    //                cj = nlohmann::ordered_json::parse(body);
    //            }
    //            catch (const std::exception& ex) {
    //                std::cerr << "LoadUrl: JSON parse failed for Contents API response (client): " << ex.what() << '\n';
    //                std::ofstream ofs("debug_contents_response_client.json");
    //                if (ofs) ofs << body;
    //                return false;
    //            }

    //            if (!cj.contains("content") || cj["content"].is_null()) {
    //                std::cerr << "LoadUrl: Contents API returned no content for " << endpoint << '\n';
    //                return false;
    //            }
    //            std::string b64 = NormalizeBase64(cj["content"].get<std::string>());
    //            bytes = Base64Decode(b64);
    //        }

    //        // size checks and pass to LoadBytes
    //        constexpr size_t MAX_BYTES = 32 * 1024 * 1024;
    //        if (bytes.empty()) {
    //            std::cerr << "LoadUrl: fetched data is empty (client)\n";
    //            return false;
    //        }
    //        if (bytes.size() > MAX_BYTES) {
    //            std::cerr << "LoadUrl: fetched data too large: " << bytes.size() << " (client)\n";
    //            return false;
    //        }
    //        return LoadBytes(bytes);
    //    }

    //    // --- client が nullptr の場合は cpp-httplib を使って取得する（同期） ---
    //    if (IsRawGithubUrl(url))
    //    {
    //        std::string owner, repo, branch, inner;
    //        if (!ParseRawGithubUrl(url, owner, repo, branch, inner)) {
    //            std::cerr << "LoadUrl: ParseRawGithubUrl failed for " << url << '\n';
    //            return false;
    //        }

    //        std::string host = "raw.githubusercontent.com";
    //        std::string path = "/" + owner + "/" + repo + "/" + branch + "/" + inner;

    //        httplib::Client cli(("https://" + host).c_str());
    //        cli.set_follow_location(true);
    //        cli.set_connection_timeout(5);
    //        cli.set_read_timeout(10);
    //        httplib::Headers h;
    //        h.insert({ "Accept", "application/octet-stream" });
    //        if (!token.empty()) h.insert({ "Authorization", std::string("token ") + token });

    //        auto res = cli.Get(path.c_str(), h);
    //        if (!res) 
    //        {
    //            std::cerr << "LoadUrl: httplib request failed for " << url << '\n';
    //            return false;
    //        }

    //        std::cerr << "DEBUG httplib raw GET status=" << res->status << " url=" << url << '\n';
    //        if (res->status != 200) 
    //        {
    //            std::cerr << "LoadUrl: httplib status=" << res->status << " url=" << url << '\n';
    //            std::cerr << "LoadUrl: httplib response head: " << (res->body.size() ? res->body.substr(0, std::min<size_t>(res->body.size(), 256)) : "<empty>") << '\n';
    //            std::ofstream ofs("debug_raw_response_httplib.html");
    //            if (ofs) ofs << res->body;
    //            return false;
    //        }
    //        const std::string& body = res->body;
    //        bytes.assign(body.begin(), body.end());
    //    }
    //    else
    //    {
    //        // blob or owner/repo/path -> use Contents API via api.github.com
    //        std::string owner, repo, branch, path, endpoint;
    //        if (ParseBlobGithubUrl(url, owner, repo, branch, path)) 
    //        {
    //            endpoint = MakeContentsEndpoint(owner, repo, path, branch);
    //        }
    //        else 
    //        {
    //            auto t = ExtractOwnerRepoRest(url);
    //            owner = std::get<0>(t);
    //            repo = std::get<1>(t);
    //            path = std::get<2>(t);
    //            if (owner.empty() || repo.empty() || path.empty()) 
    //            {
    //                std::cerr << "LoadUrl: unsupported URL format (httplib): " << url << '\n';
    //                return false;
    //            }
    //            endpoint = MakeContentsEndpoint(owner, repo, path, "");
    //        }

    //        // --- SAFETY: ensure endpoint is full "https://api.github.com/..." ---
    //        const std::string apiHost = "api.github.com";
    //        const std::string apiBase = "https://api.github.com";

    //        if (endpoint.rfind(apiBase, 0) != 0) 
    //        {
    //            std::string apiPath = endpoint;
    //            if (apiPath.empty() || apiPath[0] != '/') apiPath = "/" + apiPath;
    //            endpoint = apiBase + apiPath;
    //        }

    //        // debug before substr
    //        std::cerr << "DEBUG: endpoint=\"" << endpoint << "\" (len=" << endpoint.size() << ") apiBase=\"" << apiBase << "\" (len=" << apiBase.size() << ")\n";

    //        // SAFETY: compute apiPath without throwing
    //        std::string apiPath;
    //        if (endpoint.size() >= apiBase.size()) 
    //        {
    //            apiPath = endpoint.substr(apiBase.size());
    //        }
    //        else 
    //        {
    //            std::cerr << "LoadUrl warning: endpoint shorter than apiBase; rebuilding endpoint\n";
    //            apiPath = endpoint;
    //            if (apiPath.empty() || apiPath[0] != '/') apiPath = "/" + apiPath;
    //            endpoint = apiBase + apiPath;
    //            std::cerr << "LoadUrl info: rebuilt endpoint=\"" << endpoint << "\" (len=" << endpoint.size() << ")\n";
    //            apiPath = endpoint.substr(apiBase.size());
    //        }

    //        std::cerr << "DEBUG: apiPath to request=\"" << apiPath << "\" (len=" << apiPath.size() << ")\n";

    //        httplib::Client cli("api.github.com");
    //        //httplib::Client cli(("https://" + apiHost).c_str());
    //        cli.set_follow_location(true);
    //        cli.set_connection_timeout(5);
    //        cli.set_read_timeout(10);
    //        httplib::Headers h;
    //        h.insert({ "Accept", "application/vnd.github.v3+json" });
    //        if (!token.empty())
    //        {
    //            h.insert({ "Authorization", std::string("token ") + token });
    //        }

    //        auto res = cli.Get(apiPath.c_str(), h);
    //        if (!res) 
    //        {
    //            std::cerr << "LoadUrl: httplib request failed for " << endpoint << '\n';
    //            return false;
    //        }
    //        std::cerr << "DEBUG httplib API GET status=" << res->status << " endpoint=" << endpoint << '\n';
    //        if (res->status != 200) 
    //        {
    //            std::cerr << "LoadUrl: httplib status=" << res->status << " endpoint=" << endpoint << '\n';
    //            std::cerr << "LoadUrl: httplib response head: " << (res->body.size() ? res->body.substr(0, std::min<size_t>(res->body.size(), 256)) : "<empty>") << '\n';
    //            std::ofstream ofs("debug_api_response_httplib.txt");
    //            if (ofs) ofs << res->body;
    //            return false;
    //        }

    //        // quick HTML check
    //        if (!res->body.empty() && res->body.front() == '<')
    //        {
    //            std::cerr << "LoadUrl: API returned HTML (likely error). endpoint=" << endpoint << '\n';
    //            std::ofstream ofs("debug_api_response_httplib.html");
    //            if (ofs) ofs << res->body;
    //            return false;
    //        }

    //        // parse Contents API response safely
    //        nlohmann::ordered_json cj;
    //        try {
    //            cj = nlohmann::ordered_json::parse(res->body);
    //        }
    //        catch (const std::exception& ex) {
    //            std::cerr << "LoadUrl: JSON parse failed for API response (httplib): " << ex.what() << '\n';
    //            std::ofstream ofs("debug_api_response_httplib.json");
    //            if (ofs) ofs << res->body;
    //            return false;
    //        }

    //        if (!cj.contains("content") || cj["content"].is_null()) {
    //            std::cerr << "LoadUrl: Contents API returned no content for " << endpoint << '\n';
    //            return false;
    //        }
    //        std::string b64 = NormalizeBase64(cj["content"].get<std::string>());
    //        bytes = Base64Decode(b64);
    //    }

    //    // 最終チェックと LoadBytes 呼び出し
    //    constexpr size_t MAX_BYTES = 32 * 1024 * 1024;
    //    if (bytes.empty()) {
    //        std::cerr << "LoadUrl: fetched data is empty\n";
    //        return false;
    //    }
    //    if (bytes.size() > MAX_BYTES) 
    //    {
    //        std::cerr << "LoadUrl: fetched data too large: " << bytes.size() << '\n';
    //        return false;
    //    }

    //    LoadBytes(bytes);
    //}
    //catch (const nlohmann::json::exception& je)
    //{
    //    std::cerr << "LoadUrl json exception: " << je.what() << '\n';
    //    return false;
    //}
    //catch (const std::exception& e)
    //{
    //    std::cerr << "LoadUrl exception: " << e.what() << '\n';
    //    return false;
    //}

    //return true;
    
    std::cerr << "DEBUG LoadUrl start: url=" << url << " token=" << (token.empty() ? "<empty>" : "<set>") << '\n';
    
    try
    {
        std::vector<uint8_t> bytes;
    
        // Helper: normalize github-style url into owner/repo/rest
        auto ExtractOwnerRepoRest = [](const std::string& u) -> std::tuple<std::string, std::string, std::string> {
            std::string s = u;
            const std::string https_prefix = "https://";
            const std::string http_prefix = "http://";
            if (s.rfind(https_prefix, 0) == 0) s = s.substr(https_prefix.size());
            else if (s.rfind(http_prefix, 0) == 0) s = s.substr(http_prefix.size());
            const std::string host_prefix = "github.com/";
            if (s.rfind(host_prefix, 0) == 0) s = s.substr(host_prefix.size());
            size_t p1 = s.find('/');
            if (p1 == std::string::npos) return { "", "", "" };
            size_t p2 = s.find('/', p1 + 1);
            if (p2 == std::string::npos) return { "", "", "" };
            return { s.substr(0, p1), s.substr(p1 + 1, p2 - p1 - 1), s.substr(p2 + 1) };
        };
    
        // ------------------------------
        // Path A: caller-supplied IHttpClient
        // ------------------------------
        if (client)
        {
            if (IsRawGithubUrl(url))
            {
                std::string body;
                std::vector<std::pair<std::string, std::string>> headers;
                headers.emplace_back("Accept", "application/octet-stream");
                headers.emplace_back("User-Agent", "MyApp/1.0");
                if (!token.empty()) headers.emplace_back("Authorization", std::string("token ") + token);
    
                int status = client->Get(url, body, headers);
                std::cerr << "DEBUG client raw GET status=" << status << " url=" << url << '\n';
                if (status != 200) {
                    std::cerr << "LoadUrl: raw GET failed status=" << status << " url=" << url << '\n';
                    if (!body.empty()) { std::ofstream ofs("debug_raw_response_client.html"); if (ofs) ofs << body; }
                    return false;
                }
                bytes.assign(body.begin(), body.end());
            }
            else
            {
                std::string owner, repo, branch, path, endpoint;
                if (ParseBlobGithubUrl(url, owner, repo, branch, path)) 
                {
                    endpoint = MakeContentsEndpoint(owner, repo, path, branch);
                }
                else
                {
                    auto t = ExtractOwnerRepoRest(url);
                    owner = std::get<0>(t); repo = std::get<1>(t); path = std::get<2>(t);
                    if (owner.empty() || repo.empty() || path.empty()) {
                        std::cerr << "LoadUrl: unsupported URL format (client): " << url << '\n';
                        return false;
                    }
                    endpoint = MakeContentsEndpoint(owner, repo, path, "");
                }
    
                std::string body;
                std::vector<std::pair<std::string, std::string>> headers;
                headers.emplace_back("Accept", "application/vnd.github.v3+json");
                headers.emplace_back("User-Agent", "MyApp/1.0");
                if (!token.empty()) headers.emplace_back("Authorization", std::string("token ") + token);
    
                int status = client->Get(endpoint, body, headers);
                std::cerr << "DEBUG client Contents API GET status=" << status << " endpoint=" << endpoint << '\n';
                if (status != 200) 
                {
                    std::cerr << "LoadUrl: Contents API GET failed status=" << status << " endpoint=" << endpoint << '\n';
                    std::cerr << "LoadUrl: Contents API response head: " << (body.size() ? body.substr(0, std::min<size_t>(body.size(), 256)) : "<empty>") << '\n';
                    std::ofstream ofs("debug_contents_response_client.txt"); if (ofs) ofs << body;
                    return false;
                }
    
                if (!body.empty() && body.front() == '<') 
                {
                    std::cerr << "LoadUrl: Contents API returned HTML (likely error) endpoint=" << endpoint << '\n';
                    std::ofstream ofs("debug_contents_response_client.html"); if (ofs) ofs << body;
                    return false;
                }
    
                nlohmann::ordered_json cj;
                try 
                { 
                    cj = nlohmann::ordered_json::parse(body);
                }
                catch (const std::exception& ex)
                {
                    std::cerr << "LoadUrl: JSON parse failed for Contents API response (client): " << ex.what() << '\n';
                    std::ofstream ofs("debug_contents_response_client.json"); if (ofs) ofs << body;
                    return false;
                }
    
                if (!cj.contains("content") || cj["content"].is_null()) 
                {
                    std::cerr << "LoadUrl: Contents API returned no content for " << endpoint << '\n';
                    return false;
                }
                bytes = Base64Decode(NormalizeBase64(cj["content"].get<std::string>()));
            }
    
            constexpr size_t MAX_BYTES = 32 * 1024 * 1024;
            if (bytes.empty()) { std::cerr << "LoadUrl: fetched data is empty (client)\n"; return false; }
            if (bytes.size() > MAX_BYTES) { std::cerr << "LoadUrl: fetched data too large: " << bytes.size() << " (client)\n"; return false; }
            return LoadBytes(bytes);
        }
    
        // ------------------------------
        // Path B: cpp-httplib (synchronous)
        // ------------------------------
        if (IsRawGithubUrl(url))
        {
            std::string owner, repo, branch, inner;
            if (!ParseRawGithubUrl(url, owner, repo, branch, inner)) {
                std::cerr << "LoadUrl: ParseRawGithubUrl failed for " << url << '\n';
                return false;
            }
    
            const std::string host = "raw.githubusercontent.com";
            const std::string path = "/" + owner + "/" + repo + "/" + branch + "/" + inner;
    
    #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
            try {
                httplib::SSLClient cli(host.c_str());
                cli.set_follow_location(true);
                cli.set_connection_timeout(5);
                cli.set_read_timeout(10);
                httplib::Headers h; h.insert({ "Accept", "application/octet-stream" });
                h.insert({ "User-Agent", "MyApp/1.0" });
                if (!token.empty()) h.insert({ "Authorization", std::string("token ") + token });
    
                auto res = cli.Get(path.c_str(), h);
                if (!res) { std::cerr << "LoadUrl: httplib request failed for " << url << '\n'; return false; }
                if (res->status != 200) {
                    std::cerr << "LoadUrl: httplib status=" << res->status << " url=" << url << '\n';
                    std::ofstream ofs("debug_raw_response_httplib.html"); if (ofs) ofs << res->body;
                    return false;
                }
                bytes.assign(res->body.begin(), res->body.end());
            }
            catch (const std::exception& ex) {
                std::cerr << "LoadUrl: SSLClient exception for raw.githubusercontent: " << ex.what() << '\n';
                return false;
        }
    #else
            std::cerr << "LoadUrl: HTTPS request to raw.githubusercontent requires cpp-httplib built with OpenSSL support\n";
            return false;
    #endif
    }
        else
        {
            std::string owner, repo, branch, path, endpoint;
            if (ParseBlobGithubUrl(url, owner, repo, branch, path)) {
                endpoint = MakeContentsEndpoint(owner, repo, path, branch);
            }
            else {
                auto t = ExtractOwnerRepoRest(url);
                owner = std::get<0>(t); repo = std::get<1>(t); path = std::get<2>(t);
                if (owner.empty() || repo.empty() || path.empty()) {
                    std::cerr << "LoadUrl: unsupported URL format (httplib): " << url << '\n';
                    return false;
                }
                endpoint = MakeContentsEndpoint(owner, repo, path, "");
            }
    
            const std::string apiHost = "api.github.com";
            const std::string apiBase = "https://api.github.com";
    
            // Normalize endpoint to full https://api.github.com/...
            if (endpoint.rfind(apiBase, 0) != 0) {
                std::string apiP = endpoint;
                if (apiP.empty() || apiP[0] != '/') apiP = "/" + apiP;
                endpoint = apiBase + apiP;
            }
    
            std::cerr << "DEBUG: endpoint=\"" << endpoint << "\" (len=" << endpoint.size() << ") apiBase=\"" << apiBase << "\" (len=" << apiBase.size() << ")\n";
    
            // safe substr: ensure endpoint is long enough
            if (endpoint.size() < apiBase.size()) {
                std::cerr << "LoadUrl: endpoint unexpectedly shorter than apiBase: endpoint=\"" << endpoint << "\"\n";
                return false;
            }
            std::string apiPath = endpoint.substr(apiBase.size()); // leading '/repos/...'
    
            std::cerr << "DEBUG: apiPath to request=\"" << apiPath << "\" (len=" << apiPath.size() << ")\n";
    
            if (apiPath.empty() || apiPath.front() != '/') {
                std::cerr << "LoadUrl: invalid apiPath: \"" << apiPath << "\"\n";
                return false;
            }
    
    #ifdef CPPHTTPLIB_OPENSSL_SUPPORT
            using MyClient = httplib::SSLClient;
    #else
            using MyClient = httplib::Client;
    #endif
    
            const int maxAttempts = 3;
            std::string lastErr;
            bool success = false;
    
            for (int attempt = 0; attempt < maxAttempts; ++attempt)
            {
                try 
                {
                    MyClient cli(apiHost.c_str());
                    cli.set_follow_location(true);
                    cli.set_connection_timeout(30);
                    cli.set_read_timeout(60);
    
                    httplib::Headers h;
                    h.insert({ "Accept", "application/vnd.github.v3+json" });
                    h.insert({ "User-Agent", "MyApp/1.0" });
                    if (!token.empty()) h.insert({ "Authorization", std::string("token ") + token });
    
                    std::cerr << "DEBUG: attempt=" << (attempt + 1) << " Get host=" << apiHost << " apiPath=" << apiPath << '\n';

                    auto res = cli.Get(apiPath.c_str(), h);
    
                    if (!res) 
                    {
                        lastErr = "httplib returned null response (network/SSL/proxy)";
                        std::cerr << "DEBUG: attempt=" << (attempt + 1) << " " << lastErr << '\n';
                        std::this_thread::sleep_for(std::chrono::milliseconds(200 * (attempt + 1)));
                        continue;
                    }
    
                    // detailed response logging
                    std::cerr << "DEBUG: res->status=" << res->status << " body.size=" << res->body.size() << '\n';
                    for (const auto& kv : res->headers) std::cerr << "  resp header: " << kv.first << ": " << kv.second << '\n';
    
                    if (res->status != 200) {
                        std::ofstream ofs("debug_api_response_httplib.txt"); if (ofs) ofs << res->body;
                        std::cerr << "LoadUrl: httplib status=" << res->status << " endpoint=" << endpoint << '\n';
                        return false;
                    }
    
                    if (!res->body.empty() && res->body.front() == '<') {
                        std::ofstream ofs("debug_api_response_httplib.html"); if (ofs) ofs << res->body;
                        std::cerr << "LoadUrl: API returned HTML (likely error). endpoint=" << endpoint << '\n';
                        return false;
                    }
    
                    nlohmann::ordered_json cj;
                    try { cj = nlohmann::ordered_json::parse(res->body); }
                    catch (const std::exception& ex) {
                        std::cerr << "LoadUrl: JSON parse failed for API response (httplib): " << ex.what() << '\n';
                        std::ofstream ofs("debug_api_response_httplib.json"); if (ofs) ofs << res->body;
                        return false;
                    }
    
                    if (!cj.contains("content") || cj["content"].is_null()) {
                        std::cerr << "LoadUrl: Contents API returned no content for " << endpoint << '\n';
                        return false;
                    }
    
                    bytes = Base64Decode(NormalizeBase64(cj["content"].get<std::string>()));
                    success = true;
                    break;
                }
                catch (const std::exception& ex) {
                    lastErr = ex.what();
                    std::cerr << "DEBUG: httplib exception on attempt=" << (attempt + 1) << " : " << ex.what() << '\n';
                    std::this_thread::sleep_for(std::chrono::milliseconds(200 * (attempt + 1)));
                    continue;
                }
            }
    
            if (!success) {
                std::cerr << "LoadUrl: failed after retries. lastErr=" << lastErr << '\n';
                std::cerr << "LoadUrl: try curl -v -H \"Authorization: token <YOUR_TOKEN>\" -H \"Accept: application/vnd.github.v3+json\" -H \"User-Agent: MyApp/1.0\" \"https://api.github.com" << apiPath << "\"\n";
                return false;
            }
        }
    
        // final checks
        constexpr size_t MAX_BYTES = 32 * 1024 * 1024;
        if (bytes.empty()) { std::cerr << "LoadUrl: fetched data is empty\n"; return false; }
        if (bytes.size() > MAX_BYTES) { std::cerr << "LoadUrl: fetched data too large: " << bytes.size() << '\n'; return false; }
    
        return LoadBytes(bytes);

    }
    catch (const nlohmann::json::exception& je)
    {
        std::cerr << "LoadUrl json exception: " << je.what() << '\n';
        return false;
    }
    catch (const std::exception& e)
    {
        std::cerr << "LoadUrl exception: " << e.what() << '\n';
        return false;
    }
    
}

/**
* @brief パラメーター取得
* @return パラメーター
*/
My::CCardInfo::BaseParam My::CCardInfo::GetParam()
{
	return m_Param;
}

//bool My::CCardInfo::FetchContentsApiWithLibcurl(const std::string& apiPath, const std::string& token, std::vector<uint8_t>& out_bytes)
//{
//    const std::string url = std::string("http://api.github.com") + apiPath;
//
//    std::cerr << "DEBUG libcurl:URL = " << url << "token=" << (token.empty() ? "<empty>" : "<set>");
//
//    CURL* curl = curl_easy_init();
//
//    if (!curl)
//    {
//        std::cerr << "初期化に失敗" << std::endl;
//
//        return false;
//    }
//
//    std::string response;
//    struct curl_slist* headers = nullptr;
//    headers = curl_slist_append(headers, "Accept:application/vnd.github.v3+json");
//    headers = curl_slist_append(headers, "User-Agent:MyApp/1.0");
//
//    if (!token.empty())
//    {
//        std::string auth = std::string("Authorization:token ") + token;
//        headers = curl_slist_append(headers, auth.c_str());
//    }
//
//    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
//    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
//    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallBack);
//    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
//    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
//
//    CURLcode rc = curl_easy_perform(curl);
//    long http_code = 0;
//
//    if (rc == CURLE_OK)
//    {
//        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
//    }
//    else
//    {
//        std::cerr << "リクエストに失敗" << std::endl;
//    }
//
//    curl_slist_free_all(headers);
//    curl_easy_cleanup(curl);
//
//    std::cerr << "DEBUG libcurl: HTTP=" << http_code << " body.size=" << response.size() << std::endl;
//
//    if (rc != CURLE_OK)
//    {
//        return false;
//    }
//    
//    if (http_code != 200)
//    {
//        std::ofstream ofs("debug_libcurl_response.txt", std::ios::binary);
//
//        if (ofs)
//        {
//            ofs << response;
//        }
//        std::cerr << "libcurl: non-200 http" << http_code << std::endl;
//    }
//
//
//
//    return false;
//}
