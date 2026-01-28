//=============================================
//
//何Pかの表示UI[damage_number_UI.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COUNTDOWN_UI_H_ //これが定義されてないとき

#define _COUNTDOWN_UI_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//カウントダウンUIクラス
	//=============================================
	class CCountdownUI : public CObject2D
	{
	public:

		//定数
		static const int PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CCountdownUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CCountdownUI()override;
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
		 * @brief 寿命の設定
		 * @param [in]寿命
		 */
		virtual void SetLife(float life) { m_fLife = life; }

		/**
		 * @brief 生成
		 * @return
		 */
		float& GetLife() { return m_fLife; }

	private:
		
		//変数
		float m_fLife;
	};
}

#endif