//=============================================
//
//準備できているかの切り替えボタン[ready_button.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _READY_BUTTON_H_ //これが定義されてないとき

#define _READY_BUTTON_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	///準備できているかの切り替えボタン
	//=============================================
	class CReadyButton : public CObject2D
	{
	public:
		static const int PRIORITY = 25;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CReadyButton(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CReadyButton()override;
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
		static CReadyButton* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);

		CFontManager* GetFontManager() { return m_font_manager; }
	private:
		CFontManager* m_font_manager;
	};
}

#endif