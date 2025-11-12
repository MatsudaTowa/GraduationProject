//===========================================================================================================================================================
// 
// ゾーンの処理 [zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "zone.h"

/**
* @brief コンストラクタ
*/
My::CZone::CZone()
{
	m_CardList.clear();	// カードのデータ初期化
}

/**
* @brief デストラクタ
*/
My::CZone::~CZone()
{
	m_CardList.clear();	// カードのデータ初期化
}

/**
* @brief 初期化処理
*/
HRESULT My::CZone::Init()
{
	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CZone::Uninit()
{

}

/**
* @brief 更新処理
*/
void My::CZone::Update()
{

}

/**
* @brief 追加処理
*/
void My::CZone::AddList(CCard* card)
{
	if (card)
	{
		m_CardList.push_back(card);
	}
}

/**
* @brief 削除処理
*/
bool My::CZone::DeleteList(CCard* card)
{
	// カードを検索する
	auto it = std::find(m_CardList.begin(), m_CardList.end(), card);
	if (it != m_CardList.end()) 
	{// 見つかった場合削除
		m_CardList.erase(it);
		return true;
	}
	return false;
}
