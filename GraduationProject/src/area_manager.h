//================================
//
//area_manager.cppに必要な宣言[area_manager.h]
//Author:松田永久
// 
//================================

#ifndef _AREA_MANAGER_H_ //このマクロ定義がされていなかったら
#define _AREA_MANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"
#include "area.h"
#include "center_area.h"

namespace My
{
	//エネミーマネージャーのクラス
	class CAreaManager
	{
	public:

		//メンバ関数
		CAreaManager();			//コンストラクタ
		~CAreaManager();			//デストラクタ
		HRESULT Init();				//初期化
		void Uninit();				//終了
		/**
		 * @brief エリアを生成
		 */
		void CreateArea();

		/**
		 * @brief エリアの選択
		 */
		void SelectArea();

		/**
		 * @brief エリアの設定
		 * @param [in]エリアのポインタ
		 * @param [in]どこのエリアか
		 */
		void SetArea(CArea* area, int idx) { m_pArea[idx] = area; }

		/**
		 * @brief 中心エリアの設定
		 * @param 中心エリア
		 */
		void SetCenterArea(CCenterArea* area) { m_pCenterArea = area; }

		/**
		 * @brief エリアの取得
		 * @param [in]エリア番号
		 * @return エリアのポインタ
		 */
		CArea* GetArea(int idx) { return m_pArea[idx]; }

		/**
		 * @brief 中心エリアの取得
		 * @return 中心エリア
		 */
		CCenterArea* GetCenterArea() { return m_pCenterArea; }

		/**
		 * @brief 選択しているか取得
		 * @return
		 */
		inline bool GetActive()
		{
			return m_isActive;
		}

		/**
		 * @brief 選択されているか設定
		 * @param [in]isSelect
		 */
		inline void SetActive(bool isActive)
		{
			m_isActive = isActive;
		}

	private:
		CArea* m_pArea[CInputMouse::AREA::MAX - 1]; //真ん中のエリア以外に三角形を生成
		CCenterArea* m_pCenterArea;					//真ん中のエリア
		bool m_isActive; //実行するか
	};
}

#endif