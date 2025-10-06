//=============================================
//
//�Q�[���̃X�e�[�g�p�^�[��[game_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_state.h"
#include "game.h"
#include "game_manager.h"

//=============================================
// �ʏ�
//=============================================
void My::CNormal::Normal(CGame* game)
{
	//�I�u�W�F�N�g�̃A�b�v�f�[�g��������
	game->StopObject(false);

	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	////�|�[�Y�ڍs
	//if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	//{
	//	game->ResetPauseCnt();
	//	CGameManager::GetInstance()->ChangeState(new CPause);
	//	return;
	//}
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

	//if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	//{
	//	game->ResetPauseCnt();
	//	CGameManager::GetInstance()->ChangeState(new CNormal);
	//	return;
	//}
}
