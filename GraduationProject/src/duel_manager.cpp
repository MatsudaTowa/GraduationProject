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
	//タイマーのリセット
	m_DuelTimer.Reset();

	//順位
	m_RankMap.clear();
}
