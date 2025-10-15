//=============================================
//
//�Q�[���v���C���[[game_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player.h"
#include "active_manager.h"
#include "hand.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CGamePlayer::CGamePlayer(int nPriority):CPlayer(nPriority),
m_pState(nullptr),												//�X�e�[�g������
m_pHand(nullptr)												// ��D������
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
		m_pState = new CLobbyState;
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
	if (m_pHand != nullptr)
	{
		delete m_pHand;
		m_pHand = nullptr;
	}
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
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
		m_pState->Lobby(this);

		m_pState->Duel(this);
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
	else if (m_pState == nullptr)
	{
		delete state;
	}
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