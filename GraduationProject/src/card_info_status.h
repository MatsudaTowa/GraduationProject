//===========================================================================================================================================================
// 
// card_info_status.cppのヘッダー [card_info_status.h]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#ifndef _CARD_INFO_STATUS_H_
#define _CARD_INFO_STATUS_H_

#include "main.h"
#include "card_info_use_font.h"
#include "card.h"
#include "card_type_icon.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardInfoStatus :public CCardInfoUseFont
	{
	public:
		static const int STATUS_PRIORITY = 27;  //描画順

		CCardInfoStatus(int nPriority = STATUS_PRIORITY);
		~CCardInfoStatus() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;

		void SetUI()override;
		void SetIcon(std::vector<std::wstring>& statusText, int icon_type);
	private:
		std::vector<CCardTypeIcon*> m_IconVector;
	};
}
#endif