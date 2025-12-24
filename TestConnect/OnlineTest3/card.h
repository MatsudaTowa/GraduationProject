//================================
//
// カード用のヘッダー[card.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CARD_H_
#define _CARD_H_

//ヘッダーのインクルード
#include "main.h"
#include "card_state.h"
#include "card_manager.h"
#include "BitStream.h"

namespace My
{
	class CDuel_Player;
	class CCardStrategy_Base;
	//class CZone;

	//カードのクラス
	class CCard
	{
	public:

		//ゾーンの列挙
		enum ZONE
		{//ゾーンの種類
			NONE_ZONE,
			DECK,
			HAND,
			CAST,
			WAIT,
			FIELD,
			FLIP,
			CEMETERY
		};

		//キャスト先の列挙
		enum CastDestination
		{
			NONE = 0,	//無し
			AREA,		//エリア
			CARD,		//カード
			MAX
		};

		//関数
		CCard();			//コンストラクタ
		virtual ~CCard();	//デストラクタ

		/**
		* @brief 初期化
		*/
		virtual HRESULT Init() { return S_OK; }

		/**
		 * @brief 終了
		 */
		virtual void Uninit() {}

		/**
		 * @brief 更新
		 */
		virtual void Update() {}
		void Update(CDuel_Player* duel);

		/**
		* @brief 固有情報読み込み
		* @param [in]param
		*/
		virtual void LoadUniqueInfo(CCard_Client::Param param) {}

		/**
		 * @brief キャスト処理
		 */
		virtual void Cast(CDuel_Player* player) {}

		/**
		 * @brief ステイ処理
		 */
		virtual void Stay() {}

		/**
		 * @brief トリガー処理
		 */
		virtual void Trigger() {}

		/**
		 * @brief 現在のゾーン取得
		 * @return 現在のゾーン
		 */
		inline ZONE GetCurrentZone() { return m_CurrentZone; }

		/**
		 * @brief 過去のゾーン取得
		 * @return 過去のゾーン
		 */
		inline ZONE GetOldZone() { return m_OldZone; }

		/**
		 * @brief 現在のゾーン取得
		 * @param 現在のゾーン
		 */
		inline void SetCurrentZone(ZONE zone) { m_CurrentZone = zone; }

		/**
		 * @brief 過去のゾーン取得
		 * @param 過去のゾーン
		 */
		inline void SetOldZone(ZONE zone) { m_OldZone = zone; }

		/**
		 * @brief ステータス取得
		 * @return m_BaseStatus
		 */
		inline CCard_Client::BaseParam GetBaseStatus() { return m_BaseParam; }

		/**
		 * @brief ステータス設定
		 * @param [in]status
		 */
		inline void SetBaseStatus(CCard_Client::BaseParam status) { m_BaseParam = status; }

		/**
			* @brief カードタイプ取得
			*/
		inline void SetCardType(CCard_Client::CardType type) { m_CardType = type; }
		inline CCard_Client::CardType GetCardType() { return m_CardType; }

		/**
		* @brief 状態を変更する
		*/
		//void ChangeState(CCardState* state);
		void ChangeState(CCardState::CARD_STATE state, CDuel_Player* duel);

		/**
		* @brief ステート取得
		*/
		inline CCardState* GetState() { return m_pState; }
		inline CCardState::CARD_STATE GetStateNum() { return m_StateNum; }

		//ストラテジーの設定と取得

		/**
		 * @brief 計算前の効果処理設定
		 * @param [in]strategy_vector
		 */
		void SetPreCalculateStrategyVector(std::vector<CCardStrategy_Base*> strategy_vector) { m_PreCalculateStrategy = strategy_vector; }

		/**
		 * @brief 計算後の効果処理設定
		 * @param [in]strategy_vector
		 */
		void SetpostCalculateStrategyVector(std::vector<CCardStrategy_Base*> strategy_vector) { m_PostCalculateStrategy = strategy_vector; }

		/**
		 * @brief 効果前の効果取得
		 * @return [in]m_PreCalculateStrategy
		 */
		std::vector<CCardStrategy_Base*> GetPreCalculateStrategyVector() { return m_PreCalculateStrategy; }

