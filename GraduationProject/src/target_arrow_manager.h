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
		void Regist(CTargetArrow*pTA);

		/**
		* @brief 登録
		* @param [in]ターゲットアローのポインタ
		*/
		void Remove();

		/**
		* @brief ターゲットアローリストの取得処理
		* @param [out]ターゲットアローリストのポインタ
		*/
		std::list<CTargetArrow*> GetTargetArrow();

	private:
		/**
		* @brief ターゲットアローのリスト
		*/
		std::list<CTargetArrow*>m_pTargetArrowList;

	};
}
#endif // !TARGET_ARROW_MANAGER_H_