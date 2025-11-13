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
#include <unordered_map>
#include <shared_mutex>
#include <vector>
#include <optional>
#include <string>
#include <memory>

/** @brief My 名前空間 */
namespace My
{
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
			BuffType Bufftype;				// バフの種類
			HealType Healtype;				// 回復の種類
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
		* @brief 終了処理
		*/
		void Uninit();

		/**
		* @brief 更新処理
		*/
		void Update();

		//インスタンス
		static CCardInfo* GetInstance()
		{
			static CCardInfo instance; //静的インスタンス
			return &instance;
		}

		/**
		* @brief 読み込み処理
		* @param [in]バイナリデータ
		*/
	};
}
#endif