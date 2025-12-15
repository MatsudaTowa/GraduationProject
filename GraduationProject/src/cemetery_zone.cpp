//===========================================================================================================================================================
// 
// 墓地ゾーンの処理 [cemetery_zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "cemetery_zone.h"

/**
* @brief コンストラクタ
*/
My::CCemeteryZone::CCemeteryZone() :m_pRange(nullptr)
{
	if (m_pRange == nullptr)
	{
		m_pRange = CSelectionRange::Create({ 1100.0f, 10.0f,0.0f }, { 100.0f, 1000.0f });
		m_pRange->SetColor({ COLOR_BLACK.r,COLOR_BLACK.g,COLOR_BLACK.b,0.5f });
		m_pRange->SetisDraw(false);
	}
}

/**
* @brief デストラクタ
*/
My::CCemeteryZone::~CCemeteryZone()
{
	if (m_pRange == nullptr)
	{
		m_pRange->SetisDelete(true);
		m_pRange = nullptr;
	}
}

void My::CCemeteryZone::Update()
{
}

/**
* @brief 墓地に送る
*/
void My::CCemeteryZone::SendToGrave(CCard* pCard)
{
	// 追加処理
	AddList(pCard);
	pCard->SetPos(VEC3_RESET_ZERO);
}
