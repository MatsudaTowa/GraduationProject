//===========================================
// 
// シーンマネージャー[scene_maneger.cpp]
// Auther:UedaKou
// 
//===========================================
#include "scene_maneger.h"	// シーンマネージャー
#include "../system/manager.h"		// 全体マネージャー
namespace My
{
	namespace Scene {

		//class CGameManager;	// 
		class CSene_Effekseer;
		//============================================
		// コンスト
		//============================================
		CSceneManager::CSceneManager() :
			CBase(new CCommonData()),
			m_pScene(makeScene<CSene_Effekseer>())
		{

		}
		//============================================
		// デストラクタ
		//============================================
		CSceneManager::~CSceneManager()
		{
			if (m_pScene != nullptr)
			{
				delete m_pScene;
			}
			delete m_pCommonData;
		}
		//============================================
		// 更新
		//============================================
		CBase* CSceneManager::Update()
		{
			CBase* pBaseScene;
			// シーン更新
			if (pBaseScene = m_pScene->Update())
			{
				return m_pScene;
			}

			if (pBaseScene != m_pScene)
			{// 返り値が変わっていたらシーン変更
				delete m_pScene;
				if (pBaseScene == nullptr)
				{
					CManager::GetInstance()->SetEnd(true);
				}
				m_pScene = pBaseScene;
			}
			return m_pScene;
		}
		//============================================
		// 描画
		//============================================
		void CSceneManager::Draw() const
		{
			if (m_pScene != nullptr)
			{
				m_pScene->Draw();
			}
		}

		//============================================
		// ポーズ中取得
		//============================================
		bool CSceneManager::GetPose()
		{
			if (m_pScene != nullptr)
			{
				return m_pScene->GetPose();
			}
			return false;
		}
	}
}