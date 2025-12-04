//===========================================================================================================================================================
// 
// キャストプレビューゾーン [castpreview_zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "castpreview_zone.h"

/**
* @brief コンストラクタ
*/
My::CCastPreviewZone::CCastPreviewZone()
{

}

/**
* @brief デストラクタ
*/
My::CCastPreviewZone::~CCastPreviewZone()
{
}

/**
* @brief プレイヤーが使ったカード
* @param [in]カードのポインタ
*/
void My::CCastPreviewZone::UsePlayerCard(CCard* pCard)
{
	// カードを追加
	AddList(pCard);
}
