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

		inline void SetAssistType(AssistType assist_type) {  m_AssistType = assist_type; }
		
		/**
		 * @brief 固有情報読み込み
		 * @param [in]param
		 */
		void LoadUniqueInfo(CCard_Client::Param param) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuelCharacter* duel) override;

		/**
		 * @brief トリガー処理
		 */
		void Trigger() override;

	private:
		AssistType m_AssistType;
	};
}

#endif // !_CARD_AST_H_