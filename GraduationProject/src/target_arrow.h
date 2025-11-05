//=============================================
//
// ターゲットアロー[target_arrow.h]
// Author : 梅田爽真
//
//=============================================
#ifndef _TARGET_ARROW_ //これが定義されてないとき
#define _TARGET_ARROW_
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief テクスチャ管理クラス */
	class CTargetArrow:public CObject2D
	{
	public:
		CTargetArrow();
		~CTargetArrow()override;
		HRESULT Init()override;

	private:
		
	};
}

#endif