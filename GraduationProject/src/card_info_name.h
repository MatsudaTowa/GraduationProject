//===========================================================================================================================================================
// 
// card_info_name.cppのヘッダー [card_info_name.h]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#ifndef _CARD_INFO_NAME_H_
#define _CARD_INFO_NAME_H_

#include "main.h"
#include "card_info_use_font.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardInfoName :public CCardInfoUseFont
	{
	public:
		CCardInfoName(int nPriority = 25);
		~CCardInfoName() {};
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