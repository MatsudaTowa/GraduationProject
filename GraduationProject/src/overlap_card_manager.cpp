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
My::COverlapCard* My::COverlapCardManager::Create(CDuelCharacter* duel,CInputMouse::AREA area)
{
	// ゾーンマネージャーの取得
	CZoneManager* pZoneManager = duel->GetZoneManager();

	// オーバーラップカードにカードを追加
	std::list<COverlapCard*> overlap_list = pZoneManager->GetCastPreviewZone()->GetOverlapManager()->GetOverlapCardList();

	// TODO 
	// 今現在起きているバグ
	// 重なったカードが最初に登録、二回目はもう中身が空じゃなくなっている
	// カードを登録できずじまい
	// 

	// リストのイテレーターを回す
	for (auto& itr : overlap_list)
	{
		// 同じターゲットの重なったカードがあった時
		if (itr->GetTarget() == area)
		{
			// すでにある overlap を返す(同じターゲット)
			return itr;
		}
	}

	// 同じターゲットのカードリストが存在しなかった場合
	// overlap 生成
	COverlapCard* pOverlapCard = new COverlapCard;
	pOverlapCard->SetTarget(area);

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
	for (auto& itr : overlaplist)
	{
		// ターゲットが同じじゃなければ
		if (itr->GetTarget() != card->GetTarget())
			continue;

		// 重ねたカードリストを回す
		for (auto& i : itr->GetOverlapCards())
		{
			i->GetState()->Init(i, duel);
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
