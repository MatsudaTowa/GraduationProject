//=============================================
//
//カード情報の背景[deck_num_UI_BG.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DECK_NUM_UI_BG_H_ //これが定義されてないとき
#define _DECK_NUM_UI_BG_H_
#include "main.h"
#include "object2D.h"
namespace My
{
	//=============================================
	//カード情報の背景
	//=============================================
	class CDeckNumUI_BG : public CObject2D
	{
	public:
		static const int BG_PRIORITY = 5;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CDeckNumUI_BG(int nPriority = BG_PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CDeckNumUI_BG()override;
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
		 * @return
		 */
		static CDeckNumUI_BG* Create(D3DXVECTOR3 pos);
	private:
	};
}

#endif