//===========================================
// 
// �S�̃}�l�[�W���[[managerer.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
#include "renderer.h"					// �����_���[
#include "input.h"						// ����
#include "../engin/light/light.h"		// ���C�g
#include "../engin/camera/camera.h"		// �J����
#include "../engin/fog/fog.h"			// �t�H�O
#include "../object/2D/object_fade.h"	// �t�F�[�h
#include "../object/effect/EffekseerManager.h"	// �G�t�F�N�V�A�}�l�[�W���[

#include "../scene/scene_maneger.h"		// �V�[���}�l�[�W���[

//#include "../collision/collision_manager.h"	// �R���W�����}�l�[�W���[

#define MAX_TXT	(0x400)

namespace My
{

	class CEffekseerManager;
	class CManager
	{
	public:
		CManager();
		~CManager();
		HRESULT Init(HINSTANCE hInstance, HWND nWnd, BOOL bWindow);	// ������
		void Uninit();		// �I��
		void Update();		// �X�V
		void Draw();		// �`��

		static CManager* GetInstance()
		{
			static CManager instance;	// �ÓI�C���X�^���X
			return &instance;
		}

		CRenderer* GetRenderer() { return m_pRenderer; };	// �����_���[�擾
		CLight* GetLight() { return m_pLight; }				// ���C�g�擾
		CCamera* GetCamera() { return m_pCamera; }			// �J�����擾
		CFog* GetFog() { return m_pFog; }					// �t�H�O

		CInputKeyboard* GetInKey() { return m_pInkey; }	// �L�[�{�[�h�擾
		CInputMouse* GetMouse() { return m_pInMouse; }	// �}�E�X�擾
		CInputJoypad* GetJoyKey() { return m_pJoiKey; }	// �}�E�X�擾

		CEffekseerManager* GetEffekseerManager() { return m_pEffekseerManager; }	// �G�t�F�N�g�}�l�[�W���[�擾

		My::Scene::CBase* GetSceneManager() { return m_pSceneManager; }					// �V�[���}�l�[�W���[

		CText* GetDebugText() { return m_pDebugText; }	// �f�o�b�O�e�L�X�g�擾

		//CCollisionManager* GetCollisionManager() { return m_pCollisionManager; }	// �R���W�����}�l�[�W���[

		static void SetEnd(bool bEnd) { m_bEnd = bEnd; }
		static bool GetEnd() { return m_bEnd; }
	private:
		CRenderer* m_pRenderer;	// �����_���[

		CInputKeyboard* m_pInkey;				// �L�[�{�[�h
		CInputMouse* m_pInMouse;				// �}�E�X
		CInputJoypad* m_pJoiKey;				// �W���C�p�b�h

		CLight* m_pLight;		// ���C�g
		CCamera* m_pCamera;		// �J����
		CFog* m_pFog;			// �t�H�O

		CEffekseerManager* m_pEffekseerManager;	// �G�t�F�N�g�}�l�[�W���[

		Scene::CBase* m_pSceneManager;			// �V�[���}�l�[�W���[

		CText* m_pDebugText;				// �f�o�b�O�p�e�L�X�g

		//CCollisionManager* m_pCollisionManager;	// �R���W�����}�l�[�W���[

		static bool m_bEnd;	// �I���t���O
	};

}

#endif // !_MANAGER_H_