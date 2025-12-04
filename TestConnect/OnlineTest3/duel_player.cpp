//================================
//
// 送受信データクラス用のヘッダー[taknet_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "duel_player.h"
#include "duel_player_manager.h"

//=====================================
//コンストラクタ
//=====================================
My::CDuel_Player::CDuel_Player() : 
	m_ZoneManager(nullptr)		//ゾーンマネージャー
{
	My::CDuel_Player_Manager::GetInstance()->AddList(this);
}