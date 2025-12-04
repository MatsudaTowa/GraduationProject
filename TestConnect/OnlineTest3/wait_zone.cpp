//===========================================================================================================================================================
// 
// 待機ゾーン [wait_zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "wait_zone.h"

/**
* @brief コンストラクタ
*/
My::CWaitZone::CWaitZone()
{

}

/**
* @brief デストラクタ
*/
My::CWaitZone::~CWaitZone()
{

}

/**
* @brief 守備カードが置かれたときに送る
* @param [in]カードのポインタ
*/
void My::CWaitZone::SendDefenseCard(CCard* pCard)
{
	// カードを追加
	AddList(pCard);
}

/**
* @brief カードを並べる
*/
void My::CWaitZone::LineUpCard()
{
	//リストの周回
	for (CCard* pCard : GetList())
	{

	}
}
