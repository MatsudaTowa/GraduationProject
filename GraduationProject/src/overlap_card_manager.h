//===============================================================================================================
// 
// overlap_card_manager.cpp のヘッダー [overlap_card_manager.h]
// Author : Umeda Souma
// 
//===============================================================================================================
#ifndef _OVERLAP_CARD_MANAGER_
#define _OVERLAP_CARD_MANAGER_
#include "overlap_card.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief 重なっているカードを管理するクラス
	*/
	class COverlapCardManager
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		COverlapCardManager();

		/**
		* @brief デストラクタ
		*/
		~COverlapCardManager();

		/**
		* @brief 登録
		*/
		void Regist(COverlapCard* pOverlapCard, CDuelCharacter* duel);

		/**
		* @brief 削除
		*/
		void ReMove(COverlapCard* pOverlapCard);

		/**
		* @brief 重なったカードリストの設定
		* @param [in]m_pOverlapCardList 重なったカードリスト
		*/
		void SetOverlapCardList(std::list<COverlapCard*>list) { m_pOverlapCardList = list; }

		/**
		* @brief 重なったカードのリスト取得
		* @return 重なったカードリスト
		*/
		std::list<COverlapCard*>& GetOverlapCardList() { return m_pOverlapCardList; }

	private:
		/**
		* @brief 重なっているカードのリスト
		*/
		std::list<COverlapCard*> m_pOverlapCardList;
	};
}

#endif //_OVERLAP_CARD_MANAGER_