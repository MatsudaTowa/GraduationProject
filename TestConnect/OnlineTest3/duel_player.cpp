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
	m_ZoneManager(nullptr),		//ゾーンマネージャー
	m_ElapsedTime(0)			//経過時間
{
	//リストに追加
	My::CDuel_Player_Manager::GetInstance()->AddList(this);

	//ゾーンマネージャーの作成
	m_ZoneManager = new CZoneManager;
	m_ZoneManager->Init();
}

//=====================================
//エナジーの更新
//=====================================
bool My::CDuel_Player::UpdateEnergy(int deltatime)
{
	//デルタタイムの追加
	m_ElapsedTime += deltatime;

	//エナジーが更新されないなら返す
	if (m_ElapsedTime <= ENERGY_UP_TIME) return false;

	//エナジーの更新時間を超えているなら繰り返す
	while (m_ElapsedTime > ENERGY_UP_TIME)
	{
		//現在の経過時間からエネルギーのカウントアップに必要な時間を引く
		m_ElapsedTime -= ENERGY_UP_TIME;

		//エナジーを増やす
		m_Status.energy++;

		//ログ
		std::cout << "プレイヤー" << GetIndex() << "のエナジー : " << GetStatus().energy << "\n";
	}

	return true;
}
