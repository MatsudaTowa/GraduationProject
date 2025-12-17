////=============================================
////
////コンピューターのステート[cpu_state.cpp]
////Author 島津 湧人
////
////=============================================
//#include "cpu_state.h"
//
////=============================================
//// コンストラクタ
////=============================================
//My::CCpuLobbyState::CCpuLobbyState(CActiveSceneCharacter* character) :CEnemyLobbyState(character)
//{
//}
//
////=============================================
//// デストラクタ
////=============================================
//My::CCpuLobbyState::~CCpuLobbyState()
//{
//}
//
////=============================================
//// ロビー処理
////=============================================
//void My::CCpuLobbyState::Lobby(CActiveSceneCharacter* character)
//{
//	CLobbyCharacter::Lobby(character);
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
//My::CCpuDuelState::CCpuDuelState(CActiveSceneCharacter* character) : CEnemyDuelState(character)
//{
//
//}
//
////=============================================
//// デストラクタ
////=============================================
//My::CCpuDuelState::~CCpuDuelState()
//{
//}
//
////=============================================
//// デュエル処理
////=============================================
//void My::CCpuDuelState::Duel(CActiveSceneCharacter* character)
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
//
//	//親の更新
//	CDuelCharacter::Duel(character);
//}