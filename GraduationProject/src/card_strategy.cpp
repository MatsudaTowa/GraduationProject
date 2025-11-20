//===============================================================================
//
//カードストラテジー[active_scene_character_state.cpp]
//Author Matsuda Towa
//
//===============================================================================
#include "card_strategy.h"
#include "active_scene_manager.h"
#include "raknet.h"

//===============================================================================
// コンストラクタ
//===============================================================================
My::CAttack::CAttack()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CAttack::~CAttack()
{
}

//===============================================================================
// 攻撃処理
//===============================================================================
void My::CAttack::Strategy(CDuelCharacter* /*duel*/, CCard* card)
{
	//登録されているキャラクターを取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	int life;
	
	//リスト周回
	for (auto& itr : List)
	{
		if (itr == nullptr) { continue; }

		if (itr->GetArea() != card->GetTarget()) { continue; }

		life = itr->GetLife();
		if (life > INT_ZERO)
		{//TODO:選択できない旨のUI表示
			--life;
		}
		itr->SetLife(life);
	}

	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}
}

//===============================================================================
// コンストラクタ
//===============================================================================
My::CDiffence::CDiffence()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CDiffence::~CDiffence()
{
}

//===============================================================================
// 守備処理
//===============================================================================
void My::CDiffence::Strategy(CDuelCharacter* /*duel*/, CCard* /*card*/)
{
}

//===============================================================================
// コンストラクタ
//===============================================================================
My::CHeal::CHeal()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CHeal::~CHeal()
{
}

//===============================================================================
// ヒール処理
//===============================================================================
void My::CHeal::Strategy(CDuelCharacter* /*duel*/, CCard* card)
{
	//登録されているキャラクターを取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	int life;

	//リスト周回
	for (auto& itr : List)
	{
		if (itr == nullptr) { continue; }

		if (itr->GetArea() != card->GetTarget()) { continue; }

		life = itr->GetLife();
		if (life > INT_ZERO)
		{//TODO:選択できない旨のUI表示
			++life;
		}
		itr->SetLife(life);
	}

	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}
}

//===============================================================================
// コンストラクタ
//===============================================================================
My::CEnergyAdjust::CEnergyAdjust()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CEnergyAdjust::~CEnergyAdjust()
{
}

//===============================================================================
// エナジー変動処理
//===============================================================================
void My::CEnergyAdjust::Strategy(CDuelCharacter* /*duel*/, CCard* card)
{
	//登録されているキャラクターを取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	int nEnergy;

	//リスト周回
	for (auto& itr : List)
	{
		if (itr == nullptr) { continue; }

		if (itr->GetArea() != card->GetTarget()) { continue; }

		nEnergy = itr->GetEnergy();
		nEnergy += 10;
		itr->SetEnergy(nEnergy);
	}

	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}
}
