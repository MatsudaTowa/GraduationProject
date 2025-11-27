//=============================================
//
//カードのタイプイラスト[card_type_illust]
//Author Matsuda Towa
//
//=============================================
#ifndef _CARD_TYPE_ILLUST_H_ //これが定義されてないとき
#define _CARD_TYPE_ILLUST_H_

#include "main.h"
#include "card_frame.h"
#include "card_type_icon.h"
#include "card.h"

/** @brief My 名前空間 */
namespace My
{
	class CCardFrameIllust :public CCardFrame
	{
	public:
		CCardFrameIllust(int nPriority);
		~CCardFrameIllust() {};
		HRESULT Init()override;

		/* @brief 終了 */
		void Uninit()override;

		/* @brief 更新 */
		void Update()override;
	private:
	};
}

#endif