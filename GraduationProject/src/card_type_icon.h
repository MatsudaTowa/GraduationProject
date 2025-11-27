//=============================================
//
//カードのタイプアイコン[card_type_icon]
//Author Matsuda Towa
//
//=============================================
#ifndef _CARD_TYPE_ICON_H_ //これが定義されてないとき

#define _CARD_TYPE_ICON_H_
#include "main.h"
#include "object2D.h"
#include "card.h"

namespace My
{
	//=============================================
	//タイプアイコン
	//=============================================
	class CCardTypeIcon : public CObject2D
	{
	public:
		static const int PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCardTypeIcon(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CCardTypeIcon()override;
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
		 * @param [in]カードタイプ
		 * @return
		 */
		static CCardTypeIcon* Create(D3DXVECTOR3 pos, CCard::CARDTYPE_ type);
	private:
	};
}

#endif