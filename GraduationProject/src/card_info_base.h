//=============================================
//
//カード情報の背景[card_info_BG.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CARD_INFO_BASE_H_ //これが定義されてないとき

#define _CARD_INFO_BASE_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class CCard;
	//=============================================
	//カード情報の基底 TODO:フレームと多分処理一緒だから
	//=============================================
	class CCardInfoBase : public CObject2D
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
		 * @param [in]プライオリティ
		 */
		CCardInfoBase(int nPriority);
		/**
		 * @brief デストラクタ
		 */
		~CCardInfoBase()override;
		/**
		 * @brief 初期化
		 * @return 成功したか
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
		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @return
		 */
		static CCardInfoBase* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, UI_TYPE type);

		virtual void SetUI() = 0;

		CCard* GetCard() { return m_pSelectCard; }
		void SetCard(CCard* select_card) { m_pSelectCard = select_card; }
	private:
		CCard* m_pSelectCard;
	};
}

#endif