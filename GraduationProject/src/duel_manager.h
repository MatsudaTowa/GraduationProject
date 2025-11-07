////================================
////
////duel_manager.cppに必要な宣言[duel_manager.h]
////Author:松田永久
//// 
////================================
//
//#ifndef _DUEL_MANAGER_H_ //このマクロ定義がされていなかったら
//#define _DUEL_MANAGER_H_ //２重インクルード防止のマクロ定義
//
//namespace My
//{
//	class CActiveScenePlayer;
//	class CCard;
//	//デュエルマネージャーのクラス
//	class CDuelManager
//	{
//	public:
//
//		//メンバ関数
//		CDuelManager();			//コンストラクタ
//		~CDuelManager();			//デストラクタ
//		HRESULT Init();				//初期化
//
//		/**
//		 * @brief 終了
//		 */
//		void Uninit();
//
//		/**
//		 * @brief 自分自身のインスタンスを生成し取得
//		 */
//		static CDuelManager* GetInstance()
//		{
//			//静的インスタンス
//			static CDuelManager instance;
//			return &instance;
//		}
//		/**
//		 * @brief エリアを生成
//		 */
//		void CreateArea();
//
//		/**
//		 * @brief エリアの選択
//		 */
//		void SelectArea();
//
//		/**
//		 * @brief そこのエリアにカードを実行
//		 * @param [in]area
//		 */
//		void CardTrigger(My::CInputMouse::AREA area);
//
//		/**
//		 * @brief エリアの設定
//		 * @param [in]エリアのポインタ
//		 * @param [in]どこのエリアか
//		 */
//		void SetArea(CArea* area, int idx) { m_pArea[idx] = area; }
//
//		/**
//		 * @brief 中心エリアの設定
//		 * @param 中心エリア
//		 */
//		void SetCenterArea(CCenterArea* area) { m_pCenterArea = area; }
//
//		/**
//		 * @brief エリアの取得
//		 * @param [in]エリア番号
//		 * @return エリアのポインタ
//		 */
//		CArea* GetArea(int idx) { return m_pArea[idx]; }
//
//		/**
//		 * @brief 中心エリアの取得
//		 * @return 中心エリア
//		 */
//		CCenterArea* GetCenterArea() { return m_pCenterArea; }
//
//		/**
//		 * @brief 選択しているか取得
//		 * @return
//		 */
//		inline bool GetActive()
//		{
//			return m_isActive;
//		}
//
//		/**
//		 * @brief 選択されているか設定
//		 * @param [in]isSelect
//		 */
//		inline void SetActive(bool isActive)
//		{
//			m_isActive = isActive;
//		}
//
//	private:
//	};
//}
//
//#endif