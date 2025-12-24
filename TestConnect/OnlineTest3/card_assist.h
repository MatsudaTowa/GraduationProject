//===========================================================================================================================================================
// 
// アシストカード [card_assist.h]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#ifndef _CARD_AST_H_
#define _CARD_AST_H_

//ヘッダーのインクルード
#include "card.h"

namespace My
{
	//カードクライアントクラス
	class CCardAssist : public CCard
	{
	public:

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

		inline CCard_Client::AssistType GetAssistType() { return m_AssistType; }

		inline void SetAssistType(CCard_Client::AssistType assist_type) {  m_AssistType = assist_type; }
		
		/**
		 * @brief 固有情報読み込み
		 * @param [in]param
		 */
		void LoadUniqueInfo(CCard_Client::Param param) override;

		/**
		 * @brief キャスト処理
		 */
		void Cast(CDuel_Player* player) override;

		/**
		 * @brief ステイ処理
		 */
		void Stay() override {}

		/**
		 * @brief トリガー処理
		 */
		void Trigger() override;

		//キャストカードの読み込み
		bool LoadCastInfo(RakNet::BitStream* bsin, CastDestination destination) override;

		//キャストカードの書き出し
		void SendCastInfo(RakNet::BitStream* bsout) override {}

	private:
		CCard_Client::AssistType m_AssistType;
	};
}

#endif // !_CARD_AST_H_