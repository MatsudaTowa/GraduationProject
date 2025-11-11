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
	m_pDeck(nullptr),
	m_pCemetery(nullptr),
	m_pFieldZone(nullptr)
{

}

/**
* @brief デストラクタ
*/
My::CZoneManager::~CZoneManager()
{
	m_pDeck = nullptr;
	m_pCemetery = nullptr;
	m_pFieldZone = nullptr;
}

/**
* @brief 初期化処理
* @return 成功したかどうか
*/
HRESULT My::CZoneManager::Init()
{
	// デッキのポインタ生成
	if (m_pDeck == nullptr)
	{
		m_pDeck = new CDeckZone();
	}

	if (m_pDeck != nullptr)
	{
		m_pDeck->Init();
	}

	// 墓地のポインタ生成
	if (m_pCemetery == nullptr)
	{
		m_pCemetery = new CCemeteryZone();
	}

	// フィールドゾーンポインタ生成
	if (m_pFieldZone == nullptr)
	{
		m_pFieldZone = new CFieldZone();
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
* @return デッキのポインタ
*/
My::CDeckZone* My::CZoneManager::GetDeck()
{
	return m_pDeck;
}

/**
* @brief 墓地のポインタ取得
* @return 墓地のポインタ
*/
My::CCemeteryZone* My::CZoneManager::GetCemetery()
{
	return m_pCemetery;
}

/**
* @brief フィールドゾーンのポインタ取得
*/
My::CFieldZone* My::CZoneManager::GetFieldZone()
{
	return m_pFieldZone;
}

/**
* @brief 手札ゾーンの取得
* @return 手札のポインタ
*/
My::CHandZone* My::CZoneManager::GetHandZone()
{
	return m_pHandZone;
}

/**
* @brief 待機ゾーンのポインタ
* @return 待機ゾーンのポインタ
*/
My::CWaitZone* My::CZoneManager::GetWaitZone()
{
	return m_pWaitZone;
}
