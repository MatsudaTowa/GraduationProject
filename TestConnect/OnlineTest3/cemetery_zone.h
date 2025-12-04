//===========================================================================================================================================================
// 
// 墓地ゾーンの処理 [cemetery_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================
#ifndef _CEMETERY_ZONE_H_
#define _CEMRTERY_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 墓地のクラス
	*/
	class CCemeteryZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CCemeteryZone();

		/**
		* @brief デストラクタ
		*/
		~CCemeteryZone()override;

		/**
		* @brief 墓地にカードを送る
		* @param [in]カードのポインタ
		*/
		void SendToGrave(CCard* pCard);

	};
}
#endif