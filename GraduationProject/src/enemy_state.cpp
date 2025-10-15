////=============================================
////
////エネミーのステートパターン[enemy_state.cpp]
////Author Matsuda Towa
////
////=============================================
//#include "enemy_state.h"
//#include "active_manager.h"
//#include "enemy.h"
//
////=============================================
//// コンストラクタ
////=============================================
//My::CLobbyState::CLobbyState()
//{
//}
//
////=============================================
//// デストラクタ
////=============================================
//My::CLobbyState::~CLobbyState()
//{
//}
//
////=============================================
//// ロビー処理
////=============================================
//void My::CLobbyState::Lobby(CActiveSceneCharacter* character)
//{
//	if (typeid(*character) != typeid(CEnemy))
//	{
//		return;
//	}
//	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
//	//モーション設定
//	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
//}
//
////=============================================
//// コンストラクタ
////=============================================
//My::CDuelState::CDuelState()
//{
//}
//
////=============================================
//// デストラクタ
////=============================================
//My::CDuelState::~CDuelState()
//{
//}
//
////=============================================
//// デュエル処理
////=============================================
//void My::CDuelState::Duel(CActiveSceneCharacter* character)
//{
//	if (typeid(*character) != typeid(CEnemy))
//	{
//		return;
//	}
//	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
//	//UIが生成されてなければ生成される
//	CreateDuelUI(enemy);
//	//モーション設定
//	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
//}
//
////=============================================
//// UI設定
////=============================================
//void My::CDuelState::CreateDuelUI(CEnemy* enemy)
//{
//	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), enemy->GetPos()); //スクリーン座標に変換
//
//	if (enemy->GetLifeUI() == nullptr)
//	{
//		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
//		enemy->SetLifeUI(pLifeUI);
//	}
//
//	if (enemy->GetEnergyUI() == nullptr)
//	{
//		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
//		enemy->SetEnergyUI(pEnergyUI);
//	}
//}
