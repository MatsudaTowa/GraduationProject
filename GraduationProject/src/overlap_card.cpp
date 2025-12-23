//=========================================================================================================================
// 
// 重なっているカードを管理するクラス
// Auuthor : Umeda Souma
// 
//=========================================================================================================================
#include "overlap_card.h"

//=========================================================================================
// コンストラクタ
//=========================================================================================
My::COverlapCard::COverlapCard():
	m_IsOverlap(false)
{
	m_pOverlapCards.clear();
}

//=========================================================================================
// デストラクタ
//=========================================================================================
My::COverlapCard::~COverlapCard()
{
	m_pOverlapCards.clear();
}

//=========================================================================================
// カードの登録
//=========================================================================================
void My::COverlapCard::Regist(CCard* pCard)
{
	m_pOverlapCards.push_back(pCard);
}

//=========================================================================================
// 指定されたカードのポインタを検索して削除
//=========================================================================================
void My::COverlapCard::ReMove(CCard* pCard)
{
	// 指定されたカードのポインタを検索
	auto it = std::find(m_pOverlapCards.begin(), m_pOverlapCards.end(), pCard);

	// 見つかった場合は削除
	if (it != m_pOverlapCards.end())
	{
		m_pOverlapCards.erase(it);
	}
}

//=========================================================================================
// カードを重ねる処理
//=========================================================================================
bool My::COverlapCard::CheckOverlap(CCard* pCard)
{
	for (auto& ItrCard : m_pOverlapCards)
	{
		D3DXVECTOR3 itrcard_pos = ItrCard->GetPos();
		D3DXVECTOR3 castcard_pos = pCard->GetPos();

		// カメラ取得
		CCamera* pCamera = GET_CAMERA(0);
		// マウス取得
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		// マウス座標
		D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };

		// カードの座標をスクリーン座標変換した座標を格納する変数
		D3DXVECTOR3 screenpos,itrscreenpos;

		// カード座標をスクリーン座標変換する
		screenpos = ConvertToScreenPos(pCamera, castcard_pos);
		itrscreenpos = ConvertToScreenPos(pCamera, itrcard_pos);

		D3DXVECTOR2 size, itrsize;

		// サイズ設定
		size = { pCard->GetSize().x,pCard->GetSize().y };
		itrsize = { ItrCard->GetSize().x,ItrCard->GetSize().y };

		// 倍率を変える
		size *= 10;
		itrsize *= 10;

		// カード同士の当たり判定
		bool b = CManager::GetInstance()->GetColision()->Check2DPolygonColision(
			itrscreenpos, itrsize,
			screenpos, size);

		if (b)
			return true;
	}

	return false;
}
