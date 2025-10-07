//===========================================================================================================================================================
// 
// カードステートのヘッダー [card_state.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_STATE_H_
#define _CARD_STATE_H_
#include "card_state.h"

class CCard;

namespace My
{
	/** 
	* @brief カードのステートクラス 
	*/
	class CCardState
	{
		CCardState() {};
		virtual ~CCardState() {};

		/** @brief ニュートラル */
		virtual void Neutral(CCard* pCard) {};
	};

	//class CCardStateNeutral :public CCardState
	//{
	//	void Neutral(CCard* pCard)override {};
	//};
}

#endif // !CARD_STATE_H_
