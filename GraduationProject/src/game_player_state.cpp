//=============================================
//
//ゲームプレイヤーのステートパターン[game_player_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player_state.h"
#include "game_player.h"
#include "active_manager.h"
#include "field.h"

//=============================================
// ニュートラル処理
//=============================================
void My::CNeutralState::Neutral(CGamePlayer* player)
{
	//モーション設定
	player->SetMotion(CPlayer::MOTION_NEUTRAL);
}