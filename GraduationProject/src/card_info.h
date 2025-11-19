//===========================================================================================================================================================
// 
// カード情報のヘッダー [card_info.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _CARD_INFO_H_
#define _CARD_INFO_H_

// include
#include "card_manager.h"
#include "../src/nlohmann/json.hpp"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief HTTP 抽象インターフェイス
	 *
	 * このインターフェイスはライブラリ内の簡易 HTTP GET 呼び出し用の抽象です。
	 * 実運用では既存の HTTP クライアント実装（iconnection など）に合わせて置き換えてください。
	 */
	struct IHttpClient {
		/**
		 * @brief 仮想デストラクタ
		 */
		virtual ~IHttpClient() = default;

		/**
		 * @brief 単純な GET メソッド
		 * @param[in] url 取得先 URL
		 * @param[out] outBody レスポンスボディ（バイナリを std::string に格納することを想定）
		 * @param[in] headers 追加ヘッダー（例: Authorization）
		 * @return HTTP ステータスコード（200 が成功）
		 */
		virtual int Get(const std::string& url, std::string& outBody, const std::vector<std::pair<std::string, std::string>>& headers) = 0;
	};

	/**
	* @brief カードの情報クラス
	*/
	class CCardInfo
	{
	public:
		enum RARITY
		{// レアリティの種類
			NONE_RARITY,
			COMMON,			// コモン
			UNCOMMON,		// アンコモン
			RARE,			// レア
			SUPERRARE,		// スーパーレア
			HYPERRARE,		// ハイパーレア
			ROYALRARE,		// ロイヤルレア
			EMPERORRARE,	// エンペラーレア
			GODRARE,		// ゴッドレア
			XRARE,			// エックスレア
		};

		enum CardType
		{// カードの種類
			NONE,
			ATTACK,
			DEFENSE,
			ASSIST,
		};

		enum AttackType
		{// 攻撃の種類
			NONE_ATTACK,			// 攻撃以外のときはこれ
			ALL_ATTACK,				// 全体攻撃
			SPECIFIC_ATTACK,		// 特定の相手を選んで攻撃
			RANDOM_ATTACK,			// ランダム攻撃
			SELFINTARGET_ATTACK,	// 自分を含めた攻撃
		};

		enum DefenseType
		{// 守備の種類
			NONE_DEFENSE,		// 守備以外のときはこれ
			COUNTER,			// 反撃できる
			NOT_COUNTER,		// 反撃できない
		};

		enum AssistType
		{// アシストの種類
			NONE_ASSIST,		// アシスト以外のときはこれ
			OBSTRUCT,			// 妨害
			BUFF,				// バフ
			DEBUFF				// デバフ
		};

		enum BuffType
		{// バフの種類
			NONE_BUFF,
			HEAL,				// HP回復
			NO_NAMEONE,			// 未定①
			NO_NAMETWO,			// 未定②
		};

		enum DeBuffType
		{// デバフの種類
			NONE_DEBUFF,
			DEATH,				// 死亡
		};

		enum HealType
		{// 回復の種類
			NONE_HEAL,					// 回復しない
			ALL_HEAL,					// 全体回復
			SPECIFIC_HEAL,				// 特定の相手を選んで回復
			RANDOM_HEAL,				// ランダム回復
			SELFINTARGET_HEAL,			// 自分を含めてランダム回復
			ONLY_ME,					// 自分だけ
		};

		//パラメータの構造体
		struct Param
		{
			int nPackID;							// パック番号
			int nCardID;							// カード番号
			std::string Name;						// カード名
			std::string Ruby;						// フリガナ
			int nCost = 0;							// カードコスト
			int nDamage = 0;						// ダメージ数
			int nGuard = 0;							// ガード値
			int nCounter = 0;						// 反撃値
			int nHeal = 0;							// 回復値
			CardType Maintype;						// カードの種類
			RARITY Raritytype;						// レアリティの種類
			AssistType Assisttype;					// アシストの種類
			AttackType Attacktype;					// 攻撃の種類
			DefenseType Defensetype;				// 守備の種類
			BuffType Bufftype;						// バフの種類
			HealType Healtype;						// 回復の種類
			std::string ImagePath;					// 画像ファイルパス
		};

		// パックのパラメーター
		struct PackParam
		{
			std::string name;	// パック名
			std::string ruby;	// パック名の読み方
		};

		//基本カードのパラメータ
		struct BaseParam
		{
			int nPackID;							// パック番号
			int nCardID;							// カード番号
			std::string Name;						// カード名
			std::string Ruby;						// フリガナ
			int nCost = 0;							// カードコスト
			CardType Maintype;						// カードの種類
			RARITY Raritytype;						// レアリティの種類
			std::string ImagePath;					// 画像ファイルパス
		};

		//アタックカードのパラメータ
		struct AttackParam
		{
			BaseParam BaseParam = {};	// 基本パラメータ
			int nDamage = 0;			// ダメージ数
			AttackType Attacktype;		// 攻撃の種類
		};

		//ディフェンスカードのパラメータ
		struct DefenseParam
		{
			BaseParam BaseParam = {};		// 基本パラメータ
			int nGuard = 0;					// ガード値
			int nCounter = 0;				// 反撃値
			DefenseType Defensetype;		// 守備の種類
		};

		//アシストカードのパラメータ
		struct AssistParam
		{
			BaseParam BaseParam = {};		// 基本パラメータ
			int nHeal = 0;					// 回復値
			AssistType AssistType;			// アシストの種類
			BuffType Bufftype;				// バフの種類
			HealType Healtype;				// 回復の種類
			bool bIsOneTime;				// カード効果の発動時間(true = 単発,false = 単発じゃない)
			int nTime;						// 単発じゃないときの発動時間
		};

		// カード登録用
		struct CardRecord
		{
			BaseParam baseParam;		// 基本パラメーター
			AttackParam attackParam;	// 攻撃カード特有のパラメーター
			DefenseParam defenseParam;	// 守備カード特有のパラメーター
			AssistParam assistParam;	// アシストカード特有のパラメーター
			CardType type = NONE;		// カードの種類
		};

		/**
		* @brief コンストラクタ
		*/
		CCardInfo();

		/**
		* @brief デストラクタ
		*/
		~CCardInfo();

		/**
		* @brief 初期化処理
		* @return 成功したかどうか
		*/
		HRESULT Init();

		/**
		* @brief インスタンスの取得
		* @return インスタンス
		*/
		static CCardInfo* GetInstance()
		{
			static CCardInfo instance; //静的インスタンス
			return &instance;
		}

		/**
		* @brief JSONからカードデータの構築
		* @param [in]JSONオブジェクト
		*/
		bool LoadJson(const nlohmann::json& j);

		/**
		* @brief バイト列からカードデータの構築
		* @param [in]バイト列
		* @return 成功したかどうか
		*/
		bool LoadBytes(const std::vector<uint8_t>& bytes);

		/**
		* @brief URLからデータ取得
		* @param [in]githubのURL
		* @param [in]認証トークン
		* @return 成功したかどうか
		*/
		bool LoadUrl(const std::string& url, const std::string& token, const std::shared_ptr<IHttpClient>& client);

		/**
		* @brief パラメーター取得処理
		* @return パラメーター
		*/
		BaseParam GetParam();

	private:
		BaseParam m_Param;
		CardRecord m_Record;
	};
}
#endif