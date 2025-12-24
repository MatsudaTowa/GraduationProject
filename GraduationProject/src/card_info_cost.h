//===========================================================================================================================================================
// 
// card_info_cost.cppのヘッダー [card_info_cost.h]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#ifndef _CARD_INFO_COST_H_
#define _CARD_INFO_COST_H_

#include "main.h"
#include "card_info_use_font.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardInfoCost :public CCardInfoUseFont
	{
	public:
		static const int COST_PRIORITY = 27;  //描画順

		CCardInfoCost(int nPriority = COST_PRIORITY);
		~CCardInfoCost() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;

		void SetUI()override;
	private:
	};
}
#endif