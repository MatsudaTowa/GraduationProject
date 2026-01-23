//===========================================================================================================================================================
// 
// 選択可能ゾーンの処理 [cemetery_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================
#ifndef _SELECTABLE_ZONE_H_
#define _SELECTABLE_ZONE_H_

// include
#include "zone.h"
#include "selection_range.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 墓地のクラス
	*/
	class CSelectableZone : public CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CSelectableZone();

		/**
		* @brief デストラクタ
		*/
		~CSelectableZone()override;

		void CreateRange(D3DXVECTOR3 pos,D3DXVECTOR2 size,D3DXCOLOR col,bool isDraw, CSelectionRange::RANGE_TYPE type);

		void Update() override;

		CSelectionRange* GetSelectionRange() { return m_pRange; }
		void SetSelectionRange(CSelectionRange* pRange) { m_pRange = pRange; }
	private:
		CSelectionRange* m_pRange;

	};
}
#endif
