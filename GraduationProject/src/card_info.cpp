////===========================================================================================================================================================
//// 
//// カード情報 [card_info.cpp]
//// Author : Kasai Keisuke
//// 
////===========================================================================================================================================================
//
//// include
//#include "card_info.h"
////#include "httplib.h"
//#include <iostream>
//#include <regex>
//#include <cctype>
//#include <string>
//#include <vector>
//#include <stdexcept>
//
//using nlohmann::ordered_json;
//
//// 小さなユーティリティ
//static std::string ToLower(std::string s) {
//    for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
//    return s;
//}
//
//// base64 ノイズ除去
//static std::string NormalizeBase64(const std::string& s) {
//    std::string out; out.reserve(s.size());
//    for (unsigned char c : s) if (!std::isspace(c)) out.push_back(c);
//    return out;
//}
//
//// 簡易 base64 デコード（実運用では既存ライブラリへ差し替え推奨）
//static std::vector<uint8_t> Base64Decode(const std::string& in) {
//    static const std::string b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//    int val = 0, valb = -8;
//    std::vector<uint8_t> out;
//    out.reserve((in.size() * 3) / 4);
//    for (unsigned char c : in) {
//        if (std::isspace(c)) continue;
//        if (c == '=') break;
//        size_t idx = b64.find(c);
//        if (idx == std::string::npos) continue;
//        val = (val << 6) + static_cast<int>(idx);
//        valb += 6;
//        if (valb >= 0) {
//            out.push_back(static_cast<uint8_t>((val >> valb) & 0xFF));
//            valb -= 8;
//        }
//    }
//    return out;
//}
//
//// raw.githubusercontent URL 判定
//static bool IsRawGithubUrl(const std::string& url) {
//    return url.find("raw.githubusercontent.com/") != std::string::npos;
//}
//
//// blob URL 解析
//static bool ParseBlobGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path) {
//    static const std::regex re(R"(https?://github\.com/([^/]+)/([^/]+)/blob/([^/]+)/(.+))");
//    std::smatch m;
//    if (std::regex_match(url, m, re)) {
//        owner = m[1]; repo = m[2]; branch = m[3]; path = m[4];
//        return true;
//    }
//    return false;
//}
//
//// raw URL 解析: 
//static bool ParseRawGithubUrl(const std::string& url, std::string& owner, std::string& repo, std::string& branch, std::string& path) {
//    static const std::regex re(R"(https?://raw\.githubusercontent\.com/([^/]+)/([^/]+)/([^/]+)/(.+))");
//    std::smatch m;
//    if (std::regex_match(url, m, re)) {
//        owner = m[1]; repo = m[2]; branch = m[3]; path = m[4];
//        return true;
//    }
//    return false;
//}
//
//// GitHub Contents API endpoint 作成
//static std::string MakeContentsEndpoint(const std::string& owner, const std::string& repo, const std::string& path, const std::string& branch) {
//    std::string ep = "https://api.github.com/repos/" + owner + "/" + repo + "/contents/" + path;
//    if (!branch.empty()) ep += "?ref=" + branch;
//    return ep;
//}
//
//static size_t CurlWriteCallBack(void* contents, size_t size, size_t nmemb, void* userp)
//{
//    size_t total = size * nmemb;
//    std::string* s = static_cast<std::string*>(userp);
//    s->append(static_cast<char*>(contents), total);
//    return total;
//}
//
//
///**
//* @brief コンストラクタ
//*/
//My::CCardInfo::CCardInfo()
//{
//
//}
//
///** 
//* @brief デストラクタ
//*/
//My::CCardInfo::~CCardInfo()
//{
//    m_Param = {};
//}
//
///**
//* @brief 初期化処理
//* @return 成功したかどうか
//*/
//HRESULT My::CCardInfo::Init()
//{
//	return S_OK;
//}
//
///**
//* @brief JSONからカードデータの構築
//* @param [in]JSONオブジェクト
//*/
//bool My::CCardInfo::LoadJson(const nlohmann::json& j)
//{
//    try
//    {
//        if (!j.is_object())
//        {
//            std::cerr << "オブジェクトが空です" << std::endl;
//            return false;
//        }
//
//        auto parseOne = [&](const nlohmann::ordered_json& item, CardRecord& outRec)->bool
//        {
//            if (!item.is_object())
//            {
//                return false;
//            }
//
//            if (!item.contains("Pack ID") || !item.contains("Card ID") || !item.contains("Card Name"))
//            {// タグが見つからなかった場合
//                return false;
//            }
//
//            // BaseParam組み立て
//            try
//            {
//                outRec.baseParam.nPackID = item["Pack ID"].get<int>();  // パック番号
//                outRec.baseParam.nCardID = item["Card ID"].get<int>();  // カード番号
//            }
//            catch (const nlohmann::json::exception& e) {
//                // JSON ライブラリ由来のエラー（キー存在はしていたが型が違う等）
//                std::cerr << "JSON parse error for Pack ID/Card ID: " << e.what() << '\n';
//                return false;
//            }
//            catch (const std::exception& e) {
//                // その他の標準例外
//                std::cerr << "unexpected error reading Pack ID/Card ID: " << e.what() << '\n';
//                return false;
//            }
//
//            outRec.baseParam.Name = item.value("Card Name", item.value("name", std::string{}));                             // カード名
//            outRec.baseParam.Ruby = item.value("Card Name Ruby", item.value("ruby", std::string{}));                        // カード名の読み方
//            outRec.baseParam.nCost = item.value("cost", 0);                                                                 // コスト
//            outRec.baseParam.ImagePath = item.value("image", item.value("imagepath", std::string{}));                       // イラスト画像
//            outRec.baseParam.Maintype = static_cast<CardType>(item.value("type", static_cast<int>(CardType::NONE)));        // カードの種類
//            outRec.baseParam.Raritytype = static_cast<RARITY>(item.value("rarity", static_cast<int>(RARITY::NONE_RARITY))); // レアリティ
//            outRec.type = outRec.baseParam.Maintype;
//
//            // 個別パラメーター設定
//            switch (outRec.type)
//            {
//            case ATTACK:    // 攻撃タイプのとき
//                try
//                {
//                    outRec.attackParam.BaseParam = outRec.baseParam;
//                    outRec.attackParam.nDamage = item.value("power", item.value("nDamage", 0));
//                    outRec.attackParam.Attacktype = static_cast<AttackType>(item["attacktype"].get<int>());
//
//                }
//                catch (const nlohmann::json::exception& e)
//                {
//                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
//                    return false;
//                }
//                break;
//            case DEFENSE:   // 守備タイプののとき
//                try
//                {
//                    outRec.defenseParam.BaseParam = outRec.baseParam;
//                    outRec.defenseParam.nGuard = item.value("guard", item.value("nGuard", 0));
//                    outRec.defenseParam.nCounter = item.value("counter", item.value("nCounter", 0));
//                    outRec.defenseParam.Defensetype = static_cast<DefenseType>(item["defensetype"].get<int>());
//                }
//                catch (const nlohmann::json::exception& e)
//                {
//                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
//                    return false;
//                }
//                break;
//            case ASSIST:
//                try
//                {
//                    outRec.assistParam.BaseParam = outRec.baseParam;
//                    outRec.assistParam.bIsOneTime = item.value("isOneTime", false);                         // 効果が単発かどうか
//
//                    if (!outRec.assistParam.bIsOneTime)
//                    {// 単発じゃないときに時間の設定
//                        outRec.assistParam.nTime = item.value("time", 0);
//                    }
//
//                    outRec.assistParam.AssistType = static_cast<AssistType>(item["assisttype"].get<int>()); // アシストの種類
//                    switch (outRec.assistParam.AssistType)
//                    {// アシストの種類
//                    case OBSTRUCT:  // 妨害
//                        break;
//                    case BUFF:      // バフ
//                        outRec.assistParam.Bufftype = static_cast<BuffType>(item["bufftype"].get<int>());   // バフの種類
//                        if (outRec.assistParam.Bufftype == HEAL)
//                        {// 回復の場合
//                            outRec.assistParam.Healtype = static_cast<HealType>(item["healtype"].get<int>());   // 回復対象
//                            outRec.assistParam.nHeal = item.value("heal", 0);
//                        }
//                        break;
//                    case DEBUFF:    // デバフ
//                        break;
//                    }
//                }
//                catch (const nlohmann::json::exception& e)
//                {
//                    std::cerr << "JSON parse error in ATTACK: " << e.what() << '\n';
//                    return false;
//                }
//                break;
//
//            default:
//                break;
//            }
//
//            return true;
//        };
//
//        CardRecord newRec{};
//        if (!parseOne(j, newRec))
//        {
//             std::cerr << "パースに失敗" << std::endl;
//             return false;
//        }
//
//        m_Record = std::move(newRec);
//        m_Param = m_Record.baseParam;
//
//        // 型を合わせるために変換
//        CCard_Client::Param clientparam{};
//        clientparam.nPackID = m_Record.baseParam.nPackID;                                                       // パック番号
//        clientparam.nCardID = m_Record.baseParam.nCardID;                                                       // カード番号
//        clientparam.Name = m_Record.baseParam.Name;                                                             // カード名
//        clientparam.Ruby = m_Record.baseParam.Ruby;                                                             // カード名の読み方
//        clientparam.nCost = m_Record.baseParam.nCost;                                                           // コスト
//        clientparam.Maintype = static_cast<CCard_Client::CardType>(m_Record.baseParam.Maintype);                // カードのタイプ
//        clientparam.Raritytype = static_cast<CCard_Client::RARITY>(m_Record.baseParam.Raritytype);              // カードレアリティ
//        clientparam.ImagePath = m_Record.baseParam.ImagePath;                                                   // カードイラスト画像
//
//        switch (m_Record.baseParam.Maintype)
//        {
//        case ATTACK:    // 攻撃タイプ
//            clientparam.Cardparam.attackParam.nDamage = m_Record.attackParam.nDamage;         // 攻撃力
//            clientparam.Cardparam.attackParam.Attacktype = static_cast<CCard_Client::AttackType>(m_Record.attackParam.Attacktype); // 攻撃対象
//            break;
//        case DEFENSE:   // 守備タイプ
//            clientparam.Cardparam.defenseParam.nGuard = m_Record.defenseParam.nGuard;   // ガード値
//            clientparam.Cardparam.defenseParam.Defensetype = static_cast<CCard_Client::DefenseType>(m_Record.defenseParam.Defensetype); // カウンターの有無
//            if (clientparam.Cardparam.defenseParam.Defensetype == COUNTER)
//            {// カウンターするカードのみカウンター値を設定
//                clientparam.Cardparam.defenseParam.nCounter = m_Record.defenseParam.nCounter;
//            }
//
//            break;
//        case ASSIST:    // アシストタイプ
//            clientparam.Cardparam.assistParam.bIsOneTime = m_Record.assistParam.bIsOneTime; // 効果が単発かどうか
//            if (!clientparam.Cardparam.assistParam.bIsOneTime)
//            {// カードの効果が単発じゃないとき
//                clientparam.Cardparam.assistParam.nTime = clientparam.Cardparam.assistParam.nTime;  // 効果時間
//            }
//            clientparam.Cardparam.assistParam.Assisttype = static_cast<CCard_Client::AssistType>(m_Record.assistParam.AssistType);  // アシストの種類
//
//            switch (clientparam.Cardparam.assistParam.Assisttype)
//            {
//            case OBSTRUCT:  // 妨害カードのとき
//                break;
//            case BUFF:      // バフカードのとき
//                clientparam.Cardparam.assistParam.Bufftype = static_cast<CCard_Client::BuffType>(m_Record.assistParam.Bufftype);    // バフの種類
//
//                switch (clientparam.Cardparam.assistParam.Bufftype)
//                {
//                case HEAL:  // 回復カードのとき
//                    clientparam.Cardparam.assistParam.nHeal = clientparam.Cardparam.assistParam.nHeal;  // 回復量
//                    clientparam.Cardparam.assistParam.Healtype = static_cast<CCard_Client::HealType>(m_Record.assistParam.Healtype);    // 回復対象
//                    break;
//                }
//                break;
//            case DEBUFF:    // デバフカードのとき
//                break;
//            }
//            break;
//        }
//
//        // 読み込んだカード情報を登録
//        My::CCardManager::GetInstance()->RegistCardList(clientparam);
//
//        return true;
//    }
//    catch (const nlohmann::json::exception& je)
//    {
//        std::cerr << "LoadJson json exception: " << je.what() << '\n';
//        return false;
//    }
//    catch (const std::exception& e)
//    {
//        std::cerr << "LoadJson exception: " << e.what() << '\n';
//        return false;
//    }
//
//}
//
///**
//* @brief バイト列からカードデータの構築
//* @param [in]バイト列
//* @return 成功したかどうか
//*/
//bool My::CCardInfo::LoadBytes(const std::vector<uint8_t>& bytes)
//{
//    if (bytes.empty())
//    {// 情報がない場合
//        std::cerr << "LoadBytes:input bytes empty" << std::endl;
//
//        return false;
//    }
//
//    try
//    {
//        nlohmann::ordered_json root = nlohmann::ordered_json::from_cbor(bytes);
//
//        if (root.is_array())
//        {
//            bool any = false;
//            for (const auto& elem : root)
//            {
//                if (!elem.is_object())
//                {
//                    continue;
//                }
//
//                My::CCardInfo tmp;
//                if (!tmp.LoadJson(elem))
//                {
//                    std::cerr << "LoadBytes: LoadJson failed for an array element; skipping" << std::endl;
//                    continue;
//                }
//                any = true;
//            }
//            return any;
//        }
//        else if (root.is_object())
//        {
//            if (!this->LoadJson(root)) 
//            {
//                std::cerr << "LoadBytes: LoadJson failed for object\n";
//                return false;
//            }
//            return true;
//        }
//        else
//        {
//            std::cerr << "LoadBytes: unexpected CBOR root type\n";
//            return false;
//        }
//    }
//    catch (const std::exception& e) 
//    {
//        std::cerr << "LoadBytes exception: " << e.what() << '\n';
//        return false;
//    }
//
//    return true;
//}
//
///**
//* @brief URLからデータ取得
//* @param [in]githubのURL
//* @param [in]認証トークン
//* @return 成功したかどうか
//*/
//bool My::CCardInfo::LoadUrl(const std::string& url, const std::string& token, const std::shared_ptr<IHttpClient>& client)
//{    
//    if (!client)
//    {
//        return false;
//    }
//
//    // ヘッダ設定
//    if (!token.empty())
//    {
//        client->SetHeader("Authorization", std::string("Bearer ") + token);
//    }
//
//    client->SetHeader("Accept", "application/json");
//
//    client->SetHeader("User-Agent", "MyApp/1.0");
//
//    const int timeoutMs = 15000;
//    HttpResponse res = client->Get(url, timeoutMs);
//
//    return false;
//}
//
///**
//* @brief パラメーター取得
//* @return パラメーター
//*/
//My::CCardInfo::BaseParam My::CCardInfo::GetParam()
//{
//	return m_Param;
//}
