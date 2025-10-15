//=============================================
//
//ゲームのステートパターン[active_scene_state.cpp]
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
// ロビー
//=============================================
void My::CLobby::Lobby(CGame* game)
{
	CGameManager::GetInstance()->GetEnemyManager()->GetList().size();

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_RETURN) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->GetPlayer()->ChangeState(new CDuelState);
		//地面生成
		CField::Create(VEC3_RESET_ZERO, { FIELD_SIZE,0.0f,FIELD_SIZE }, new CGameField);

		//エナジーUI枠表示
		CEnergy_Gauge::CreateEnergy();

		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
}

//=============================================
// コンストラクタ
//=============================================
My::CDuel::CDuel()
{

}

//=============================================
// デストラクタ
//=============================================
My::CDuel::~CDuel()
{
}

//=============================================
// デュエル
//=============================================
void My::CDuel::Duel(CGame* game)
{
	//オブジェクトのアップデートを許可する
	game->StopObject(false);

	//ゲージ用チャージの更新
	CEnergy_Charge* pCharge = CEnergy_Charge::GetInstance();
	pCharge->Update();

	//入力デバイス取得
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

	//ポーズ移行
	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CPause);
		return;
	}
}

//=============================================
// デストラクタ
//=============================================
My::CPause::~CPause()
{
}

//=============================================
// ポーズ
//=============================================
void My::CPause::Pause(CGame* game)
{
	//オブジェクトのアップデートを止める
	game->StopObject(true);

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CDuel);
		return;
	}
}

//=============================================
// コンストラクタ
//=============================================
My::CCardCast::CCardCast()
{
	CGameManager::GetInstance()->CreateArea();
}

//=============================================
// デストラクタ
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
// カードキャスト
//=============================================
void My::CCardCast::CardCast(CGame* game)
{
	CGameManager::GetInstance()->SelectArea();
#ifdef _DEBUG
	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
#endif
}

