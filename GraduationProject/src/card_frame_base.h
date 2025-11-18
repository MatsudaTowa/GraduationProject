//===========================================================================================================================================================
// 
// card_frame_base.cppのヘッダー [card_frame_base.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_BASE_H_
#define _CARD_FRAME_BASE_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class FrameBase :public CCardFrame
	{
		FrameBase();
		~FrameBase() {};
	};
}
#endif