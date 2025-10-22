//===========================================================================================================================================================
// 
// ディフェンスカード [card_deffence.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_DEF_H_
#define _CARD_DEF_H_
#include "main.h"
#include "card.h"

namespace My
{
	class CCardDeffence :public CCard
	{
		/**
		 * @brief コンストラクタ
		 */
		CCardDeffence();

		/**
		 * @brief デストラクタ
		 */
		~CCardDeffence()override;

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

#endif // !_CARD_DF_H_