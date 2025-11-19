//===========================================================================================================================================================
// 
// card_frame_base.cppのヘッダー [card_frame_cost.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_COST_H_
#define _CARD_FRAME_COST_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"
#include "font_manager.h"

/** @brief My 名前空間 */
namespace My
{
	class FrameCost :public CCardFrame
	{
	public:
		FrameCost(int nPriority);
		~FrameCost() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
		CFontManager* m_pFontManager;
	};
}
#endif