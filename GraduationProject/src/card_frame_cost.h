//===========================================================================================================================================================
// 
// card_frame_base.cppのヘッダー [card_frame_cost.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_COST_H_
#define _CARD_FRAME_COST_H_

#include "main.h"
#include "card_frame_use_font.h"
#include "card.h"
#include "font_manager.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardFrameCost :public CCardFrameUseFont
	{
	public:
		CCardFrameCost(int nPriority);
		~CCardFrameCost() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
	};
}
#endif