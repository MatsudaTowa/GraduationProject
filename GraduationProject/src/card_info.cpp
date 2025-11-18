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
        };

        CardRecord newRec{};
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

    return true;
}

/**
* @brief バイト列からカードデータの構築
* @param [in]バイト列
* @return 成功したかどうか
*/
bool My::CCardInfo::LoadBytes(const std::vector<uint8_t>& bytes)
{
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
    return false;
}

/**
* @brief パラメーター取得
* @return パラメーター
*/
My::CCardInfo::BaseParam My::CCardInfo::GetParam()
{
	return m_Param;
}
