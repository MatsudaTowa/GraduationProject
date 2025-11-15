//=============================================
//
//エネミーのステートパターン[enemy_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy_state.h"
#include "active_scene_manager.h"
#include "enemy.h"

//=============================================
// コンストラクタ
//=============================================
My::CEnemyLobbyState::CEnemyLobbyState(CActiveSceneCharacter* character):CLobbyCharacter(character)
{
}

//=============================================
// デストラクタ
//=============================================
My::CEnemyLobbyState::~CEnemyLobbyState()
{
}

//=============================================
// ロビー処理
//=============================================
void My::CEnemyLobbyState::Lobby(CActiveSceneCharacter* character)
{
	CLobbyCharacter::Lobby(character);
	if (typeid(*character) != typeid(CEnemy))
	{
		return;
	}
	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
	//モーション設定
	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
}

//=============================================
// コンストラクタ
//=============================================
My::CEnemyDuelState::CEnemyDuelState(CActiveSceneCharacter* character) : CDuelCharacter(character)
{

}

//=============================================
// デストラクタ
//=============================================
My::CEnemyDuelState::~CEnemyDuelState()
{
}

//=============================================
// デュエル処理
//=============================================
void My::CEnemyDuelState::Duel(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CEnemy))
	{
		return;
	}
	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
	//UIが生成されてなければ生成される
	CreateDuelUI(enemy);
	//モーション設定
	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
}

//=============================================
// UI設定
//=============================================
void My::CEnemyDuelState::CreateDuelUI(CEnemy* enemy)
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), enemy->GetPos()); //スクリーン座標に変換

	if (enemy->GetPlayerUI()->GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		enemy->GetPlayerUI()->SetLifeUI(pLifeUI);
	}

	if (enemy->GetPlayerUI()->GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
		enemy->GetPlayerUI()->SetEnergyUI(pEnergyUI);
	}
}
