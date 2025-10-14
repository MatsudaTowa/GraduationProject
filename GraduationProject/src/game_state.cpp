//=============================================
//
//ゲームのステートパターン[game_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_state.h"
#include "game.h"
#include "active_manager.h"

//=============================================
// 通常
//=============================================
void My::CNormal::Normal(CGame* game)
{
	//オブジェクトのアップデートを許可する
	game->StopObject(false);

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
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
		CGameManager::GetInstance()->ChangeState(new CNormal);
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
		CGameManager::GetInstance()->ChangeState(new CNormal);
	}
#endif
}
