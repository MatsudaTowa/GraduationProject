//=============================================
//
//�v���C���[[result_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "result_player.h"

namespace
{
	const float BELT_TARGET_POS_Y = 80.0f;
	const float BELT_TARGET_POS_Z = 30.0f;
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CResultPlayer::CResultPlayer(int nPriority):CPlayer(nPriority)
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CResultPlayer::~CResultPlayer()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CResultPlayer::Init()
{
	//�e�N���X�̏��������s
	CPlayer::Init();
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CResultPlayer::Uninit()
{
	//�e�N���X�̏I������
	CPlayer::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CResultPlayer::Update()
{
	CPlayer::Update();
}

//=============================================
// �`��
//=============================================
void My::CResultPlayer::Draw()
{
	//�e�N���X�̕`��
	CPlayer::Draw();
}
