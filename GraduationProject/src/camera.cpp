//=============================================
//
//�J����[camera.cpp]
//Author Matsuda Towa
//
//=============================================
#include "camera.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"

/**
 * @brief �R���X�g���N�^
 */
My::CCamera::CCamera():
m_fAngle(FLOAT_ZERO),		//�p�x
m_fLength(FLOAT_ZERO),		//����
m_moveR(VEC3_RESET_ZERO),	//�����_�̈ړ���
m_moveV(VEC3_RESET_ZERO),	//���_�̈ړ���
m_mtxProjection(),			//�v���W�F�N�V�����s��
m_mtxView(),				//�r���[�s��
m_pCameraState(),			//�J�����X�e�[�g
m_posR(VEC3_RESET_ZERO),	//�����_
m_posV(VEC3_RESET_ZERO),	//���_
m_rot(VEC3_RESET_ZERO),		//����
m_rotmove(VEC3_RESET_ZERO),	//�����̈ړ���
m_vecU(VEC3_RESET_ZERO),	//������x�N�g��
m_ViewPort()				//�r���[�|�[�g������
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CCamera::~CCamera()
{
}

/**
 * @brief ������
 * @return ����������
 */
HRESULT My::CCamera::Init()
{
	if (m_pCameraState == nullptr)
	{
		m_pCameraState = new CBirdView;
	}


	m_posV = D3DXVECTOR3(VEC3_RESET_ZERO);		//���_
	m_posR = D3DXVECTOR3(VEC3_RESET_ZERO);		 //����

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��

	m_moveV = D3DXVECTOR3(VEC3_RESET_ZERO);		//���_�ړ���
	m_moveR = D3DXVECTOR3(VEC3_RESET_ZERO);		//�����_�ړ���

	m_rot = D3DXVECTOR3(VEC3_RESET_ZERO);		//�p�x
	m_rotmove = D3DXVECTOR3(VEC3_RESET_ZERO);	//��]��

	D3DXVECTOR3 vecCamera = m_posR - m_posV;
	m_fLength = sqrtf(vecCamera.y * vecCamera.y + vecCamera.z * vecCamera.z);

	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(vecCamera.y, vecCamera.z);

	m_ViewPort.Width = SCREEN_WIDTH;
	m_ViewPort.Height = SCREEN_HEIGHT;

	m_ViewPort.X = 0;
	m_ViewPort.Y = 0;

	m_ViewPort.MinZ = 0.0f;
	m_ViewPort.MaxZ = 1.0f;

	GET_DEVICE->SetViewport(&m_ViewPort);

	//���R���񏈗�
	if (m_pCameraState != nullptr)
	{
		m_pCameraState->BirdView(this);
	}
	//���_�ʒu�Z�o
	m_posV = m_posR + D3DXVECTOR3(-m_fLength * cosf(m_rot.x) * sinf(m_rot.y),
		m_fLength * sinf(m_rot.x),
		-m_fLength * cosf(m_rot.x) * cosf(m_rot.y));

	SetCamera();

	return S_OK;
}

/**
 * @brief �I��
 */
void My::CCamera::Uninit()
{
	//�X�e�[�g�|�C���^�폜
	if (m_pCameraState != nullptr)
	{
		delete m_pCameraState;
		m_pCameraState = nullptr;
	}

}

/**
 * @brief �X�V
 */
void My::CCamera::Update()
{
	//���R���񏈗�
	if (m_pCameraState != nullptr)
	{
		m_pCameraState->BirdView(this);
	}

	m_ViewPort.Width = SCREEN_WIDTH;
	m_ViewPort.Height = SCREEN_HEIGHT;

	m_ViewPort.X = 0;
	m_ViewPort.Y = 0;

	m_ViewPort.MinZ = 0.0f;
	m_ViewPort.MaxZ = 1.0f;

	GET_DEVICE->SetViewport(&m_ViewPort);

	//���_�ʒu�Z�o
	m_posV = m_posR + D3DXVECTOR3(-m_fLength * cosf(m_rot.x) * sinf(m_rot.y),
		m_fLength * sinf(m_rot.x),
		-m_fLength * cosf(m_rot.x) * cosf(m_rot.y));

	//��������
	if (m_rot.x <= MIN_TURN_X)
	{
		m_rot.x = MIN_TURN_X;
	}
	if (m_rot.x >= MAX_TURN_X)
	{
		m_rot.x = MAX_TURN_X;
	}

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y = -D3DX_PI;
	}

	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}

	if (m_rot.x > D3DX_PI)
	{
		m_rot.x = -D3DX_PI;
	}

	if (m_rot.x < -D3DX_PI)
	{
		m_rot.x = D3DX_PI;
	}

	m_posV += m_moveV;
	m_posR += m_moveR;

	//�ړ��ʂ��X�V(�����j
	m_moveV.x += (FLOAT_ZERO - m_moveV.x) * DAMPING_COEFFICIENT;
	m_moveV.y += (FLOAT_ZERO - m_moveV.y) * DAMPING_COEFFICIENT;
	m_moveV.z += (FLOAT_ZERO - m_moveV.z) * DAMPING_COEFFICIENT;

	m_moveR.x += (FLOAT_ZERO - m_moveR.x) * DAMPING_COEFFICIENT;
	m_moveR.y += (FLOAT_ZERO - m_moveR.y) * DAMPING_COEFFICIENT;
	m_moveR.z += (FLOAT_ZERO - m_moveR.z) * DAMPING_COEFFICIENT;

}

