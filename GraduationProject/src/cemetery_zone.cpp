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
My::CCemeteryZone::CCemeteryZone()
{
	CSelectionRange* pRange = GetSelectionRange();
	if (pRange == nullptr)
	{
		pRange = CSelectionRange::Create({ 1200.0f, 10.0f,0.0f }, { 100.0f, 1000.0f });
		pRange->SetColor({ COLOR_BLACK.r,COLOR_BLACK.g,COLOR_BLACK.b,0.5f });
		pRange->SetisDraw(false);
	}
	SetSelectionRange(pRange);
}

/**
* @brief デストラクタ
*/
My::CCemeteryZone::~CCemeteryZone()
{
}

void My::CCemeteryZone::Update()
{
	CZone::Update();
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
