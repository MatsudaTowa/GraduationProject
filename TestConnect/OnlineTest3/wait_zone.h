//===========================================================================================================================================================
// 
// 待機ゾーンのヘッダー [wait_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _WAIT_ZONE_H_
#define _WAIT_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 待機ゾーンクラス
	*/
	class CWaitZone : public CZone
	{
	public:

		//定数
		static constexpr int MAX_CARD = 6;

		/**
		* @brief コンストラクタ
		*/
		CWaitZone();

		/**
		* @brief デストラクタ
		*/
		~CWaitZone()override;

		/**
		* @brief 守備のカードが置かれたとき送る
		* @param [in]カードのポインタ
		*/
		void SendDefenseCard(CCard*pCard);

		/**
		* @brief カードを並べる
		*/
		void LineUpCard();
	};
}

#endif