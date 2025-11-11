//===========================================================================================================================================================
// 
// ゾーンの処理 [deck.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _DECK_H_
#define _DECK_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 山札のクラス
	*/
	class CDeckZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CDeckZone();

		/**
		* @brief デストラクタ
		*/
		~CDeckZone();

		/**
		* @brief めくる処理
		* @param [in]枚数
		*/
		void Flip(int sheet);

		/**
		* @brief シャッフル処理
		*/
		void Shuffle();

	};
}
#endif