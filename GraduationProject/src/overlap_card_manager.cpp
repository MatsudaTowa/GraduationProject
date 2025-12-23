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

	// リストのイテレーターを回す
	for (auto& overlap : overlap_list)
	{
		for (auto& target : pcard->GetTargetPlayerList())
		{
			// 同じターゲットの重なったカードがあった時
			if (overlap->GetTarget() == target->GetArea())
			{
				// オーバーラップカードからカード検索
				std::vector<CCard*>overlap_card = overlap->GetOverlapCards();

				// TODO
				// カードを検索してキャスト時カードの上に重なってたらなどやる場所
				// ここでやるかは知らない
				// おそらくここでやらない
				// 【カードの位置設定について】
				// カードは現在 card_state.cpp の SetCardPos() にて設定している
				// そこで UserArea の取得によって位置を設定。変えます～
				// カードの位置を変えて、そうしましたらカードを重ねたとき、
				// ターゲットを設定するときに重ねたカードのターゲットを参照すればできそうですね

				// カードが重なったかどうかチェックする
				bool b = overlap->CheckOverlap(pcard);
				b = true;

				// 重ねていたら
				if (b)
				{
					// ステイ時間をリセット
					ResetStayTime(duel, overlap);

					// すでにある overlap を返す(同じターゲット&&同じ重ね方)
					return overlap;
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
	
	overlap_list.push_back(pOverlapCard);
	SetOverlapCardList(overlap_list);

	return pOverlapCard;
}

//============================================================================================================
// ステイ時間のリセット
//============================================================================================================
void My::COverlapCardManager::ResetStayTime(CDuelCharacter* duel, COverlapCard* overlap)
{
	// 重ねたカードリストを回す
	for (auto& i : overlap->GetOverlapCards())
	{
		i->GetState()->Init(i, duel);
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
