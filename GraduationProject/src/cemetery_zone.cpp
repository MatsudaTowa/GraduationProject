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

}

/**
* @brief デストラクタ
*/
My::CCemeteryZone::~CCemeteryZone()
{

}

/**
* @brief 墓地に送る
*/
void My::CCemeteryZone::SendToGrave(CCard* pCard)
{
	// 追加処理
	AddCard(pCard);
}