		/**
		 * @brief 効果後の効果取得
		 * @return [in]m_PreCalculateStrategy
		 */
		std::vector<CCardStrategy_Base*> GetPostCalculateVector() { return m_PostCalculateStrategy; }

		//効果の追加
		void AddPreCalculateStrategy(CCardStrategy_Base* strategy) { m_PreCalculateStrategy.push_back(strategy); }		//計算前効果の追加
		void AddPostCalculateStrategy(CCardStrategy_Base* strategy) { m_PostCalculateStrategy.push_back(strategy); }	//計算後効果の追加

		//設定と取得
		CCard_Client::Param& GetParam() { return m_Param; }						//基本のパラメータ取得
		CCard_Client::AttackParam GetAttackParam(CCard_Client::Param param);	//攻撃のパラメータ取得
		CCard_Client::DefenseParam GetDefenseParam(CCard_Client::Param param);	//防御のパラメータ取得
		CCard_Client::AssistParam GetAssistParam(CCard_Client::Param param);	//アシストパラメータ取得

		//対象の番号
		virtual void AddTargetIdVector(int id) { m_TargetIdVector.push_back(id); }		//追加
		void SetTargetIdVector(std::vector<int> vector) { m_TargetIdVector = vector; }	//設定
		std::vector<int> GetTargetIdVector() { return m_TargetIdVector; }				//取得

		//使用者の番号
		void SetUserId(int id) { m_nUserId = id; }	//設定
		int GetUserId() { return m_nUserId; }		//取得

		//引数のゾーンの列挙に対応したポインタを返す
		CZone* CastToZone(ZONE zone, CDuel_Player* duel);

		//同種類の中のカード番号
		void SetSameTypeId(int id) { m_nSameTypeId = id; }
		int GetSameTypeId() { return m_nSameTypeId; }

		//キャスト開始時間
		void SetStartCastTime(uint64_t time) { m_fCastStartTime = time * 0.001f; }
		float GetStartCastTime() { return m_fCastStartTime; }

		//トリガー時に送るデータ
		virtual void SendTriggerData(RakNet::BitStream* bsout);

		/**
		 * @brief キャスト先の列挙取得
		 * @return キャスト先の列挙
		 */
		inline CastDestination GetCastDestination() { return m_CastDestination; }

		/**
		 * @brief キャスト先の列挙設定
		 * @param キャスト先の列挙値
		 */
		inline void SetCastDestination(CastDestination castdestination) { m_CastDestination = castdestination; }

		//キャストカードの読み込み
		virtual bool LoadCastInfo(RakNet::BitStream* bsin, CastDestination destination) = 0;

		//キャストカードの書き出し
		virtual void SendCastInfo(RakNet::BitStream* bsout) = 0;

	private:

		//関数
		//次の列挙から過去のゾーンのポインタを返す
		CZone* CastToEnumZone(ZONE zone, CDuel_Player* duel);

		//変数
		CCard_Client::Param m_Param;			//パラメータ
		CCard_Client::BaseParam m_BaseParam;	//ベースパラメータ
		CCard_Client::CardType m_CardType;		//カードの種類
		std::vector<int> m_TargetIdVector;		//対象の番号のベクター
		int m_nUserId;							//使用者の番号

		/**
		 * @brief ゾーンの状態
		 */
		ZONE m_CurrentZone;	//現在のゾーン
		ZONE m_OldZone;		//昔のゾーン

		/**
		* @brief カードのストラテジー
		*/
		std::vector<CCardStrategy_Base*> m_PreCalculateStrategy;	//!<効果前の効果
		std::vector<CCardStrategy_Base*> m_PostCalculateStrategy;	//!<効果後の効果

		/**
		 * @brief カードの状態
		 */
		CCardState* m_pState;

		/**
		 * @brief カードの状態列挙
		 */
		My::CCardState::CARD_STATE m_StateNum;

		/**
		* 同じ種類の何番目のカードか
		*/
		int m_nSameTypeId;

		/**
		* キャスト時間
		*/
		float m_fCastStartTime;

		/**
		* キャスト先
		*/
		CastDestination m_CastDestination;
	};
}

#endif