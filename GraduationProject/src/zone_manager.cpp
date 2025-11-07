//===========================================================================================================================================================
// 
// ゾーンマネージャーの処理 [zone_manager.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include"zone_manager.h"

/**
* @brief コンストラクタ
*/
My::CZoneManager::CZoneManager():
	m_pDeck(nullptr)
{

}

/**
* @brief デストラクタ
*/
My::CZoneManager::~CZoneManager()
{
	
}

/**
* @brief 初期化処理
* @return 成功したかどうか
*/
HRESULT My::CZoneManager::Init()
{
	if (m_pDeck == nullptr)
	{
		m_pDeck = new CDeck();
	}

	if (m_pDeck != nullptr)
	{
		m_pDeck->Init();
	}

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CZoneManager::Uninit()
{

}

/**
* @brief 更新処理
*/
void My::CZoneManager::Update()
{
	
}

/**
* @brief 山札のポインタ取得
*/
My::CDeck* My::CZoneManager::GetDeck()
{
	return nullptr;
}
