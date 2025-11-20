//===========================================================================================================================================================
// 
// card_frame_name.cppのヘッダー [card_frame_name.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_NAME_H_
#define _CARD_FRAME_NAME_H_

#include "main.h"
#include "card_frame_use_font.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardFrameName :public CCardFrameUseFont
	{
	public:
		CCardFrameName(int nPriority);
		~CCardFrameName() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
	};
}
#endif