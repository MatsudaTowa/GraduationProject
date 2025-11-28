//===========================================================================================================================================================
// 
// アシスト(デバフ)カード [card_assist.h]
// Author : 松田 永久
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_DEBUFF_H_
#define _CARD_AST_DEBUFF_H_
#include "main.h"
#include "card_assist.h"

namespace My
{
	class CCardAssist_Debuff :public CCardAssist
	{
	public:
		enum DebuffType
		{// デバフの種類 TODO:継承によって変わるかも
			NONE_DEBUFF,
			DEATH,                // 死亡
		};

		/**
		 * @brief コンストラクタ
		 */
		CCardAssist_Debuff(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCardAssist_Debuff()override;

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

		inline DebuffType GetDebuffType() { return m_DebuffType; }

		inline void SetDebuffType(DebuffType debuff_type) { m_DebuffType = debuff_type; }

		/**
		 * @brief 固有情報読み込み
		 * @param [in]param
		 */
		void LoadUniqueInfo(CCard_Client::Param param) override;

		/**
		 * @brief キャストしたかの確認
		 */
		bool IsCast(CDuelCharacter* duel) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuelCharacter* duel) override;

		/**
		 * @brief ステイ処理
		 */
		void Stay() override {}

		/**
		 * @brief トリガー処理
		 */
		void Trigger() override;

	private:
		DebuffType m_DebuffType;
	};
}

#endif // !_CARD_AST_H_