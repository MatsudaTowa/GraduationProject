//===========================================================================================================================================================
// 
// target_arrow_manager.cppのヘッダー [target_arrow_manager.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _TARGET_ARROW_MANAGER_H_
#define _TARGET_ARROW_MANAGER_H_
#include "main.h"

namespace My
{
	class CTargetArrow;
	class CCard;

	class CTargetArrowManager
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CTargetArrowManager();

		/**
		* @brief デストラクタ
		*/
		~CTargetArrowManager();

		/**
		* @brief 登録
		* @param [in]ターゲットアローのポインタ
		*/
		void Regist(CTargetArrow*pTA, CCard* card);

		/**
		* @brief 登録
		* @param [in]ターゲットアローのポインタ
		*/
		void Remove();

		/**
		* @brief カードリストからの削除
		* @param [in]カード
		*/
		void CardRemove(CCard* card);

		/**
		* @brief ターゲットアローリストの取得処理
		* @param [out]ターゲットアローリストのポインタ
		*/
		std::list<CTargetArrow*> GetTargetArrow();

		void SetOverlappedPos(CTargetArrow* itr, CTargetArrow* ptr);

	private:
		/**
		* @brief ターゲットアローのリスト
		*/
		std::list<CTargetArrow*>m_pTargetArrowList;

		/**
		* @brief arrowが重なっているかどうか
		*/
		bool m_IsOverlapped;
	};
}
#endif // !TARGET_ARROW_MANAGER_H_