//===========================================================================================================================================================
// 
// アシスト(デバフ:死)カード [card_assist_death.h]
// Author : 松田 永久
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_DEBUFF_DEATH_H_
#define _CARD_AST_DEBUFF_DEATH_H_
#include "main.h"
#include "card_assist_debuff.h"

namespace My
{
	class CCardAssist_Debuff_Death :public CCardAssist_Debuff
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CCardAssist_Debuff_Death(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCardAssist_Debuff_Death()override;

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
	private:
	};
}

#endif // !_CARD_AST_H_