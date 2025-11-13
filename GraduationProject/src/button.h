//=============================================
//
// ボタン[button.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _BUTTON_H_ //これが定義されてないとき

#define _BUTTON_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	///ボタン
	//=============================================
	class CButton : public CObject2D
	{
	public:
		static const int PRIORITY = 25;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CButton(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CButton()override;
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
		 * @brief ボタンが押された時の処理
		 */
		virtual void ButtonTrigger() = 0;

		/**
		 * @brief マウスの判定
		 * @return 当たっているか
		 */
		virtual bool ProcessMouseEvent();
	private:
	};
}

#endif