//=============================================
//
// カードの情報テキスト「card_info_UI.h」
// Author松田永久
//
//=============================================
#ifndef _CARD_INFO_UI_H_ //これが定義されてないとき
#define _CARD_INFO_UI_H_
#include "main.h"
#include "card.h"
#include "card_info_base.h"

namespace My
{
	//=============================================
	// カードの情報UI
	//=============================================
	class CCardInfoUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCardInfoUI();
		/**
		 * @brief デストラクタ
		 */
		~CCardInfoUI();
		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init();
		/**
		 * @brief 終了
		 */
		void Uninit();
		/**
		 * @brief 更新
		 */
		void Update();

		void SetCurrentUI(CCard* card);

		void SetisDraw(bool isDraw)
		{
			for (int i = 0; i < CCardInfoBase::MAX; ++i)
			{
				if (m_pCardInfo[i] == nullptr) { continue; }
				m_pCardInfo[i]->SetisDraw(isDraw);
			}
		}

	private:
		CCardInfoBase* m_pCardInfo[CCardInfoBase::MAX];
	};
}


#endif