/**
 * @brief �J�����̐ݒ�
 */
void My::CCamera::SetCamera()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		3000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	D3DXVECTOR3 posV = m_posV;
	D3DXVECTOR3 posR = m_posR;

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&posV,
		&posR,
		&m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

/**
 * @brief �J�����̃��Z�b�g
 */
void My::CCamera::ResetCamera()
{
	m_posV = D3DXVECTOR3(VEC3_RESET_ZERO); //���_
	m_posR = D3DXVECTOR3(VEC3_RESET_ZERO); //����

	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f); //������x�N�g��

	m_moveV = D3DXVECTOR3(VEC3_RESET_ZERO); //���_�ړ���
	m_moveR = D3DXVECTOR3(VEC3_RESET_ZERO); //�����_�ړ���

	m_rot = D3DXVECTOR3(VEC3_RESET_ZERO); //�p�x
	m_rotmove = D3DXVECTOR3(VEC3_RESET_ZERO); //��]��

	D3DXVECTOR3 vecCamera = m_posR - m_posV;
	m_fLength = sqrtf(vecCamera.y * vecCamera.y + vecCamera.z * vecCamera.z);

	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(vecCamera.y, vecCamera.z);
}

/**
 * @brief �J�����̈ړ�
 */
void My::CCamera::InputMove()
{
	//�L�[�{�[�h���擾
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetKeyboard();

	if (pKeyboard->GetPress(DIK_J) == true)
	{
		m_moveV.x -= sinf(HALF_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.x -= sinf(HALF_PI + m_rot.y) * DEFAULT_MOVE;

		m_moveV.z -= cosf(HALF_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.z -= cosf(HALF_PI + m_rot.y) * DEFAULT_MOVE;

	}

	if (pKeyboard->GetPress(DIK_L) == true)
	{
		m_moveV.x += sinf(HALF_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.x += sinf(HALF_PI + m_rot.y) * DEFAULT_MOVE;

		m_moveV.z += cosf(HALF_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.z += cosf(HALF_PI + m_rot.y) * DEFAULT_MOVE;

	}

	if (pKeyboard->GetPress(DIK_I) == true)
	{
		m_moveV.x -= sinf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.x -= sinf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;

		m_moveV.z -= cosf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.z -= cosf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;

	}

	if (pKeyboard->GetPress(DIK_K) == true)
	{
		m_moveV.x += sinf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.x += sinf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;

		m_moveV.z += cosf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;
		m_moveR.z += cosf(D3DX_PI + m_rot.y) * DEFAULT_MOVE;
	}


	if (pKeyboard->GetPress(DIK_SPACE))
	{
		m_moveV.y += 1.0f;
		m_moveR.y += 1.0f;
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		m_moveV.y -= 1.0f;
		m_moveR.y -= 1.0f;
	}
}

/**
 * @brief �J�����̃X�e�[�g�ύX
 * @param [in]���̃X�e�[�g�N���X
 */
void My::CCamera::ChangeCameraState(CCameraState* state)
{
	//���̃X�e�[�g�����������̃X�e�[�g�ɐ؂�ւ���
	if (m_pCameraState != nullptr)
	{
		delete m_pCameraState;
		m_pCameraState = state;
	}
	if (m_pCameraState == nullptr)
	{
		delete state;
	}
}

/**
 * @brief ��]
 */
void My::CCamera::InputTurn()
{
	//�L�[�{�[�h���擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	if (pKeyboard->GetPress(DIK_U) == true)
	{
		m_rot.y += TURN_SPEED;

		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;

		m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
	}

	if (pKeyboard->GetPress(DIK_O) == true)
	{
		m_rot.y -= TURN_SPEED;

		m_posV.x = m_posR.x - sinf(m_rot.y) * m_fLength;

		m_posV.z = m_posR.z - cosf(m_rot.y) * m_fLength;
	}

	//�}�E�X
	MouseTurn();
}

/**
 * @brief �}�E�X�ɂ���]
 */void My::CCamera::MouseTurn()
{
	//�}�E�X���擾
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	m_rot.y += pMouse->GetMouseMove().x * 0.001f;
	m_rot.x += pMouse->GetMouseMove().y * 0.001f;
	m_posR.y += pMouse->GetMouseMove().z * 0.1f;
}

/**
 * @brief �J�����̃f�o�b�O�\��
 */
void My::CCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n[camera]\nposR:%.1f,%.1f,%.1f\nposV:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\nlength:%.1f"
		, m_posR.x, m_posR.y, m_posR.z, m_posV.x, m_posV.y, m_posV.z, m_rot.x, m_rot.y, m_rot.z,m_fLength);
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}