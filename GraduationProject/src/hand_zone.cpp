//===========================================================================================================================================================
// 
// 手札ゾーン [hand_zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "hand_zone.h"
#include "card_frame.h"
/**
* @brief コンストラクタ
*/
My::CHandZone::CHandZone()
{
}

/**
* @brief デストラクタ
*/
My::CHandZone::~CHandZone()
{
}

/**
* @brief 手札に追加されたもの追加
* @param [in]カードのポインタ
*/
void My::CHandZone::AddHandZone(CCard* pCard)
{
	// リストに追加
	AddList(pCard);
}

/**
* @brief 使われたカードの判断
* @param [in]カード使用フラグ
*/
void My::CHandZone::JudgeUseCard(bool bUse)
{
	// TODO:使われたカードの削除
	if (!bUse)
	{// 使用されていなかったら何もしない
		return;
	}

	std::list<CCard*>cardsToRemove = GetList();

	for (CCard* pCard : cardsToRemove)
	{// カードの削除
		DeleteList(pCard);
	}
}
