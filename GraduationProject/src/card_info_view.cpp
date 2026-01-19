//===========================================================================================================================================================
// 
// カードの情報を見る [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "card_info_view.h"
#include "card_state.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_assist.h"
#include "active_scene.h"
#include "active_scene_manager.h"
#include "active_scene_state.h"
#include "zone_manager.h"
#include <algorithm>

My::CCardInfoView::CCardInfoView():m_IsPickUp(false),
m_SelectNum(0),
m_TotalNum(0)
{
}

My::CCardInfoView::~CCardInfoView()
{
}

void My::CCardInfoView::Init()
{
}

void My::CCardInfoView::Update(CDuelCharacter* state, CActiveSceneCharacter* player)
{
}

void My::CCardInfoView::Select(CDuelCharacter* state)
{
}

void My::CCardInfoView::Cast(CDuelCharacter* character, CActiveSceneCharacter* player)
{
}

//===========================================================================================================================================================
// セレクトされた時の設定
//===========================================================================================================================================================
void My::CCardInfoView::SelectStateSet(CDuelCharacter* character)
{
	int select_num = GetSelectNum();

	int nCount = 0;
	CCard* pCard = nullptr;

	// すべてのカードを選ばれていない状態にする
	for (auto& iter : character->GetZoneManager()->GetHandZone()->GetList())
	{
		///選択中のカードなら代入
		if (select_num == nCount)
		{
			pCard = iter;
		}

		iter->ChangeState(CCardState::CARD_STATE::CARD_NEUTRAL, character);
		nCount++;
	}

	if (!pCard || select_num <= -1)
		return;

	// 選択中のカードのステートを変える
	if (pCard != nullptr)
	{
		pCard->ChangeState(CCardState::CARD_STATE::CARD_PICKUP, character);
	}
}

