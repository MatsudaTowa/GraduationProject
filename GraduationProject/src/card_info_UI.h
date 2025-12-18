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
#include "card_info_BG.h"

namespace My
{
	class CCardInfoBG;
	//=============================================
	// カードの情報UI
	//=============================================
	class CCardInfoUI
	{
	public:
		enum UI_TYPE
		{
			BG = 0,
			COST,
			STATES,
			NAME,
			ILLUST,
			TEXT,
			MAX
		};

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
			if (m_pBG != nullptr)
			{
				m_pBG->SetisDraw(isDraw);
			}
		}

	private:
		CCardInfoBG* m_pBG;
	};
}


#endif