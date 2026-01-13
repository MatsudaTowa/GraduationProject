//===========================================================================================================================================================
// 
// card_info_ruby.cppのヘッダー [card_info_ruby.h]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#ifndef _CARD_INFO_RUBY_H_
#define _CARD_INFO_RUBY_H_

#include "main.h"
#include "card_info_use_font.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardInfoRuby :public CCardInfoUseFont
	{
	public:
		CCardInfoRuby(int nPriority = 27);
		~CCardInfoRuby() {};
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