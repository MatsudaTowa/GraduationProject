//===========================================================================================================================================================
// 
// キャストプレビューゾーンのヘッダー [castpreview_zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _CASTPREVIEW_ZONE_H_
#define _CASTPREVIEW_ZONE_H_

// include
#include "zone.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief キャストプレビューゾーンクラス
	*/
	class CCastPreviewZone : public CZone
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		CCastPreviewZone();

		/**
		* @brief デストラクタ
		*/
		~CCastPreviewZone();

		/**
		* @brief 使用したカードを送る
		* @param [in]カードのポインタ
		*/
		void UsePlayerCard(CCard* pCard);

		/*
		* @brief 重ねたカードを設定
		* @param [in]重ねたカード
		*/
		void SetOverlapCard(CCard* pCard) { m_OverlapCard.push_back(pCard); }

		/*
		* @brief 重ねたカードリストを設定
		* @param [in]重ねたカードリスト
		*/
		void SetOverlapCardList(const std::list<std::vector<CCard*>>& overlapCardList) { m_OverlapCardList = overlapCardList; }

		/**
		* @brief カードプレビューゾーンで重ねたカードを取得
		* @return 重ねたカードのリスト
		*/
		std::vector<CCard*>& GetOverlapCard(){ return m_OverlapCard; }

		/*
		* @brief カードプレビューゾーンで重ねたカードリストを取得
		* @return 重ねたカードリスト
		*/
		std::list<std::vector<CCard*>>& GetOverlapCardList() { return m_OverlapCardList; }

	private:

		/**
		* @brief カードプレビューゾーンで重ねたカード
		*/
		std::vector<CCard*> m_OverlapCard;

		/**
		* @brief 重ねたカードのリスト
		*/
		std::list<std::vector<CCard*>> m_OverlapCardList;
	};
}
#endif