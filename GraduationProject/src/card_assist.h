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
	public:

		enum AssistType
		{// アシストの種類
			NONE_ASSIST,        // アシスト以外のときはこれ
			OBSTRUCT,            // 妨害
			BUFF,                // バフ
			DEBUFF                // デバフ
		};

		enum BuffType
		{// バフの種類 TODO:継承によって変わるかも
			NONE_BUFF,
			HEAL,                // HP回復
			NO_NAMEONE,            // 未定①
			NO_NAMETWO,            // 未定②
		};

		enum DebuffType
		{// デバフの種類 TODO:継承によって変わるかも
			NONE_DEBUFF,
			DEATH,                // 死亡
		};

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
		CCardAssist(int nPriority);

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

		void LoadCardData() override;

		inline AssistType GetAssistType() { return m_AssistType; }
		inline BuffType GetBuffType() { return m_BuffType; }
		inline DebuffType GetDebuffType() { return m_DebuffType; }
		inline HealType GetHealType() { return m_HealType; }

		inline void SetAssistType(AssistType assist_type) {  m_AssistType = assist_type; }
		inline void SetBuffType(BuffType buff_type) {  m_BuffType = buff_type; }
		inline void SetDebuffType(DebuffType debuff_type) { m_DebuffType = debuff_type; }
		inline void SetHealType(HealType heal_type) { m_HealType = heal_type; }
	private:
		AssistType m_AssistType;
		BuffType m_BuffType;
		DebuffType m_DebuffType;
		HealType m_HealType;
	};
}

#endif // !_CARD_AST_H_