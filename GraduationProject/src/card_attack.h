//===========================================================================================================================================================
// 
// アタックカード [card_attack.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_ATK_H_
#define _CARD_ATK_H_
#include "main.h"
#include "card.h"

namespace My
{
	class CCardAttack :public CCard
	{
		/**
		 * @brief コンストラクタ
		 */
		CCardAttack();

		/**
		 * @brief デストラクタ
		 */
		~CCardAttack()override;

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

#endif // !_CARD_ATK_H_