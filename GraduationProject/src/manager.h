//=============================================
//
//�}�l�[�W���[[maneger.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MANAGER_H_ //���ꂪ��`����ĂȂ��Ƃ�

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

/** @brief My ���O��� */
namespace My
{
	/** @brief My �}�l�[�W���[�N���X */
	class CManager
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 */
		CManager();

		/**
		 * @brief �f�X�g���N�^
		 */		
		~CManager();

		HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
		void Uninit();
		void Update();
		void Draw();

		//�V�[���ݒ�
		void SetMode(CScene::MODE mode);

		static CManager* GetInstance()
		{
			static CManager instance; //�ÓI�C���X�^���X
			return &instance;
		}

		/**
		 * @brief �����_���[�擾
		 * @return �����_���[�|�C���^
		 */
		CRenderer* GetRenderer();
		/**
		 * @brief �L�[�{�[�h�擾
		 * @return �L�[�{�[�h�̃|�C���^
		 */
		CInputKeyboard* GetKeyboard();
		/**
		 * @brief �}�E�X�擾
		 * @return �}�E�X�̃|�C���^
		 */		
		CInputMouse* GetMouse();
		/**
		 * @brief pad�擾
		 * @return pad�̃|�C���^
		 */			
		CInputPad* GetPad();
		/**
		 * @brief �J�����擾
		 * @param [in]�J�����̔ԍ�
		 * @return �J����
		 */		
		CCamera* GetCamera(int idx);
		/**
		 * @brief �J�������擾
		 * @return �J�����̌�
		 */
		int GetCameraSize();
		/**
		 * @brief ���݉f���Ă���J�����̔ԍ��擾
		 * @return ���݉f���Ă���J�����̔ԍ�
		 */
		int GetCureentViewCamera();
		/**
		 * @brief ���݉f���Ă���J�����̔ԍ��ݒ�
		 * @param ���݉f���Ă���J�����̔ԍ�
		 */
		void SetCureentViewCamera(int idx);	
		/**
		 * @brief �J������S�폜
		 */
		void ClearCamera();
		/**
		 * @brief �w�肳�ꂽ�J�����폜
		 */
		void EraseCamera(unsigned int idx);
		/**
		 * @brief ���C�g�擾
		 * @return ���C�g�̃|�C���^
		 */
		CLight* GetLight();
		/**
		 * @brief �e�N�X�`���擾
		 * @return �e�N�X�`���̃|�C���^
		 */
		CTexture* GetTexture();
		/**
		 * @brief ���f���擾
		 * @return ���f���̃|�C���^
		 */
		CModel* GetModel();
		/**
		 * @brief �����蔻��擾
		 * @return �����蔻��̃|�C���^
		 */
		CColision* GetColision();
		/**
		 * @brief �t�F�[�h�擾
		 * @return �t�F�[�h�̃|�C���^
		 */
		CFade* GetFade();

		/**
		 * @brief �J��������
		 * @param [in]�J�����̃|�C���^
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