//=============================================
//
//マネージャー[maneger.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MANAGER_H_ //これが定義されてないとき

#define _MANAGER_H_
#include "main.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "model.h"
#include "useful.h"
#include "object.h"
#include "colision.h"
#include "scene.h"
#include "fade.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief My マネージャークラス */
	class CManager
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CManager();

		/**
		 * @brief デストラクタ
		 */		
		~CManager();

		HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
		void Uninit();
		void Update();
		void Draw();

		//シーン設定
		void SetMode(CScene::MODE mode);

		static CManager* GetInstance()
		{
			static CManager instance; //静的インスタンス
			return &instance;
		}

		/**
		 * @brief レンダラー取得
		 * @return レンダラーポインタ
		 */
		CRenderer* GetRenderer();
		/**
		 * @brief キーボード取得
		 * @return キーボードのポインタ
		 */
		CInputKeyboard* GetKeyboard();
		/**
		 * @brief マウス取得
		 * @return マウスのポインタ
		 */		
		CInputMouse* GetMouse();
		/**
		 * @brief pad取得
		 * @return padのポインタ
		 */			
		CInputPad* GetPad();
		/**
		 * @brief カメラ取得
		 * @param [in]カメラの番号
		 * @return カメラ
		 */		
		CCamera* GetCamera(int idx);
		/**
		 * @brief カメラ個数取得
		 * @return カメラの個数
		 */
		int GetCameraSize();
		/**
		 * @brief 現在映しているカメラの番号取得
		 * @return 現在映しているカメラの番号
		 */
		int GetCureentViewCamera();
		/**
		 * @brief 現在映しているカメラの番号設定
		 * @param 現在映しているカメラの番号
		 */
		void SetCureentViewCamera(int idx);	
		/**
		 * @brief カメラを全削除
		 */
		void ClearCamera();
		/**
		 * @brief 指定されたカメラ削除
		 */
		void EraseCamera(unsigned int idx);
		/**
		 * @brief ライト取得
		 * @return ライトのポインタ
		 */
		CLight* GetLight();
		/**
		 * @brief テクスチャ取得
		 * @return テクスチャのポインタ
		 */
		CTexture* GetTexture();
		/**
		 * @brief モデル取得
		 * @return モデルのポインタ
		 */
		CModel* GetModel();
		/**
		 * @brief 当たり判定取得
		 * @return 当たり判定のポインタ
		 */
		CColision* GetColision();
		/**
		 * @brief フェード取得
		 * @return フェードのポインタ
		 */
		CFade* GetFade();

		/**
		 * @brief カメラ生成
		 * @param [in]カメラのポインタ
		 */
		void CreateCamera(CCamera* camera)
		{
			CCamera* pCamera = camera;
			if (pCamera == nullptr)
			{
				return;
			}
			pCamera->Init();

			m_pCamera.push_back(pCamera);

		}
	private:
		unsigned int m_CurrentViewCamera;

		CScene* m_pScene;
		CRenderer* m_pRenderer;
		CInputKeyboard* m_pKeyboard;
		CInputMouse* m_pMouse;
		CInputPad* m_pPad;
		std::vector<CCamera*> m_pCamera;
		CLight* m_pLight;
		CTexture* m_pTexture;
		CModel* m_pModel;
		CColision* m_pColision;
		CFade* m_pFade;
	};
}

#endif