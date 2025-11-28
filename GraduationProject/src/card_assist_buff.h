//===========================================================================================================================================================
// 
// アシスト(バフ)カード [card_assist_buff.h]
// Author : 松田 永久
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_BUFF_H_
#define _CARD_AST_BUFF_H_
#include "main.h"
#include "card_assist.h"

namespace My
{
	class CCardAssist_Buff :public CCardAssist
	{
	public:

		enum BuffType
		{// バフの種類 TODO:継承によって変わるかも
			NONE_BUFF,
			HEAL,                // HP回復
			NO_NAMEONE,            // 未定①
			NO_NAMETWO,            // 未定②
		};

		/**
		 * @brief コンストラクタ
		 */
		CCardAssist_Buff(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CCardAssist_Buff()override;

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

		inline BuffType GetBuffType() { return m_BuffType; }

		inline void SetBuffType(BuffType buff_type) { m_BuffType = buff_type; }

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
		BuffType m_BuffType;
	};
}

#endif // !_CARD_AST_H_