//===========================================
// 
// �V�[���}�l�[�W���[[scene_maneger.cpp]
// Auther:UedaKou
// 
//===========================================
#include "scene_maneger.h"	// �V�[���}�l�[�W���[
#include "../system/manager.h"		// �S�̃}�l�[�W���[
namespace My
{
	namespace Scene {

		//class CGameManager;	// 
		class CSene_Effekseer;
		//============================================
		// �R���X�g
		//============================================
		CSceneManager::CSceneManager() :
			CBase(new CCommonData()),
			m_pScene(makeScene<CSene_Effekseer>())
		{

		}
		//============================================
		// �f�X�g���N�^
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
		// �X�V
		//============================================
		CBase* CSceneManager::Update()
		{
			CBase* pBaseScene;
			// �V�[���X�V
			if (pBaseScene = m_pScene->Update())
			{
				return m_pScene;
			}

			if (pBaseScene != m_pScene)
			{// �Ԃ�l���ς���Ă�����V�[���ύX
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
		// �`��
		//============================================
		void CSceneManager::Draw() const
		{
			if (m_pScene != nullptr)
			{
				m_pScene->Draw();
			}
		}

		//============================================
		// �|�[�Y���擾
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