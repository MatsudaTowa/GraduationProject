//===========================================
// 
// 全体マネージャー[managerer.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
#include "renderer.h"					// レンダラー
#include "input.h"						// 入力
#include "../engin/light/light.h"		// ライト
#include "../engin/camera/camera.h"		// カメラ
#include "../engin/fog/fog.h"			// フォグ
#include "../object/2D/object_fade.h"	// フェード
#include "../object/effect/EffekseerManager.h"	// エフェクシアマネージャー

#include "../scene/scene_maneger.h"		// シーンマネージャー

//#include "../collision/collision_manager.h"	// コリジョンマネージャー

#define MAX_TXT	(0x400)

namespace My
{

	class CEffekseerManager;
	class CManager
	{
	public:
		CManager();
		~CManager();
		HRESULT Init(HINSTANCE hInstance, HWND nWnd, BOOL bWindow);	// 初期化
		void Uninit();		// 終了
		void Update();		// 更新
		void Draw();		// 描画

		static CManager* GetInstance()
		{
			static CManager instance;	// 静的インスタンス
			return &instance;
		}

		CRenderer* GetRenderer() { return m_pRenderer; };	// レンダラー取得
		CLight* GetLight() { return m_pLight; }				// ライト取得
		CCamera* GetCamera() { return m_pCamera; }			// カメラ取得
		CFog* GetFog() { return m_pFog; }					// フォグ

		CInputKeyboard* GetInKey() { return m_pInkey; }	// キーボード取得
		CInputMouse* GetMouse() { return m_pInMouse; }	// マウス取得
		CInputJoypad* GetJoyKey() { return m_pJoiKey; }	// マウス取得

		CEffekseerManager* GetEffekseerManager() { return m_pEffekseerManager; }	// エフェクトマネージャー取得

		My::Scene::CBase* GetSceneManager() { return m_pSceneManager; }					// シーンマネージャー

		CText* GetDebugText() { return m_pDebugText; }	// デバッグテキスト取得

		//CCollisionManager* GetCollisionManager() { return m_pCollisionManager; }	// コリジョンマネージャー

		static void SetEnd(bool bEnd) { m_bEnd = bEnd; }
		static bool GetEnd() { return m_bEnd; }
	private:
		CRenderer* m_pRenderer;	// レンダラー

		CInputKeyboard* m_pInkey;				// キーボード
		CInputMouse* m_pInMouse;				// マウス
		CInputJoypad* m_pJoiKey;				// ジョイパッド

		CLight* m_pLight;		// ライト
		CCamera* m_pCamera;		// カメラ
		CFog* m_pFog;			// フォグ

		CEffekseerManager* m_pEffekseerManager;	// エフェクトマネージャー

		Scene::CBase* m_pSceneManager;			// シーンマネージャー

		CText* m_pDebugText;				// デバッグ用テキスト

		//CCollisionManager* m_pCollisionManager;	// コリジョンマネージャー

		static bool m_bEnd;	// 終了フラグ
	};

}

#endif // !_MANAGER_H_