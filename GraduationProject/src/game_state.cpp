//=============================================
//
//ゲームのステートパターン[game_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_state.h"
#include "game.h"
#include "game_manager.h"

//=============================================
// 通常
//=============================================
void My::CNormal::Normal(CGame* game)
{
	//オブジェクトのアップデートを許可する
	game->StopObject(false);

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	////ポーズ移行
	//if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	//{
	//	game->ResetPauseCnt();
	//	CGameManager::GetInstance()->ChangeState(new CPause);
	//	return;
	//}
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

	//if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	//{
	//	game->ResetPauseCnt();
	//	CGameManager::GetInstance()->ChangeState(new CNormal);
	//	return;
	//}
}
