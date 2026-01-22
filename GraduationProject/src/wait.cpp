//===========================================================================================================================================================
// 
// 待機 [wait.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "wait.h"
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
	const D3DXVECTOR3 BASE_POS = { 1200.0f,100.0f,0.0f };
	const float SHIFT_POS = 170.0f;
	const float CARD_VIEW_CORRECTION = 7.0f;
}

My::CWait::CWait():CCardInfoView(),
m_pos(VEC3_RESET_ZERO),
m_pBG(nullptr),
m_pNext(nullptr)
{
	if (m_pBG == nullptr)
	{
		m_pBG = CWaitZoneBG::Create();
	}
	if (m_pNext == nullptr)
	{
		m_pNext = CWaitNextUI::Create({ 1050.0f,560.0f,0.0f });
	}
}

My::CWait::~CWait()
{
	if (m_pBG != nullptr)
	{
		m_pBG->Uninit();
		m_pBG = nullptr;
	}
	if (m_pNext != nullptr)
	{
		m_pNext->Uninit();
		m_pNext = nullptr;
	}
}

void My::CWait::Init()
{
	m_pos = BASE_POS;
}

void My::CWait::Update(CPlayerDuelState* state, CActiveSceneCharacter* player)
{
	state->GetZoneManager()->GetWaitZone()->GetSelectionRange()->SetisDraw(state->GetIsWaitView());
	if (!state->GetIsWaitView())
	{
		m_pos = BASE_POS;

		return;
	}
	std::list<CCard*> card_list = state->GetZoneManager()->GetWaitZone()->GetList();

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
}

void My::CWait::Cast(CDuelCharacter* character, CActiveSceneCharacter* player)
{
}

My::CWait* My::CWait::Create()
{
	CWait* pStay = new CWait;
	pStay->Init();
	return pStay;
}

My::CCard* My::CWait::SearchList(CDuelCharacter* character, int num)
{
	auto itr = character->GetZoneManager()->GetHandZone()->GetList().begin();

	// リスト分回す
	for (unsigned int i = 0; i < character->GetZoneManager()->GetHandZone()->GetList().size(); i++)
	{
		if (i == num) {
			return *itr;
		}
		itr++;
	}

	return nullptr;
}
