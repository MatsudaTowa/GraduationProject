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
	if (GetState() == nullptr)
	{
		ChangeState(new CLobbyState);
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
	//�e�N���X�̏I������
	CPlayer::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CGamePlayer::Update()
{
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