//===========================================================================================================================================================
// 
// めくったカードを確認するゾーンの処理 [flip_preview_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================
#ifndef _FLIP_PREVIEW_ZONE_H_
#define _FLIP_PREVIEW_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief めくったカードを確認するゾーンのクラス
	*/
	class CFlipPreviewZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CFlipPreviewZone();

		/**
		* @brief デストラクタ
		*/
		~CFlipPreviewZone()override;

		/**
		* @brief カードを送る
		* @param [in]カードのポインタ
		*/
		void SendToZone(CCard* pCard);

	};
}
#endif