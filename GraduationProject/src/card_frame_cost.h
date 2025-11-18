//===========================================================================================================================================================
// 
// card_frame_base.cppのヘッダー [card_frame_base.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_COST_H_
#define _CARD_FRAME_COST_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class FrameCost :public CCardFrame
	{
	public:
		FrameCost();
		~FrameCost() {};
	};
}
#endif