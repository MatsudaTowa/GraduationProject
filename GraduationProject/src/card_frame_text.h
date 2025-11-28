//===========================================================================================================================================================
// 
// card_frame_text.cppのヘッダー [card_frame_text.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_TEXT_H_
#define _CARD_FRAME_TEXT_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"
#include "font_manager.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardFrameText :public CCardFrame
	{
	public:
		CCardFrameText(int nPriority);
		~CCardFrameText() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
	};
}
#endif