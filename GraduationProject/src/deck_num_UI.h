//=============================================
//
//デッキUI処理「deck_num_UI.h」
// Author松田永久
//
//=============================================
#ifndef _DECK_NUM_UI_H_ //これが定義されてないとき
#define _DECK_NUM_UI_H_
#include "main.h"
#include "object2D.h"
#include "zone_num_UI.h"
#include "deck_num_UI_bg.h"
/** @brief My 名前空間 */
namespace My
{
	/** @brief 体力クラス */
	class CDeckNumUI : public CZoneNumUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CDeckNumUI();
		/**
		 * @brief デストラクタ
		 */
		~CDeckNumUI() override;
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init() override;
		/**
		 * @brief 終了
		 */
		void Uninit() override;

		CDeckNumUI_BG* GetBG() { return m_pBG; }
	private:
		CDeckNumUI_BG* m_pBG;
	};
}

#endif // DEBUG