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
		~CZone();

		/**
		* @brief 初期化処理
		* @return 成功したかどうか
		*/
		HRESULT Init();

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

	private:
		std::list<CCard*> m_CardList;	// カードリスト
	};
}
#endif