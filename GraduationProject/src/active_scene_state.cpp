//=============================================
//
//�Q�[���̃X�e�[�g�p�^�[��[active_scene_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_state.h"
#include "game.h"
#include "active_manager.h"
#include "game_field.h"
#include "energy_charge.h"
#include "energy_gauge.h"

//=============================================
// ���r�[
//=============================================
void My::CLobby::Lobby(CGame* game)
{
	CGameManager::GetInstance()->GetEnemyManager()->GetList().size();

	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_RETURN) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->GetPlayer()->ChangeState(new CDuelState);
		//�n�ʐ���
		CField::Create(VEC3_RESET_ZERO, { FIELD_SIZE,0.0f,FIELD_SIZE }, new CGameField);

		//�G�i�W�[UI�g�\��
		CEnergy_Gauge::CreateEnergy();

		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CDuel::CDuel()
{

}

//=============================================
// �f�X�g���N�^
//=============================================
My::CDuel::~CDuel()
{
}

//=============================================
// �f���G��
//=============================================
void My::CDuel::Duel(CGame* game)
{
	//�I�u�W�F�N�g�̃A�b�v�f�[�g��������
	game->StopObject(false);

	//�Q�[�W�p�`���[�W�̍X�V
	CEnergy_Charge* pCharge = CEnergy_Charge::GetInstance();
	pCharge->Update();

	//���̓f�o�C�X�擾
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_RETURN) && game->GetPauseKey())
	{
		GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
	}
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
		CGameManager::GetInstance()->ChangeState(new CDuel);
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
		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
#endif
}

