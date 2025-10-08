//=============================================
//
//�Q�[���}�l�[�W���[[gamemaneger.cpp]
//Auther MatsudaTowa
//
//=============================================
#include "game_manager.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CGameManager::CGameManager():
m_pState(nullptr),			//�Q�[���̃X�e�[�g
m_pEnemyManager(),			//�G�l�~�[�}�l�[�W���[
m_pPlayer(nullptr),			//�v���C���[�̃|�C���^
m_pField(nullptr),			//���̃|�C���^
m_isFinish(false)
{
	//�G�l�~�[�}�l�[�W���[�̐���
	m_pEnemyManager = new CEnemyManager();		//�������m��
	m_pEnemyManager->Init();					//�����ݒ�
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CGameManager::~CGameManager()
{
	//�G�l�~�[�}�l�[�W���[�̔j��
	if (m_pEnemyManager != nullptr)
	{
		//�I��������ɊJ��
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}
}

//=============================================
// �I��
//=============================================
void My::CGameManager::Uninit()
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] == nullptr) { continue; }
		m_pArea[i]->Uninit();
		m_pArea[i] = nullptr;
	}
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}
}

//=============================================
//�X�e�[�g�ύX
//=============================================
void My::CGameManager::ChangeState(CGameState* state)
{
	CGameState* current_state = CGameManager::GetInstance()->GetState();
	//���̃X�e�[�g�����������̃X�e�[�g�ɐ؂�ւ���
	if (current_state != nullptr)
	{
		if (typeid(*current_state) == typeid(*state))
		{//���݂Ɠ�����������֐��𔲂���
			return;
		}
		delete current_state;

		current_state = state;
		CGameManager::GetInstance()->SetState(current_state);
	}
}

//=============================================
//�G���A����
//=============================================
void My::CGameManager::CreateArea()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] != nullptr) { continue; }

		//�O�p�`�̒��_���W���w��
		D3DXVECTOR2 triangle_vtx[CObject2D_Triangle::NUM_VERTEX];

		//�E�B���h�E�̒��S�ɂ�K�����_��
		D3DXVECTOR2 center = { SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF };
		triangle_vtx[1] = center;

		//���ꂼ��̒��_�ʒu���w��
		switch (i)
		{
		case CInputMouse::AREA::UP:
			triangle_vtx[0] = { SCREEN_WIDTH,FLOAT_ZERO };
			triangle_vtx[2] = { VEC2_RESET_ZERO };
			break;
		case CInputMouse::AREA::DOWN:
			triangle_vtx[0] = { FLOAT_ZERO,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::LEFT:
			triangle_vtx[0] = { FLOAT_ZERO,FLOAT_ZERO };
			triangle_vtx[2] = { FLOAT_ZERO,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::RIGHT:
			triangle_vtx[0] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,FLOAT_ZERO };
			break;
		default:
			break;
		}

		m_pArea[i] = CArea::Create(triangle_vtx);
	}
}

//=============================================
//�G���A�I��
//=============================================
void My::CGameManager::SelectArea()
{
	CInputMouse::AREA area = GET_INPUT_MOUSE->GetArea();

	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		m_pArea[i]->SetSelect(false);
		if (area == CInputMouse::CENTER) { continue; }

		//�I������Ă���Ƃ���͖��邭
		m_pArea[area]->SetSelect(true);
	}
}
