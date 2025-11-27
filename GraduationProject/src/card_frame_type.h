//===========================================================================================================================================================
// 
// card_frame_type.cppのヘッダー [card_frame_type.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _CARD_FRAME_TYPE_H_
#define _CARD_FRAME_TYPE_H_

#include "main.h"
#include "card_frame.h"
#include "card_type_icon.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardFrameType :public CCardFrame
	{
	public:
		CCardFrameType(int nPriority);
		~CCardFrameType() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
		CCardTypeIcon* m_pTypeicon = nullptr;
	};
}
#endif