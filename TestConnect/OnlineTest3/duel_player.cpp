//================================
//
// 送受信データクラス用のヘッダー[taknet_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "duel_player.h"
#include "duel_player_manager.h"
#include "card.h"
#include "duel_manager.h"

//=====================================
//コンストラクタ
//=====================================
My::CDuel_Player::CDuel_Player() : 
	m_ZoneManager(nullptr),		//ゾーンマネージャー
	m_ElapsedTime(0),			//経過時間
	m_isDeath(false)			//死亡フラグ
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
	//更新を行うか
	if (!CDuel_Manager::GetInstance()->GetIsStartBattle()) return false;
	if (m_Status.life <= 0) return false;
	
	//エナジーが最大なら更新しない
	if (m_Status.energy >= 99)
	{
		return false;
	}

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

//=====================================
//カードのドロー
//=====================================
bool My::CDuel_Player::DrawCard()
{
	//対象者のエナジーがあるならカードを引く
	if (GetStatus().energy <= 0)
	{
		return false;
	}

	//山札切れになるかを確認
	if (m_ZoneManager->GetDeck()->CheckDeckOut())
	{
		return false;
	}

	//最大手札枚数ではないか
	if (m_ZoneManager->GetHandZone()->GetList().size() >= MAX_HAND_CARD)
	{
		return false;
	}

	//エナジーを消費
	GetStatus().energy--;

	//カードの移動
	CCard* pCard = m_ZoneManager->GetDeck()->GetTopCard();
	pCard->SetCurrentZone(CCard::ZONE::HAND);
	m_ZoneManager->MoveZone(pCard, m_ZoneManager->GetDeck(), m_ZoneManager->GetHandZone(), false);

	//ステータスに反映
	GetStatus().deckSize = m_ZoneManager->GetDeck()->GetList().size();	//デッキ枚数
	GetStatus().hand++;													//手札枚数

	 // コンソールの出力コードページを UTF-8 に変更
	UINT CP = GetConsoleOutputCP();
	
	//const char* log = pCard->GetBaseStatus().Name.c_str();

	//引いたカードの確認用
	std::cout << "引いたカード : ";
	SetConsoleOutputCP(CP_UTF8);
	std::cout << pCard->GetBaseStatus().Name << "\n";
	SetConsoleOutputCP(CP);

	return true;
}

//=====================================
//ダメージを受ける処理
//=====================================
void My::CDuel_Player::ReceiveDamage(int damage)
{
	m_Status.life -= damage;

	if (m_Status.life < 0)
	{
		m_Status.life = 0;
	}
}
