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
		/** @brief 初期化 */
		virtual void Init() {};

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
		/** @brief 終了 */
		void Uninit()override;

		/** @brief 更新 */
		void Update(CCard* cpy)override;
	};


	/**
	* @brief 選択状態
	*/
	class CCardStatePickup :public CCardState
	{
	public:
		/** @brief 終了 */
		void Uninit()override;

		/** @brief 更新 */
		void Update(CCard* cpy)override;
	};

	class CCardStateCast :public CCardState
	{
	public:
		/** @brief 終了 */
		void Uninit()override;

		/** @brief 更新 */
		void Update(CCard* cpy)override;
	};

	class CCardStateStay :public CCardState
	{

	};

	class CCardStatePlay :public CCardState
	{

	};
}

#endif // !CARD_STATE_H_
