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
	m_pOverlapManager = nullptr;
	m_pOverlapManager = new COverlapCardManager;
}

/**
* @brief デストラクタ
*/
My::CCastPreviewZone::~CCastPreviewZone()
{
	delete m_pOverlapManager;
	m_pOverlapManager = nullptr;
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
