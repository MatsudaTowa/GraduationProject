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
	m_pFieldZone(nullptr),
	m_pHandZone(nullptr),
	m_pWaitZone(nullptr),
	m_pCastPreviewZone(nullptr)
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
	m_pHandZone = nullptr;
	m_pWaitZone = nullptr;
	m_pCastPreviewZone = nullptr;
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

	// キャストプレビューゾーンのポインタ生成
	if (m_pCastPreviewZone == nullptr)
	{
		m_pCastPreviewZone = new CCastPreviewZone();
	}

	// 待機ゾーンのポインタ生成
	if (m_pWaitZone == nullptr)
	{
		m_pWaitZone = new CWaitZone();
	}

	// 手札ゾーンのポインタ生成
	if (m_pHandZone == nullptr)
	{
		m_pHandZone = new CHandZone();
	}

	return S_OK;
}

/**
* @brief 終了処理
*/
void My::CZoneManager::Uninit()
{
	// デッキのポインタ生成
	if (m_pDeck != nullptr)
	{
		delete m_pDeck;
		m_pDeck = nullptr;
	}

	// 墓地のポインタ生成
	if (m_pCemetery != nullptr)
	{
		delete m_pCemetery;
		m_pCemetery = nullptr;
	}

	// フィールドゾーンポインタ生成
	if (m_pFieldZone != nullptr)
	{
		delete m_pFieldZone;
		m_pFieldZone = nullptr;
	}

	// キャストプレビューゾーンのポインタ生成
	if (m_pCastPreviewZone != nullptr)
	{
		delete m_pCastPreviewZone;
		m_pCastPreviewZone = nullptr;
	}

	// 待機ゾーンのポインタ生成
	if (m_pWaitZone != nullptr)
	{
		delete m_pWaitZone;
		m_pWaitZone = nullptr;
	}

	// 手札ゾーンのポインタ生成
	if (m_pHandZone != nullptr)
	{
		delete m_pHandZone;
		m_pHandZone = nullptr;
	}
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
* @brief 待機ゾーンの取得
* @return 待機ゾーンのポインタ
*/
My::CWaitZone* My::CZoneManager::GetWaitZone()
{
	return m_pWaitZone;
}

/**
* @brief キャストプレビューゾーンの取得
* @return キャストプレビューゾーンのポインタ
*/
My::CCastPreviewZone* My::CZoneManager::GetCastPreviewZone()
{
	return m_pCastPreviewZone;
}

/**
* @brief ゾーンの移動処理
* @param [in]カードのポインタ
* @param [in]現在のゾーン
* @param [in]次のゾーン
* @return 成功したかどうか
*/
bool My::CZoneManager::MoveZone(CCard* pCard, CZone* pCurrentZone, CZone* pNextZone,bool Addflag)
{
	if (pCard == nullptr || pCurrentZone == nullptr || pNextZone == nullptr)
	{// カードがなかったりゾーンが存在しなかったら
		return false;
	}
	else
	{
		// 現在のゾーンのリストから削除
		pCurrentZone->DeleteList(pCard);

		if (pNextZone == m_pDeck)
		{// ゾーンの移動先が山札の場合Addflagがtrueならpush_front、falseならpush_back
			pNextZone->SelectAddList(pCard, Addflag);
		}
		else
		{// ゾーンの移動先が山札以外の場合
			// 新しいゾーンにカードを追加
			pNextZone->AddList(pCard);
		}
	}

	return true;
}
