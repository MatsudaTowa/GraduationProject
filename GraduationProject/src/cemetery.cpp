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
#include "card_frame.h"

namespace
{
	const D3DXVECTOR3 BASE_POS = { 1100.0f,100.0f,0.0f };
	const float SHIFT_POS = 170.0f;
	const float CARD_VIEW_CORRECTION = 7.0f;
}

//===========================================================================================================================================================
// コンストラクタ
//===========================================================================================================================================================
My::CCemetery::CCemetery():m_pos(VEC3_RESET_ZERO)
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
	m_pos = BASE_POS;
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::CCemetery::Update(CPlayerDuelState* state, CActiveSceneCharacter* player)
{
	if (!state->GetIsCemeteryView())
	{
		m_pos = BASE_POS;
		return;
	}
	std::list<CCard*> card_list = state->GetZoneManager()->GetCemetery()->GetList();

	int card_size = card_list.size();

	float scroll = GET_INPUT_MOUSE->GetMouseMove().z;
	float delta_y = scroll * 0.5f;

	float cardHeight = CCardFrame::CARD_HEIGHT;
	float contentHeight = cardHeight + SHIFT_POS * (card_size - 1);	//全部の高さを取得
	float viewHeight = SCREEN_HEIGHT;

	// 先頭カードが「下」に行きすぎない最大値 TODO:この補正値
	float maxY = SCREEN_HEIGHT - cardHeight * CARD_VIEW_CORRECTION;

	// 末尾カードが「上」に行きすぎない最小値
	float minY = SCREEN_HEIGHT - contentHeight;

	float newY = m_pos.y + delta_y;

	// 上限値の場合に押し戻す処理
	if (newY < minY)
	{
		newY = minY;
	}
	if (newY > maxY)
	{
		newY = maxY;
	}

	m_pos.y = newY;

	int i = 0;
	for (auto& itr : card_list)
	{
		if (!itr) { continue; }
		D3DXVECTOR3 pos = m_pos;
		pos.y += SHIFT_POS * (float)i;

		D3DXVECTOR3 worldPos = ConvertToWorldPoint(GET_CAMERA(GET_CAMERA_IDX), pos, { 0,0,0 });

		itr->SetCurrentZone(CCard::ZONE::CEMETERY);
		itr->ChangeState(CCardState::CARD_CEMETERY, state);
		itr->SetNeutralPos(worldPos);
		itr->SetPos(worldPos);

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
