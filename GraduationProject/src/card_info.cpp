//===========================================================================================================================================================
// 
// カード情報 [card_info.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "card_info.h"
#include <iostream>
#include <regex>
#include <cctype>
#include <string>
#include <vector>
#include <stdexcept>

extern bool IsRawGithubUrl(const std::string& url);
extern bool ParseBlobGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path);
extern bool ParseRawGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path);
extern std::string MakeContentsEndpoint(const std::string& owner, const std::string& repo, const std::string& path, const std::string& branch);
extern std::string NormalizeBase64(const std::string& s);
extern std::vector<uint8_t> Base64Decode(const std::string& in);

// Base64 デコードの簡易実装
static inline int b64_value(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

std::vector<uint8_t> Base64Decode(const std::string& in) {
    std::string s;
    s.reserve(in.size());
    for (char c : in) if (!std::isspace(static_cast<unsigned char>(c))) s.push_back(c);

    std::vector<uint8_t> out;
    out.reserve((s.size() * 3) / 4);
    int val = 0;
    int valb = -8;
    for (char c : s) {
        if (c == '=') break;
        int d = b64_value(c);
        if (d == -1) continue;
        val = (val << 6) + d;
        valb += 6;
        if (valb >= 0) {
            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

std::string NormalizeBase64(const std::string& s) {
    std::string t;
    t.reserve(s.size());
    for (char c : s) if (!std::isspace(static_cast<unsigned char>(c))) t.push_back(c);
    size_t mod = t.size() % 4;
    if (mod != 0) t.append(4 - mod, '=');
    return t;
}

bool ParseBlobGithubUrl(const std::string& url,
    std::string& owner,
    std::string& repo,
    std::string& branch,
    std::string& path)
{
    // https://github.com/<owner>/<repo>/blob/<branch>/path/to/file
    std::regex re(R"(https?://github\.com/([^/]+)/([^/]+)/blob/([^/]+)/(.*))", std::regex::icase);
    std::smatch m;
    if (std::regex_match(url, m, re) && m.size() == 5) {
        owner = m[1].str();
        repo = m[2].str();
        branch = m[3].str();
        path = m[4].str();
        return true;
    }
    return false;
}

bool ParseRawGithubUrl(const std::string& url,
    std::string& owner,
    std::string& repo,
    std::string& branch,
    std::string& path)
{
    // https://raw.githubusercontent.com/<owner>/<repo>/<branch>/path/to/file
    std::regex re(R"(https?://raw\.githubusercontent\.com/([^/]+)/([^/]+)/([^/]+)/(.*))", std::regex::icase);
    std::smatch m;
    if (std::regex_match(url, m, re) && m.size() == 5) {
        owner = m[1].str();
        repo = m[2].str();
        branch = m[3].str();
        path = m[4].str();
        return true;
    }
    return false;
}

bool IsRawGithubUrl(const std::string& url) {
    return url.find("://raw.githubusercontent.com/") != std::string::npos;
}

std::string MakeContentsEndpoint(const std::string& owner,
    const std::string& repo,
    const std::string& path,
    const std::string& branch)
{
    std::ostringstream oss;
    oss << "https://api.github.com/repos/" << owner << "/" << repo << "/contents/" << path;
    if (!branch.empty()) oss << "?ref=" << branch;
    return oss.str();
}

// ヘルパ: バイト列がテキスト JSON に見えるかの簡易判定
static bool LooksLikeTextJson(const std::vector<uint8_t>& bytes) {
    if (bytes.empty()) return false;
    uint8_t c = bytes.front();
    // '{' or '[' なら JSON テキストの可能性が高い
    if (c == '{' || c == '[') return true;
    // ASCII の可視文字列ならテキストの可能性が高い
    if (c >= 0x20 && c <= 0x7E) return true;
    return false;
}

using nlohmann::ordered_json;

/**
* @brief コンストラクタ
*/
My::CCardInfo::CCardInfo()
{
    m_Param = {};
    m_Record = {};
}

/** 
* @brief デストラクタ
*/
My::CCardInfo::~CCardInfo()
{
    m_Param = {};
    m_Record = {};
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
    {
        std::cerr << "情報の読み込みに失敗" << std::endl;
        return false;
    }

    try
    {
        auto j = nlohmann::ordered_json::from_cbor(bytes);

        if (j.is_object() || j.is_array())
        {
            if (j.is_array())
            {
                bool any = false;
                for (const auto& elem : j)
                {
                    if (!elem.is_object())
                    {
                        continue;
                    }

                    CCardInfo tmp;
                    if (!tmp.LoadJson(elem))
                    {
                        std::cerr << "情報の読み込みに失敗" << std::endl;
                        continue;
                    }
                    any = true;
                }
                return any;
            }
            else
            {
                return this->LoadJson(j);
            }
        }
    }
    catch (const std::exception&)
    {
        std::cerr << "パースに失敗" << std::endl;
    }

    try
    {
        std::string s(bytes.begin(), bytes.end());
        auto j = nlohmann::json::parse(s);

        if (j.is_object() || j.is_array())
        {
            if (j.is_array())
            {
                bool any = false;
                for (const auto& elem : j)
                {
                    if (!elem.is_object())
                    {
                        continue;
                    }

                    CCardInfo tmp;
                    if (!tmp.LoadJson(elem))
                    {
                        std::cerr << "情報の読み込み失敗" << std::endl;
                        continue;
                    }
                    any = true;
                }
                return any;
            }
            else
            {
                return this->LoadJson(j);
            }
        }
    }
    catch (const std::exception&)
    {
        std::cerr << "パースに失敗" << std::endl;
    }

    return false;
}

/**
* @brief URLからデータ取得
* @param [in]githubのURL
* @param [in]認証トークン
* @return 成功したかどうか
*/
bool My::CCardInfo::LoadUrl(const std::string& url, const std::string& token, const std::shared_ptr<IHttpClient>& client)
{    
    if (!client)
    {
        std::cerr << "URLが空です" << std::endl;
        return false;
    }

    // ヘッダ準備
    std::vector<std::pair<std::string, std::string>>headers;
    headers.emplace_back("Accept", "application/json");
    headers.emplace_back("User-Agent", "MyApp/1.0");
    if (!token.empty())
    {
        headers.emplace_back("Authorization", std::string("token ") + token);
    }

    HttpResponse res;
    try
    {
        auto fut = client->GetAsync(url, headers, 15000);
        res = fut.get();
    }
    catch (const std::exception& e)
    {
        std::cerr << "LoadUrl:GetAsync threw:" << e.what() << std::endl;
        return false;
    }

    if (!res.error.empty())
    {
        std::cerr << "ネットワークエラー" << res.error << std::endl;

        return false;
    }

    if (res.status == 304)
    {
        return true;
    }

    if (res.status < 200 || res.status >= 300)
    {
        std::string owner, repo, branch, path;
        if (ParseBlobGithubUrl(url, owner, repo, branch, path) || ParseRawGithubUrl(url, owner, repo, branch, path))
        {
            std::string api = MakeContentsEndpoint(owner, repo, path, branch);
            std::vector<std::pair<std::string, std::string>>apiHeaders;

            if (!token.empty())
            {
                apiHeaders.emplace_back("Authorization", std::string("token ") + token);
            }
            apiHeaders.emplace_back("Accept", "application/vnd.github.v3+json");
            apiHeaders.emplace_back("User-Agent", "MyApp/1.0");

            HttpResponse apiRes;

            try
            {
                auto fut2 = client->GetAsync(api, apiHeaders, 15000);
                apiRes = fut2.get();
            }
            catch (const std::exception& e)
            {
                std::cerr << "LoadUrl: Contents API GetAsync threw: " << e.what() << std::endl;
            }

            if (!apiRes.error.empty())
            {
                std::cerr << "ネットワークエラー" << apiRes.error << std::endl;
                return false;
            }

            if (apiRes.status == 304)
            {
                return true;
            }
            if (apiRes.status < 200 || apiRes.status >= 300)
            {
                std::cerr << "LoadUrl:Contents API network error:" << apiRes.error << std::endl;
                return false;
            }

            try
            {
                std::string apiText(apiRes.body.begin(), apiRes.body.end());
                auto j = ordered_json::parse(apiText);
                if (j.contains("content") && j["content"].is_string())
                {
                    std::string b64 = NormalizeBase64(j["content"].get<std::string>());
                    auto bytes = Base64Decode(b64);
                    if (bytes.empty())
                    {
                        std::cerr << "LoadUrl:base64 decode produced empty bytes" << std::endl;

                        return false;
                    }
                    return this->LoadBytes(bytes);
                }
                else if (j.contains("download_url") && j["download_url"].is_string())
                {
                    std::string dl = j["download_url"].get<std::string>();

                    HttpResponse dlRes;

                    try
                    {
                        auto fut3 = client->GetAsync(dlRes.body.empty() ? dl : dl, headers, 15000);
                        dlRes = fut3.get();
                    }
                    catch (const std::exception& e)
                    {
                        std::cerr << "LoadUrl:download_url GetAsync threw:" << e.what() << std::endl;

                        return false;
                    }
                    if (!dlRes.error.empty() || dlRes.status < 200 || dlRes.status >= 300)
                    {
                        std::cerr << "LoadUrl:download_url fetch failed";
                        return false;
                    }
                    return this->LoadBytes(dlRes.body);
                }
                else
                {
                    std::cerr << "LoadUrl:unexpected Contents API response" << std::endl;

                    return false;
                }
            }
            catch (const std::exception& e)
            {
                std::cerr << "LoadUrl:parse error from Contents API response:" << e.what() << std::endl;
           
                return false;
            }
        }

        std::cerr << "LoadUrl:HTTP status not OK:" << res.status << std::endl;
        return false;
    }

    bool looksText = LooksLikeTextJson(res.body);

    if (looksText)
    {
        try
        {
            std::string text(res.body.begin(), res.body.end());

            auto j = ordered_json::parse(text);

            if (j.is_object() || j.is_array())
            {
                return this->LoadJson(j);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "LoadUrl:Json parse failed,will try CBOR: " << e.what() << std::endl;
        }
    }

    try
    {
        auto j = ordered_json::from_cbor(res.body);
        if (j.is_object() || j.is_array())
        {
            return this->LoadJson(j);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "LoadUrl:CBOR parse failed: " << e.what() << std::endl;
        if (!looksText)
        {
            try
            {
                std::string text(res.body.begin(), res.body.end());
                auto jsontext = ordered_json::parse(text);
                if (jsontext.is_object() || jsontext.is_array())
                {
                    return this->LoadJson(jsontext);
                }
            }
            catch (const std::exception& error)
            {
                std::cerr << "LoadUrl:JSON parse also failed:" << error.what() << std::endl;
            }
        }
    }

    try
    {
        return this->LoadBytes(res.body);
    }
    catch (const std::exception& e)
    {
        std::cerr << "LoadUrl:LoadBytes exception" << e.what() << std::endl;
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

void My::CCardInfo::SetHttpClient(const std::shared_ptr<IHttpClient>& client)
{
    std::lock_guard<std::mutex> lk(m_ClientMutex);
    m_Client = client;
}
