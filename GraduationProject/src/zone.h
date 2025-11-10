//===========================================================================================================================================================
// 
// ゾーンのヘッダー [zone.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _ZONE_H_
#define _ZONE_H_

// include
#include "card.h"
#include <set>

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief ゾーンのクラス
	*/
	class CZone
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CZone();

		/**
		* @brief デストラクタ
		*/
		virtual ~CZone() = 0;

		/**
		* @brief 初期化処理
		* @return 成功したかどうか
		*/
		HRESULT Init();

		/**
		* @brief 終了処理
		*/
		void Uninit();

		/**
		* @brief 更新処理
		*/
		void Update();

		/**
		* @brief リストの取得処理
		* @return カードリスト
		*/
		const std::list<CCard*>GetList()const
		{
			return m_CardList;
		}

		/**
		* @brief リストの追加処理
		*/
		void AddCard(CCard* card);

		/**
		* @brief リストの削除処理
		*/
		bool DeleteCard(CCard* card);

	protected:
		/**
		* @brief 参照を返す関数
		*/
		std::list<CCard*>& GetListRef()
		{
			return m_CardList;
		}

		/**
		* @brief カードを表にする
		* @param [in]カードのポインタ
		*/
		void SetCardFaceUp(CCard* card)
		{
			if (card)
			{
				m_FaceCard.insert(card);
			}
		}

		/**
		* @brief カード情報取得
		*/
		const std::list<CCard*>& GetCards()const
		{
			return m_CardList;
		}

		/**
		* @brief カードを裏にする
		* @param [in]カードのポインタ
		*/
		void SetCardfaceDown(CCard* card)
		{
			m_FaceCard.erase(card);
		}

		/**
		* @brief 表か判定
		*/
		bool IsCardFaceUp(CCard* card)const
		{
			return m_FaceCard.count(card) > 0;
		}

	private:
		std::list<CCard*> m_CardList;	// カードリスト
		std::set<CCard*>m_FaceCard;		// 表にしたカード
	};
}
#endif