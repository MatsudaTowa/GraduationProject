//=============================================
//
//墓地の墓地ボタン[cemetery_button.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CEMETERY_BUTTON_H_ //これが定義されてないとき
#define _CEMETERY_BUTTON_H_
#include "main.h"
#include "button.h"

namespace My
{
	//=============================================
	///準備できているかの切り替えボタン
	//=============================================
	class CCemeteryButton : public CButton
	{
	public:
		static const int PRIORITY = 25;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCemeteryButton(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CCemeteryButton()override;
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
		void ButtonTrigger()override;

		/**
		 * @brief マウスの判定
		 * @return 当たっているか
		 */
		bool ProcessMouseEvent()override;

		CActiveSceneCharacter* GetCharacter()
		{
			return m_pCharacter;
		}

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]サイズ
		 * @return
		 */
		static CCemeteryButton* Create(D3DXVECTOR3 pos, CActiveSceneCharacter* pCharacter);
	private:
		CActiveSceneCharacter* m_pCharacter;
	};
}

#endif