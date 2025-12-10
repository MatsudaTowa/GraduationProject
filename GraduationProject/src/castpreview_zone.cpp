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
My::CCastPreviewZone::CCastPreviewZone():
	m_OverlapCard(),
	m_OverlapCardList()
{
	m_OverlapCard.clear();
	m_OverlapCardList.clear();
}

/**
* @brief デストラクタ
*/
My::CCastPreviewZone::~CCastPreviewZone()
{
	m_OverlapCard.clear();
	m_OverlapCardList.clear();
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
