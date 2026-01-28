//=============================================
//
//数字管理「number.h」
// Author松田永久
//
//=============================================
#ifndef _ZONE_NUM_H_ //これが定義されてないとき
#define _ZONE_NUM_H_
#include "main.h"
#include "object2D.h"
#include "billboard.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My 数字(2D)クラス */
	class CZoneNum :public CNumber_2D
	{
	public:
		static const int NUMBER_PRIORITY = 9;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CZoneNum(int nPriority = NUMBER_PRIORITY):CNumber_2D(nPriority)
		{};

		/**
		 * @brief 数字生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @param [in]テクスチャ番号
		 * @return 数字の2Dポインタ
		 */
		static CZoneNum* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int idx);
	private:
	};
}
#endif