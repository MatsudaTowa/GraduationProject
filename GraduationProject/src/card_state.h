//===========================================================================================================================================================
// 
// カードステートのヘッダー [card_state.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_STATE_H_
#define _CARD_STATE_H_
#include "card_state.h"

namespace My
{
	// 前方宣言
	class CCard;

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
			CARD_NEUTRAL = 0,
			CARD_PICKUP,
			CARD_CAST,
			CARD_STAY,
			CARD_TRIGGER,
			CARD_MAX
		}CARD_STATE;

		/** @brief 初期化 */
		virtual void Init() {};
		virtual void Init(CCard* cpy) {};

		/** @brief 終了 */
		virtual void Uninit();
		
		/** @brief 更新 */
		virtual void Update(CCard* cpy) {};

		/** @brief 設定 */
		void Copy(CCard* cpy);

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
		/** @brief 更新 */
		void Update(CCard* cpy)override;
	};


	/**
	* @brief 選択状態
	*/
	class CCardStatePickup :public CCardState
	{
	public:
		/** @brief 更新 */
		void Update(CCard* cpy)override;
	};

	class CCardStateCast :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy) override;

		/** @brief 更新 */
		void Update(CCard* cpy) override;
	};

	class CCardStateStay :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init() override;

		/** @brief 更新 */
		void Update(CCard* cpy)override;

	private:
		/** @brief 待機カウント */
		unsigned int m_Staycount;

		/** @brief 設定された待機時間 */
		const unsigned int STAY_TIME = ONE_SECOND * 3;
	};

	class CCardStateTrigger :public CCardState
	{
	public:
		/** @brief 初期化 */
		void Init(CCard* cpy) override;
	};
}

#endif // !CARD_STATE_H_
