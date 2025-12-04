//===========================================================================================================================================================
// 
// キャストプレビューゾーンのヘッダー [castpreview_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _CASTPREVIEW_ZONE_H_
#define _CASTPREVIEW_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief キャストプレビューゾーンクラス
	*/
	class CCastPreviewZone : public CZone
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		CCastPreviewZone();

		/**
		* @brief デストラクタ
		*/
		~CCastPreviewZone();

		/**
		* @brief 使用したカードを送る
		* @param [in]カードのポインタ
		*/
		void UsePlayerCard(CCard* pCard);

	};
}
#endif