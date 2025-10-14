//=============================================
//
//�Q�[���̃X�e�[�g�p�^�[��[game_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_state.h"
#include "game.h"
#include "active_manager.h"

//=============================================
// �ʏ�
//=============================================
void My::CNormal::Normal(CGame* game)
{
	//�I�u�W�F�N�g�̃A�b�v�f�[�g��������
	game->StopObject(false);

	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();

		CGameManager::GetInstance()->ChangeState(new CCardCast);
	}
#endif


	//�|�[�Y�ڍs
	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CPause);
		return;
	}
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CPause::~CPause()
{
}

//=============================================
// �|�[�Y
//=============================================
void My::CPause::Pause(CGame* game)
{
	//�I�u�W�F�N�g�̃A�b�v�f�[�g���~�߂�
	game->StopObject(true);

	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CNormal);
		return;
	}
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CCardCast::CCardCast()
{
	CGameManager::GetInstance()->CreateArea();
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CCardCast::~CCardCast()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		CArea* area = CGameManager::GetInstance()->GetArea(i);
		if (area == nullptr) { continue; }
		area->Uninit();
		area = nullptr;
		CGameManager::GetInstance()->SetArea(area, i);
	}
}

//=============================================
// �J�[�h�L���X�g
//=============================================
void My::CCardCast::CardCast(CGame* game)
{
	CGameManager::GetInstance()->SelectArea();
#ifdef _DEBUG
	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CNormal);
	}
#endif
}
