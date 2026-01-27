//================================
//
//duel_manager.cppに必要な宣言[duel_manager.cpp]
//Author:松田永久
// 
//================================
#include "duel_manager.h"

//==============================================
//初期化
//==============================================
void My::CDuel_Manager::Init()
{
	//対戦の合図を初期化
	m_isStartBattle - false;

	//タイマーのリセット
	m_DuelTimer.Reset();

	//順位
	m_RankMap.clear();
}
