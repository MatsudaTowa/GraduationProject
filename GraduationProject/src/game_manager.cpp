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