//===========================================================================================================================================================
// 
// 墓地 [cemetery.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "cemetery.h"
#include "card_state.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_assist.h"
#include "active_scene.h"
#include "active_scene_manager.h"
#include "active_scene_state.h"
#include "zone_manager.h"

namespace
{
	const D3DXVECTOR3 BASE_POS = { -400.0f,0.0f,300.0f };
	const float SHIFT_POS = 150.0f;
}

//===========================================================================================================================================================
// コンストラクタ
//===========================================================================================================================================================
My::CCemetery::CCemetery()
{
}

//===========================================================================================================================================================
// デストラクタ
//===========================================================================================================================================================
My::CCemetery::~CCemetery()
{
}

//===========================================================================================================================================================
// 初期化
//===========================================================================================================================================================
void My::CCemetery::Init()
{
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::CCemetery::Update(CPlayerDuelState* state, CActiveSceneCharacter* player)
{
	std::list<CCard*> card_list = state->GetZoneManager()->GetCemetery()->GetList();

	int i = INT_ZERO;
	for (auto& itr : card_list)
	{
		if (itr == nullptr) { continue; }

		D3DXVECTOR3 pos = BASE_POS;

		pos.x += SHIFT_POS * i;

		itr->SetCurrentZone(CCard::ZONE::CEMETERY);
		itr->ChangeState(CCardState::CARD_CEMETERY, state);
		itr->SetNeutralPos(pos);

		itr->SetPos(pos);

		++i;
	}

	//for (auto& iter : state->GetZoneManager()->GetCemetery()->GetList())
	//{
	//	iter->Update(state);
	//}

	// 手札選択
	Select(state);

	//// カードキャスト
	//Cast(state, player);

}

//===========================================================================================================================================================
// 選択
//===========================================================================================================================================================
void My::CCemetery::Select(CPlayerDuelState* state)
{
	// キャスト状態だったらセレクトさせない
	if (GetInfoState() == SELECT)
	{
		return;
	}

	bool isPickUp = GetIsPickUp();
	// 何も選択されていない場合
	if (!isPickUp)
	{
		int nCount = 0;
		for (auto& iter : state->GetZoneManager()->GetCemetery()->GetList())
		{
			// マウスでカード選択
			isPickUp = iter->CardSelectToMouse(state);

			if (isPickUp)
			{// どれかのカードが選択されたら
				SetSelectNum(nCount);	// 今の配列番号を一時格納しておく
				break;
			}
			else
			{
				nCount++;
			}
		}
	}
	else
	{
		int nCount = 0;

		for (auto& iter : state->GetZoneManager()->GetCemetery()->GetList())
		{
			if (nCount != GetSelectNum())
			{
				nCount++;
				continue;
			}

			// 選択番号のカードが非選択状態になったら
			if (iter->GetStateNum() == CCardState::CARD_NEUTRAL)
			{
				isPickUp = false;	// 選択されていない状態にする
			}
			else
			{
				// 選択番号のカードのみ判定する
				isPickUp = iter->CardSelectToMouse(state);
			}
			break;
		}
	}
	SetIsPickUp(isPickUp);
}

//===========================================================================================================================================================
// キャスト
//===========================================================================================================================================================
void My::CCemetery::Cast(CDuelCharacter* character, CActiveSceneCharacter* player)
{
}

//===========================================================================================================================================================
// 生成
//===========================================================================================================================================================
My::CCemetery* My::CCemetery::Create()
{
	CCemetery* pCemetery = new CCemetery;

	pCemetery->Init();

	return pCemetery;
}

My::CCard* My::CCemetery::SearchList(CDuelCharacter* character, int num)
{
	return nullptr;
}
