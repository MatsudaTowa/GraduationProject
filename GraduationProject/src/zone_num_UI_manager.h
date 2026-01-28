//=============================================
//
//zoneのUIマネージャー
// Author松田永久
//
//=============================================
#ifndef _ZONE_NUM_UI_MANAGER_H_ //これが定義されてないとき
#define _ZONE_NUM_UI_MANAGER_H_
#include "main.h"
#include "player_number_UI.h"
#include "cemetery_num_UI.h"
#include "zone_num_UI_BG.h"

namespace My
{
	//=============================================
	//プレイヤーのUIクラス
	//=============================================
	class CZoneNumUIManager
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CZoneNumUIManager();

		/**
		 * @brief デストラクタ
		 */
		~CZoneNumUIManager();
		/**
		 * @brief 初期化
		 * @param [in]キャラクターのポインタ
		 * @return 成功したか
		 */
		HRESULT Init(CActiveSceneCharacter* character);
		/**
		 * @brief 終了
		 */
		void Uninit();

		/**
		 * @brief 現在のプレイヤーUI設定
		 * @param [in]2D座標変換後
		 * @param [in]キャラクターのポインタ
		 */
		void SetCurrentCharacter_UI(D3DXVECTOR3 screen_pos, CActiveSceneCharacter* character);

		/**
		 * @brief エナジーUIの設定
		 * @param [in]エナジーUIのポインタ
		 */
		inline void SetCemeteryUI(CCemeteryNumUI* pCemeteryUI)
		{
			m_pCemeteryUI = pCemeteryUI;
		}

		/**
		 * @brief 体力UIの取得
		 * @return [in]体力UIのポインタ
		 */
		inline CCemeteryNumUI* GetCemeteryNumUI()
		{
			return m_pCemeteryUI;
		}

	private:
		CCemeteryNumUI* m_pCemeteryUI;
		CZoneNumUI_BG* m_pZoneNumBG;
	};
}


#endif