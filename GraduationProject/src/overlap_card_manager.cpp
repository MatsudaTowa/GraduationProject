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

	bool b = false;

	// リストのイテレーターを回す
	for (auto& itr : overlap_list)
	{
		// 同じターゲットの重なったカードがあった時
		if (itr->GetTarget() == area ||
			overlap_list.size() <= 0)
		{
			return nullptr;
		}
	}

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

void My::COverlapCardManager::ReMove(COverlapCard* pOverlapCard)
{
}
