//===========================================================================================================================================================
// 
// 手札ゾーンのヘッダー [hand_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _HAND_ZONE_H_
#define _HAND_ZONE_H_

#include "zone.h"
#include "selection_range.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 手札ゾーンクラス
	*/
	class CHandZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CHandZone();

		/**
		* @brief デストラクタ
		*/
		~CHandZone()override;

		/**
		* @brief 手札に追加されたもの追加
		* @param [in]カードのポインタ
		*/
		void AddHandZone(CCard* pCard);

		/**
		* @brief 使われたカードの判断
		* @param [in]カード使用フラグ
		*/
		void JudgeUseCard(bool bUse);
	};
}
#endif