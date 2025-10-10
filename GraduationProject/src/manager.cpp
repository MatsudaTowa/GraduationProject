//=============================================
//
//3DTemplate[manager.cpp]
//Author Matsuda Towa
//
//=============================================
#include "manager.h"
#include "renderer.h"

/**
 * @brief �R���X�g���N�^
 */
My::CManager::CManager():
m_pFade(nullptr),		//�t�F�[�h�̃|�C���^
m_pCamera(),		//�J�����̃|�C���^	
m_pColision(nullptr),	//�R���W�����̃|�C���^
m_pKeyboard(nullptr),	//�L�[�{�[�h�̃|�C���^
m_pLight(nullptr),		//���C�g�̃|�C���^
m_pModel(nullptr),		//���f���|�C���^
m_pMouse(nullptr),		//�}�E�X�̃|�C���^
m_pPad(nullptr),		//pad�̃|�C���^
m_pRenderer(nullptr),	//�����_���[�|�C���^
m_pScene(nullptr),		//�V�[���|�C���^
m_pTexture(nullptr),		//�e�N�X�`���Ǘ��|�C���^
m_CurrentViewCamera(INT_ZERO)	//�f���J�����ԍ�
{
	
}

/**
 * @brief �f�X�g���N�^
 */
My::CManager::~CManager()
{
}

/**
 * @brief ������
 * @param [in]�n���h���C���X�^���X
 * @param [in]�n���h�������h
 * @param [in]�E�B���h�E���[�h
 */
HRESULT My::CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	srand((unsigned)time(NULL) * 54321);  // �����n���������

	m_hWnd = hWnd;

	//������������
	if (m_pRenderer == nullptr)
	{
		m_pRenderer = new CRenderer();
		m_pRenderer->Init(hWnd,bWindow);
	}
	if (m_pKeyboard == nullptr)
	{
		m_pKeyboard = new CInputKeyboard();

		//�L�[�{�[�h�̏���������
		if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}
	if (m_pMouse == nullptr)
	{
		m_pMouse = new CInputMouse();

		//�}�E�X�̏���������
		if (FAILED(m_pMouse->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}
	if (m_pPad == nullptr)
	{
		m_pPad = new CInputPad();

		//�}�E�X�̏���������
		if (FAILED(m_pPad->Init(hInstance, hWnd)))
		{
			return E_FAIL;
		}
	}

	//�V�[������
	if (m_pScene == nullptr)
	{
		m_pScene = new CScene();
		m_pScene->Init();

	}

	//���C�g����
	if (m_pLight == nullptr)
	{
		m_pLight = new CLight();
		m_pLight->Init();
	}

	//�e�N�X�`������
	if (m_pTexture == nullptr)
	{
		m_pTexture = new CTexture();
	}

	//���f������
	if (m_pModel == nullptr)
	{
		m_pModel = new CModel();
	}

	//�����蔻�萶��
	if (m_pColision == nullptr)
	{
		m_pColision = new CColision();
	}

	//�ŏ��̃V�[���ݒ�
	if (m_pFade == nullptr)
	{
		m_pFade = CFade::Create(VEC3_RESET_ZERO);
	}

	SetMode(CScene::MODE::MODE_TITLE);

	return S_OK;
}

/**
 * @brief �I��
 */
void My::CManager::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Unload();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pModel != nullptr)
	{
		m_pModel->Unload();
		delete m_pModel;
		m_pModel = nullptr;
	}

	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		m_pLight = nullptr;
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	if (m_pColision != nullptr)
	{
		delete m_pColision;
		m_pColision = nullptr;
	}

	ClearCamera();

	//�L�[�{�[�h�̏I������
	m_pKeyboard->Uninit();
	//�}�E�X�̏I������
	m_pMouse->Uninit();
	//Pad�̏I������
	m_pPad->Uninit();

	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	CObject::ReleaseAll();
}

/**
 * @brief �X�V
 */
void My::CManager::Update()
{
	//�X�V����
	m_pRenderer->Update();

	for (auto& itr : m_pCamera)
	{
		if (itr == nullptr)
		{
			continue;
		}
		itr->Update();
	}

	m_pKeyboard->Update();
	m_pMouse->Update();
	m_pPad->Update();

	//�V�[���̍X�V
	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	CObject::UpdateAll();
}

/**
 * @brief �`��
 */
void My::CManager::Draw()
{
	//�`�揈��
	m_pRenderer->Draw();

	//�V�[���̕`��
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}
}

/**
 * @brief �V�[���ݒ�
 * @param [in]���̃V�[��
 */
void My::CManager::SetMode(CScene::MODE mode)
{
	//�V�[���I��
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//�V�[���؂�ւ�
	if (m_pScene == nullptr)
	{
		m_pScene = CScene::Create(mode);
	}
}


/**
 * @brief �����_���[�擾
 */
My::CRenderer* My::CManager::GetRenderer()
{
	return m_pRenderer;
}

/**
 * @brief �L�[�{�[�h�擾
 */
My::CInputKeyboard* My::CManager::GetKeyboard()
{
	return m_pKeyboard;
}

/**
 * @brief pad�擾
 */
My::CInputMouse* My::CManager::GetMouse()
{
	return m_pMouse;
}

/**
 * @brief �}�E�X�擾
 */
My::CInputPad* My::CManager::GetPad()
{
	return m_pPad;
}

/**
 * @brief �J�����擾
 */
My::CCamera* My::CManager::GetCamera(int idx)
{
	if (idx >= m_pCamera.size())
	{
		return nullptr;
	}
	return m_pCamera[idx];
}

/**
 * @brief �J�����̌�
 */
int My::CManager::GetCameraSize()
{
	return m_pCamera.size();
}

/**
 * @brief �f���J�����̔ԍ��擾
 */
int My::CManager::GetCureentViewCamera()
{
	return m_CurrentViewCamera;
}

/**
 * @brief �f���J�����̔ԍ��ݒ�
 */
void My::CManager::SetCureentViewCamera(int idx)
{
	if (idx >= m_pCamera.size())
	{
		assert(false && "�J�����̃T�C�Y�I�[�o�[");
	}

	m_CurrentViewCamera = idx;
}

/**
 * @brief �J�����S�폜
 */
void My::CManager::ClearCamera()
{
	for (auto& itr : m_pCamera)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		delete itr;
		itr = nullptr;
	}
	m_pCamera.clear();
}

/**
 * @brief �w�肳�ꂽ�J�����폜
 */
void My::CManager::EraseCamera(unsigned int idx)
{
	if (idx >= m_pCamera.size())
	{
		return;
	}
	CCamera* camera = m_pCamera[idx];
	camera->Uninit();
	delete camera;
	camera = nullptr;

	m_pCamera.erase(m_pCamera.begin() + idx);
}

/**
 * @brief ���C�g�擾
 */
My::CLight* My::CManager::GetLight()
{
	return m_pLight;
}

/**
 * @brief �e�N�X�`���擾
 */
My::CTexture* My::CManager::GetTexture()
{
	return m_pTexture;
}

/**
 * @brief ���f���擾
 */
My::CModel* My::CManager::GetModel()
{
	return m_pModel;
}

/**
 * @brief �t�F�[�h�擾
 */
My::CFade* My::CManager::GetFade()
{
	return m_pFade;
}

/**
 * @brief �R���W�����擾
 */
My::CColision* My::CManager::GetColision()
{
	return m_pColision;
}
