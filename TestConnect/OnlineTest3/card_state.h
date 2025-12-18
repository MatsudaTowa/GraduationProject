//===========================================================================================================================================================
// 
// カードステートのヘッダー [card_state.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_STATE_H_
#define _CARD_STATE_H_

//#include "card.h"
#include "duel_player.h"

namespace My
{
	// 前方宣言
	class CCard;
	//class CDuel_Player;

	/** 
	* @brief カードのステートクラス 
	*/
	class CCardState
	{
	public:
		/**
		* @brief カードのステート列挙
		* @param 何もされていない状態
		* @param カードが選択されている状態
		* @param カードを呼び出す状態
		* @param カードの発動待機時間
		* @param カードの効果を発動する
		*/
		typedef enum
		{
			CARD_NONE = 0,
			CARD_NEUTRAL,
			CARD_PICKUP,
			CARD_SELECT,
			CARD_CAST,
			CARD_STAY,
			CARD_WAIT,
			CARD_TRIGGER,
			CARD_CEMETERY,
			CARD_MAX
		}CARD_STATE;

		/** @brief 初期化 */
		//virtual void Init() {};
		virtual void Init([[maybe_unused]]CCard* cpy, [[maybe_unused]] CDuel_Player* duel) {}

		/** @brief 終了 */
		virtual void Uninit();
		
		/** @brief 更新 */
		virtual void Update([[maybe_unused]] CCard* cpy, [[maybe_unused]] CDuel_Player* duel) {}

		/** @brief 設定 */
		void Copy([[maybe_unused]]CCard* cpy);

	protected:
		//My::CCard* m_pCardCpy;
	private:
	};


	/**
	* @brief ニュートラル状態
	*/
	class CCardStateNeutral :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;
	};


	/**
	* @brief 選択状態
	*/
	class CCardStatePickup :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;
	};

	/**
	* @brief 選択状態
	*/
	class CCardStateSelect :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;
	};

	/**
	* @brief 使用状態
	*/
	class CCardStateCast :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel) override;

	private:

		//キャスト後の状態変更
		void ChangeToState(CCard* cpy, CDuel_Player* duel);
	};

	/**
	* @brief 発動待機状態
	*/
	class CCardStateStay :public CCardState
	{
	public:

		/** @brief コンストラクタ */
		CCardStateStay();

		/** @brief デストラクタ */
		~CCardStateStay() {}


		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;

	private:

		/** @brief 対象によってカードの位置を設定 */
		void CountDown();

		//変数
		//RakNet::Time m_OldTime;		//前回の時間
		//RakNet::Time m_ElapsedTime;	//経過時間
		/** @brief 待機カウント */
		float m_fStaycount;
	};

	/**
	* @brief 守備待機状態
	*/
	class CCardStateWait :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;

	private:
	};

	/**
	* @brief 発動状態
	*/
	class CCardStateTrigger :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override;

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override;
	};

	/**
	* @brief 守備待機状態
	*/
	class CCardStateCemetery :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy, CDuel_Player* duel) override {}

		/** @brief 更新 */
		void Update(CCard* cpy, CDuel_Player* duel)override {}

	private:
	};
}

#endif // !CARD_STATE_H_
