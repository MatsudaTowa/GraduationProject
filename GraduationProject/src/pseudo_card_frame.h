//===========================================================================================================================================================
// 
// pseund_card_frame.cppのヘッダー [card_frame_base.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _PSEND_CARD_FRAME_H_
#define _PSEND_CARD_FRAME_H_

#include "main.h"
#include "card_frame.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CPsendCardFrame :public CCardFrame
	{
	public:
		CPsendCardFrame(int nPriority = 25);
		~CPsendCardFrame() override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;
		
		static CPsendCardFrame* Create(FRAMETYPE type, D3DXVECTOR3 pos,D3DXVECTOR3 rot);
	};
}
#endif