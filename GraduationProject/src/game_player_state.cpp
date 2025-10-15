//=============================================
//
//�Q�[���v���C���[�̃X�e�[�g�p�^�[��[game_player_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player_state.h"
#include "game_player.h"
#include "active_manager.h"
#include "field.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CLobbyState::CLobbyState()
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CLobbyState::~CLobbyState()
{
}

//=============================================
// ���r�[����
//=============================================
void My::CLobbyState::Lobby(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CGamePlayer))
	{
		return;
	}
	CGamePlayer* player = dynamic_cast<CGamePlayer*>(character);
	//���[�V�����ݒ�
	player->SetMotion(CPlayer::MOTION_NEUTRAL);
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CDuelState::CDuelState():m_pEnergyUpCount(nullptr)
{
	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
	}
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CDuelState::~CDuelState()
{
	if (m_pEnergyUpCount != nullptr)
	{
		delete m_pEnergyUpCount;
		m_pEnergyUpCount = nullptr;
	}
}

//=============================================
// �f���G������
//=============================================
void My::CDuelState::Duel(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CGamePlayer))
	{
		return;
	}
	CGamePlayer* player = dynamic_cast<CGamePlayer*>(character);
	//UI����������ĂȂ���ΐ��������
	CreateDuelUI(player);
	//���[�V�����ݒ�
	player->SetMotion(CPlayer::MOTION_NEUTRAL);

	/*
		* @brief �J�n
		* TODO : �����������ɂ����Ă���B�̂��ɃQ�[���J�n���ɌĂяo��
		*/

	CHand* pHand = player->GetHand();
	if (pHand != nullptr)
	{
		pHand->Start();

		pHand->Update();
	}

	EnergyUp(player);
}

//=============================================
// UI�ݒ�
//=============================================
void My::CDuelState::CreateDuelUI(CGamePlayer* player)
{
	if (player->GetHand() == nullptr)
	{// ��D����
		player->SetHand(CHand::Create());
	}

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), player->GetPos()); //�X�N���[�����W�ɕϊ�

	if (player->GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		player->SetLifeUI(pLifeUI);
	}

	if (player->GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
		player->SetEnergyUI(pEnergyUI);
	}
}


//=============================================
//�G�i�W�[�グ�鏈��
//=============================================
void My::CDuelState::EnergyUp(CGamePlayer* player)
{
	int energy = player->GetEnergy();

#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_1))
	{
		energy += 10;
		player->SetEnergy(energy);
	}
#endif // _DEBUG

	if (m_pEnergyUpCount == nullptr)
	{
		return;
	}

	if (!m_pEnergyUpCount->CountUp())
	{
		return;
	}

	//�G�i�W�[����
	++energy;
	player->SetEnergy(energy);
	m_pEnergyUpCount->SetCnt(INT_ZERO);
}
