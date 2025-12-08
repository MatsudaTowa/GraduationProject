//===========================================================================================================================================================
// 
// 墓地ゾーンの処理 [cemetery_zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "cemetery_zone.h"

namespace
{
	const D3DXVECTOR3 BASE_POS = { -400.0f,0.0f,300.0f };
	const float SHIFT_POS = 150.0f;
}

/**
* @brief コンストラクタ
*/
My::CCemeteryZone::CCemeteryZone()
{

}

/**
* @brief デストラクタ
*/
My::CCemeteryZone::~CCemeteryZone()
{

}

void My::CCemeteryZone::Update()
{
	std::list<CCard*> card_list = GetCards();

	int i = INT_ZERO;
	for (auto& itr : card_list)
	{
		if (itr == nullptr) { continue; }

		D3DXVECTOR3 pos = BASE_POS;

		pos.x += SHIFT_POS * i;

		itr->SetPos(pos);

		++i;
	}
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
