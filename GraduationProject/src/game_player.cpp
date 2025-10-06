//=============================================
//
//�Q�[���v���C���[[game_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player.h"
#include "game_manager.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CGamePlayer::CGamePlayer(int nPriority):CPlayer(nPriority),
m_pEnergyUpCount(nullptr),										//�G�i�W�[�v���J�E���g
m_pState(nullptr)												//�X�e�[�g������
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CGamePlayer::~CGamePlayer()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CGamePlayer::Init()
{
	if (m_pState == nullptr)
	{
		m_pState = new CNeutralState;
	}

	if (GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create({600.0f,600.0f,0.0f});
		SetLifeUI(pLifeUI);
	}

	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
	}

	//�e�N���X�̏��������s
	CPlayer::Init();

	//�Q�[���̃}�l�[�W���Ɏ�������
	CGameManager::GetInstance()->SetPlayer(this);
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CGamePlayer::Uninit()
{
	if (m_pEnergyUpCount != nullptr)
	{
		delete m_pEnergyUpCount;
		m_pEnergyUpCount = nullptr;
	}
	//�e�N���X�̏I������
	CPlayer::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CGamePlayer::Update()
{
	if (m_pState != nullptr)
	{
		m_pState->Neutral(this);
	}

#ifdef _DEBUG
	int life = GetLife();
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_L))
	{
		++life;
	}
	else if (GET_INPUT_KEYBOARD->GetTrigger(DIK_K))
	{
		--life;
	}
	SetLife(life);
#endif // _DEBUG


	EnergyUp();

	//�e�N���X�̍X�V
	CPlayer::Update();
}

//=============================================
// �`��
//=============================================
void My::CGamePlayer::Draw()
{
	//�e�N���X�̕`��
	CPlayer::Draw();
}

//=============================================
// �X�e�[�g�ύX
//=============================================
void My::CGamePlayer::ChangeState(CGamePlayerState* state)
{
	//���̃X�e�[�g�����������̃X�e�[�g�ɐ؂�ւ���
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = state;
	}
}

//=============================================
//�G�i�W�[�グ�鏈��
//=============================================
void My::CGamePlayer::EnergyUp()
{
	int energy = GetEnergy();

#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_1))
	{
		energy += 10;
		SetEnergy(energy);
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
	SetEnergy(energy);
	m_pEnergyUpCount->SetCnt(INT_ZERO);
}

//=============================================
// ����
//=============================================
void My::CGamePlayer::Input()
{
}

//=============================================
//�v���C���[�̃f�o�b�O�\��
//=============================================
void My::CGamePlayer::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = CManager::GetInstance()->GetRenderer()->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\n�G�i�W�[:%d\n�̗�:%d K�Ō��� L�ő���"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z,GetEnergy(),GetLife());
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}