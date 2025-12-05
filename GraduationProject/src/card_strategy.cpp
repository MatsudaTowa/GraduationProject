//===============================================================================
//
//カードストラテジー[active_scene_character_state.cpp]
//Author Matsuda Towa
//
//===============================================================================
#include "card_strategy.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "zone_manager.h"
#include "enemy_state.h"

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
	//カードのトリガー処理
	card->Trigger();

	////登録されているキャラクターを取得
	//std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	//int life;
	//
	////リスト周回
	//for (auto& itr : List)
	//{
	//	if (itr == nullptr) { continue; }

	//	if (itr->GetArea() != card->GetTarget()) { continue; }

	//	life = itr->GetLife();
	//	if (life > INT_ZERO)
	//	{//TODO:選択できない旨のUI表示
	//		--life;
	//	}
	//	itr->SetLife(life);
	//}

	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	/*if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}*/
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
void My::CDiffence::Strategy(CDuelCharacter* /*duel*/, CCard* card)
{
	//カードのトリガー処理
	card->Trigger();
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

		//TODO:カードのエナジー変動値を代入
		nEnergy += 10;
		itr->SetEnergy(nEnergy);
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
My::CHandDestruction::CHandDestruction()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CHandDestruction::~CHandDestruction()
{
}

//===============================================================================
// 初期化
//===============================================================================
void My::CHandDestruction::Strategy(CDuelCharacter* duel, CCard* card)
{
	//登録されているキャラクターを取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	//リスト周回
	for (auto& itr : List)
	{
		if (itr == nullptr) { continue; }

		if (itr->GetArea() != card->GetTarget()) { continue; }
		
		CActiveSceneCharacterState* state = itr->GetState();

		if(typeid(*state) != typeid(CEnemyDuelState))
		{
			continue;
		}

		//TODO : デュエル状態を参照できる場所が必要
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(itr->GetState());	//キャスト

		CHandZone* hand_zone = DuelState->GetZoneManager()->GetHandZone();
		CCemeteryZone* cemetary_zone = DuelState->GetZoneManager()->GetCemetery();
		std::list<CCard*> hand_list = DuelState->GetZoneManager()->GetHandZone()->GetList();

		int hand_size = hand_list.size();

		if (hand_size == 0)
		{
			return;
		}

		int idx = Rundom(1, hand_list.size());

		int i = 0;
		for (auto& card_itr : hand_list)
		{
			if (card_itr == nullptr) 
			{ 
				++i;
				continue; 
			}

			if (i != idx)
			{
				++i;
				continue;
			}

			//カードの状態を墓地に変更
			card_itr->ChangeState(CCardState::CARD_CEMETERY, DuelState);

			//DuelState->GetZoneManager()->MoveZone(card_itr, hand_zone, cemetary_zone, true);
		}
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
My::CFlipMyDeck::CFlipMyDeck()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CFlipMyDeck::~CFlipMyDeck()
{
}

//===============================================================================
// 山札からめくる処理
//===============================================================================
void My::CFlipMyDeck::Strategy(CDuelCharacter* duel, CCard* card)
{
	CZoneManager* pZoneManager = duel->GetZoneManager();

	//TODO:ここの数もカードのステータスから
	std::list<CCard*> card_list = pZoneManager->GetDeck()->Flip(1);
	for (auto& itr : card_list)
	{
		if (itr == nullptr) { continue; }
		//カードを引けるか

		duel->GetZoneManager()->MoveZone(itr, pZoneManager->GetDeck(), pZoneManager->GetFlipPreviewZone(), true);

		//TODO:フリッププレビューゾーンの列挙に
		itr->SetCurrentZone(CCard::FLIP);
	}


	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}
}

My::CSendToMyCemetary::CSendToMyCemetary()
{
}

My::CSendToMyCemetary::~CSendToMyCemetary()
{
}

void My::CSendToMyCemetary::Strategy(CDuelCharacter* duel, CCard* card)
{
	CZoneManager* pZoneManager = duel->GetZoneManager();

	//TODO:ここの数もカードのステータスから
	std::list<CCard*> card_list = pZoneManager->GetFlipPreviewZone()->GetList();

	//TODO:どの番号とか選択する
	for (auto& itr : card_list)
	{
		if (itr == nullptr) { continue; }
		//カードを引けるか

		pZoneManager->MoveZone(itr, itr->CastToZone(itr->GetCurrentZone(), duel), pZoneManager->GetCemetery(), true);

		//TODO:フリッププレビューゾーンの列挙に
		itr->SetCurrentZone(CCard::CEMETERY);
	}


	//オンライン時は通信処理TODO : カードのやり取りが出来たら必要なし
	if (CRakNet::GetInstance()->GetOnline())
	{
		CRakNet::GetInstance()->SendStatus();
	}
}
