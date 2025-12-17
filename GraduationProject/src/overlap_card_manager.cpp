//===================================================================================================
// 
// 重なっているカードの管理
// Author : Umeda Souma
// 
//===================================================================================================
#include "overlap_card_manager.h"
#include "zone_manager.h"

//=========================================================================
// コンストラクタ
//=========================================================================
My::COverlapCardManager::COverlapCardManager()
{
	m_pOverlapCardList.clear();
}

//=========================================================================
// デストラクタ
//=========================================================================
My::COverlapCardManager::~COverlapCardManager()
{
	m_pOverlapCardList.clear();
}

//=========================================================================
// 重ねたカードの管理リスト生成
//=========================================================================
My::COverlapCard* My::COverlapCardManager::Create(CDuelCharacter* duel,CCard* pcard)
{
	// ゾーンマネージャーの取得
	CZoneManager* pZoneManager = duel->GetZoneManager();

	// オーバーラップカードにカードを追加
	std::list<COverlapCard*> overlap_list = pZoneManager->GetCastPreviewZone()->GetOverlapManager()->GetOverlapCardList();

	/*
	{
		// カメラ取得
		CCamera* pCamera = GET_CAMERA(0);
		// マウス取得
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		// マウス座標
		D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };
		// カード座標
		D3DXVECTOR3 pos = GetPos();

		// カードの座標をスクリーン座標変換した座標を格納する変数
		D3DXVECTOR3 screenpos, screenposN;

		// カード座標をスクリーン座標変換する
		screenpos = ConvertToScreenPos(pCamera, pos);
		// ニュートラル状態のカード座標をスクリーン座標変換する
		screenposN = ConvertToScreenPos(pCamera, m_NeutralPos);

		// マウスとカードの位置の差
		D3DXVECTOR2 resultpos;
		resultpos.x = mousepos.x - screenpos.x;
		resultpos.y = mousepos.y - screenpos.y;

		// マウスとニュートラル状態のカードの位置の差
		D3DXVECTOR2 resultposN;
		resultposN.x = mousepos.x - screenposN.x;
		resultposN.y = mousepos.y - screenposN.y;

		// 矩形判定
		if (resultpos.x <= 50.0f * GetSize().x && resultpos.x >= -50.0f * GetSize().x &&
			resultpos.y <= 100.0f * GetSize().y && resultpos.y >= -100.0f * GetSize().y ||
			resultposN.x <= 50.0f && resultposN.x >= -50.0f &&
			resultposN.y <= 100.0f && resultposN.y >= -100.0f)
	}
	*/

	// リストのイテレーターを回す
	for (auto& itr : overlap_list)
	{
		for (auto& target : pcard->GetTargetPlayerList())
		{
			// 同じターゲットの重なったカードがあった時
			if (itr->GetTarget() == target->GetArea())
			{
				// オーバーラップカードからカード検索
				std::vector<CCard*>card = itr->GetOverlapCards();

				// TODO
				// カードを検索してキャスト時カードの上に重なってたらなどやる場所
				// ここでやるかは知らない
				// おそらくここでやらない
				// 【カードの位置設定について】
				// カードは現在 card_state.cpp の SetCardPos() にて設定している
				// そこで UserArea の取得によって位置を設定。変えます～
				// カードの位置を変えて、そうしましたらカードを重ねたとき、
				// ターゲットを設定するときに重ねたカードのターゲットを参照すればできそうですね
				for (auto& iter : card)
				{
					if (iter->GetPos() == pcard->GetPos())
					{
						// すでにある overlap を返す(同じターゲット&&同じ重ね方)
						return itr;
					}
				}
			}
		}
		
	}

	// 同じターゲットのカードリストが存在しなかった場合
	// overlap 生成
	COverlapCard* pOverlapCard = new COverlapCard;
	for (auto& target : pcard->GetTargetPlayerList())
	{
		pOverlapCard->SetTarget(target->GetArea());
	}
	// TODO : [OverlapCardList]を[push_back]する時の条件
		// -------------------------------------------------
		// リストのサイズがないとき
		// デュエルキャラクターが違うとき
		// カードを重ねなかったとき
	/*if (overlap_list.size() <= 0)*/
	{
		overlap_list.push_back(pOverlapCard);
		SetOverlapCardList(overlap_list);
		return pOverlapCard;
	}

	return nullptr;
}

//===========================================================================================================
// ステイ時間のリセット
//===========================================================================================================
void My::COverlapCardManager::ResetStayTime(CDuelCharacter* duel, CCard* card)
{
	// 重ねたカードのリスト取得
	std::list<COverlapCard*>overlaplist = m_pOverlapCardList;

	// リストを回す
	for (auto Target : card->GetTargetPlayerList())
	{
		// リストを回す
		for (auto& itr : overlaplist)
		{
			// ターゲットが同じじゃなければ
			if (itr->GetTarget() != Target->GetArea())
				continue;

			// 重ねたカードリストを回す
			for (auto& i : itr->GetOverlapCards())
			{
				i->GetState()->Init(i, duel);
			}
		}
	}
}

//===========================================================================================================
// 削除
//===========================================================================================================
void My::COverlapCardManager::ReMove()
{
	// TODO
	// オーバーラップリストの削除もそうだが、
	// オーバーラップカード事態の削除も急用
	// overlap関連の名前を早急に変えましょう
	m_pOverlapCardList.clear();
}
