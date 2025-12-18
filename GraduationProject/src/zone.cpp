//===========================================================================================================================================================
// 
// ゾーンの処理 [zone.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "zone.h"
#include "duel_manager.h"
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
	for (auto& itr : m_CardList)
	{
		if (itr == nullptr) { continue; }
		// マウス取得
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		// カードの座標をスクリーン座標変換した座標を格納する変数
		D3DXVECTOR3 screenpos;

		// カード座標
		D3DXVECTOR3 pos = itr->GetPos();

		// カード座標をスクリーン座標変換する
		screenpos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), pos);
		// マウス座標
		D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };
		// マウスとカードの位置の差
		D3DXVECTOR2 resultpos;
		resultpos.x = mousepos.x - screenpos.x;
		resultpos.y = mousepos.y - screenpos.y;

		if (GET_INPUT_MOUSE->GetTrigger(0))
		{
			// 矩形判定
			if (resultpos.x <= 50.0f * itr->GetSize().x && resultpos.x >= -50.0f * itr->GetSize().x &&
				resultpos.y <= 100.0f * itr->GetSize().y && resultpos.y >= -100.0f * itr->GetSize().y)
			{
				CDuel_Manager::GetInstance()->GetCardInfoUI()->SetisDraw(true);
				return;
			}
			else
			{
				CDuel_Manager::GetInstance()->GetCardInfoUI()->SetisDraw(false);
			}
		}

	}
}

/**
* @brief 追加処理
* @param [in]カードのポインタ
*/
void My::CZone::AddList(CCard* card)
{
	if (card)
	{
		m_CardList.push_back(card);
	}
}

/**
* @brief リストの選択追加
* @param [in]カードのポインタ
* @param [in]追加フラグ
*/
void My::CZone::SelectAddList(CCard* card, bool bAddflag)
{
	if (card)
	{
		if (bAddflag)
		{// 要素を先頭に追加
			m_CardList.push_front(card);
		}
		else if (!bAddflag)
		{// 要素を末尾に追加
			m_CardList.push_back(card);
		}
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

/**
* トップカードの取得
*/
My::CCard* My::CZone::GetTopCard()
{
	if (m_CardList.empty()) return nullptr;

	//先頭のカードを返す
	CCard* pCard = *m_CardList.begin();

	return pCard;
}
