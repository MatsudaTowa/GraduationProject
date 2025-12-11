//===================================================================================================
// 
// 重なっているカードの管理
// Author : Umeda Souma
// 
//===================================================================================================
#include "overlap_card_manager.h"
#include "zone_manager.h"

My::COverlapCardManager::COverlapCardManager()
{
	m_pOverlapCardList.clear();
}

My::COverlapCardManager::~COverlapCardManager()
{
	m_pOverlapCardList.clear();
}

void My::COverlapCardManager::Regist(COverlapCard* pOverlapCard, CDuelCharacter* duel)
{
	//// ゾーンマネージャーの取得
	//CZoneManager* pZoneManager = duel->GetZoneManager();

	//// オーバーラップカードにカードを追加
	//std::list<COverlapCard*> overlap_list = pZoneManager->GetCastPreviewZone()->GetOverlapCardList();

	//// TODO : [OverlapCardList]を[push_back]する時の条件
	//	// -------------------------------------------------
	//	// リストのサイズがないとき
	//	// デュエルキャラクターが違うとき
	//	// カードを重ねなかったとき
	//if (overlap_list.size() <= 0)
	//{
	//	overlap_list.push_back(new COverlapCard);
	//}
}

void My::COverlapCardManager::ReMove(COverlapCard* pOverlapCard)
{
}
