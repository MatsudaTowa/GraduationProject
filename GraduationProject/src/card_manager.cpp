//================================
//
//card_manager.cppに必要な宣言[card_manager.cpp]
//Author:松田永久
// 
//================================
#include "card_manager.h"

//================================
// コンストラクタ
//================================
My::CCardManager::CCardManager()
{
	m_CardList.clear();
}

//================================
// デストラクタ
//================================
My::CCardManager::~CCardManager()
{
	m_CardList.clear();
}

//================================
// 初期化
//================================
HRESULT My::CCardManager::Init()
{
	return S_OK;
}

//================================
// 終了
//================================
void My::CCardManager::Uninit()
{
}

//================================
// カードのリストに登録
//================================
void My::CCardManager::RegistCardList(CCard* card)
{
	//ターゲットリストの情報を登録
	m_CardList.push_back(card);
	card->SetUpdate(false);
}

//================================
// カードのリストから削除
//================================
void My::CCardManager::RemoveCardList(CCard* card)
{
	//サイズが0なら抜ける
	if (m_CardList.size() == 0)
	{
		return;
	}
	//カードの情報を削除
	m_CardList.remove(card);
}

//================================
// リストのリセット
//================================
void My::CCardManager::ResetCardList()
{
	//リストの周回 TODO:他の場所で参照するならクリアのみにする
	for (auto& iter : m_CardList)
	{
		iter->Uninit();
	}

	m_CardList.clear();
}
