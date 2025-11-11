//===========================================================================================================================================================
// 
// フィールドゾーンのヘッダー [field_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _FIELD_ZONE_H_
#define _FIELD_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief フィールドクラス
	*/
	class CFieldZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CFieldZone();

		/**
		* @brief デストラクタ
		*/
		~CFieldZone();

		/**
		* @brief カードを送る
		* @param [in]カードのポインタ
		*/
		void UseCardSend(CCard* pCard);
	};
}
#endif