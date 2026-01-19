//===========================================================================================================================================================
// 
// ‘Ò‹@ [wait.cpp]
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
	const D3DXVECTOR3 BASE_POS = { 1300.0f,100.0f,0.0f };
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
		m_pBG = CWaitZoneBG::Create({ 1050.0f, 660.0f, 0.0f });
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
	std::list<CCard*> card_list = state->GetZoneManager()->GetWaitZone()->GetList();

	int i = INT_ZERO;
	for (auto& itr : card_list)
	{
		if (itr == nullptr) 
		{
			++i;
			continue; 
		}
		itr->SetPos(ConvertToWorldPoint(GET_CAMERA(GET_CAMERA_IDX), m_pos, VEC3_RESET_ZERO));
		if (i == INT_ZERO)
		{
			itr->SetisDraw(true);
		}
		else
		{
			itr->SetisDraw(false);
		}

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

	// ƒŠƒXƒg•ª‰ñ‚·
	for (unsigned int i = 0; i < character->GetZoneManager()->GetHandZone()->GetList().size(); i++)
	{
		if (i == num) {
			return *itr;
		}
		itr++;
	}

	return nullptr;
}
