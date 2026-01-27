//=============================================
//
//ゾーンUI処理「cemetery_num_UI.h」
// Author松田永久
//
//=============================================
#ifndef _CEMETERY_NUM_UI_H_ //これが定義されてないとき
#define _CEMETERY_NUM_UI_H_
#include "main.h"
#include "object2D.h"
#include "zone_num_UI.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 体力クラス */
	class CCemeteryNumUI : public CZoneNumUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCemeteryNumUI();
		/**
		 * @brief デストラクタ
		 */
		~CCemeteryNumUI() override;
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init() override;
		/**
		 * @brief 終了
		 */
		void Uninit() override;
	private:
	};
}

#endif // DEBUG