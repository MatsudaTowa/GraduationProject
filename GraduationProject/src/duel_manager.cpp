////================================
////
////duel_manager.cppに必要な宣言[duel_manager.cpp]
////Author:松田永久
//// 
////================================
//#include "duel_manager.h"
//
////================================
//// コンストラクタ
////================================
//My::CDuelManager::CDuelManager()
//{
//	m_UseCardList.clear();
//}
//
////================================
//// デストラクタ
////================================
//My::CDuelManager::~CDuelManager()
//{
//	m_UseCardList.clear();
//}
//
////================================
//// 初期化
////================================
//HRESULT My::CDuelManager::Init()
//{
//	return S_OK;
//}
//
////================================
//// 終了
////================================
//void My::CDuelManager::Uninit()
//{
//}
//
////================================
//// カードの戦闘処理
////================================
//void My::CDuelManager::ProcessCardBattle(std::list<CCard*> attack_card_list, std::list<CCard*> defence_card_list)
//{
//
//}
//
////================================
//// カードのリストに登録
////================================
//void My::CDuelManager::RegistUseCardList(CCard* card)
//{
//	//ターゲットリストの情報を登録
//	m_UseCardList.push_back(card);
//}
//
////================================
//// カードのリストから削除
////================================
//void My::CDuelManager::RemoveUseCardList(CCard* card)
//{
//	//サイズが0なら抜ける
//	if (m_UseCardList.size() == 0)
//	{
//		return;
//	}
//	//カードの情報を削除
//	m_UseCardList.remove(card);
//}
