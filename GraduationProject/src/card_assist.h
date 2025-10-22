//===========================================================================================================================================================
// 
// アシストカード [card_assist.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_H_
#define _CARD_AST_H_
#include "main.h"
#include "card.h"

namespace My
{
	class CCardAssist :public CCard
	{
		/**
		 * @brief コンストラクタ
		 */
		CCardAssist();

		/**
		 * @brief デストラクタ
		 */
		~CCardAssist()override;

		/**
		 * @brief 初期化
		 */
		HRESULT Init()override;

		/**
		 * @brief 終了
		 */
		void Uninit()override;

		/**
		 * @brief 更新
		 */
		void Update()override;

		/**
		 * @brief 描画
		 */
		void Draw()override;
	};
}

#endif // !_CARD_AST_H_