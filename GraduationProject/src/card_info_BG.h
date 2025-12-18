//=============================================
//
//カード情報の背景[card_info_BG.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CARD_INFO_BG_H_ //これが定義されてないとき

#define _CARD_INFO_BG_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//カード情報の背景
	//=============================================
	class CCardInfoBG : public CObject2D
	{
	public:
		static const int PRIORITY = 27;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCardInfoBG(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CCardInfoBG()override;
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
		static CCardInfoBG* Create(D3DXVECTOR3 pos,D3DXVECTOR2 size);
	private:
	};
}

#endif