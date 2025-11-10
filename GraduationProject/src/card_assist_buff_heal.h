//===========================================================================================================================================================
// 
// アシスト(バフ:回復)カード [card_assist_heal.h]
// Author : 松田 永久
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_BUFF_HEAL_H_
#define _CARD_AST_BUFF_HEAL_H_
#include "main.h"
#include "card_assist_buff.h"

namespace My
{
	class CCardAssist_Buff_Heal :public CCardAssist_Buff
	{
	public:
		enum HealType
		{// 回復の種類 TODO:継承によって変わるかも
			NONE_HEAL,                    // 回復しない
			ALL_HEAL,                    // 全体回復
			SPECIFIC_HEAL,                // 特定の相手を選んで回復
			RANDOM_HEAL,                // ランダム回復
			SELFINTARGET_HEAL,            // 自分を含めてランダム回復
			ONLY_ME,                    // 自分だけ
		};

		/**
		 * @brief コンストラクタ
		 */
		CCardAssist_Buff_Heal(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCardAssist_Buff_Heal()override;

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

		inline HealType GetHealType() { return m_HealType; }
		inline int GetHeal() { return m_HealType; }

		inline void SetHealType(HealType heal_type) { m_HealType = heal_type; }
		inline void SetHeal(int heal) { m_nHeal = heal; }
	private:
		HealType m_HealType;
		int m_nHeal;
	};
}

#endif // !_CARD_AST_H_