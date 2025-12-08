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
//#include "card_client.h"
//#include "duel_player.h"
//#include "card_strategy.h"
#include "card_state.h"
#include "card_manager.h"

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
		void Update(CDuel_Player* duel) {}

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

		//効果の追加
		void AddPreCalculateStrategy(CCardStrategy_Base* strategy) { m_PreCalculateStrategy.push_back(strategy); }		//計算前効果の追加
		void AddPostCalculateStrategy(CCardStrategy_Base* strategy) { m_PostCalculateStrategy.push_back(strategy); }	//計算後効果の追加

		//設定と取得
		CCard_Client::Param& GetParam() { return m_Param; }						//基本のパラメータ取得
		CCard_Client::AttackParam GetAttackParam(CCard_Client::Param param);	//攻撃のパラメータ取得
		CCard_Client::DefenseParam GetDefenseParam(CCard_Client::Param param);	//防御のパラメータ取得
		CCard_Client::AssistParam GetAssistParam(CCard_Client::Param param);	//アシストパラメータ取得

		//対象の番号
		void AddTargetIdVector(int id) { m_TargetIdVector.push_back(id); }	//追加
		std::vector<int> GetTargetIdVector() { return m_TargetIdVector; }	//取得

		//使用者の番号
		void SetUserId(int id) { m_nUserId = id; }	//設定
		int GetUserId() { return m_nUserId; }		//取得

		//引数のゾーンの列挙に対応したポインタを返す
		CZone* CastToZone(ZONE zone, CDuel_Player* duel);

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
	};
}

#endif