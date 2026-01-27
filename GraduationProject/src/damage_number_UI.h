//=============================================
//
//何Pかの表示UI[damage_number_UI.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DAMAGE_NUMBER_UI_H_ //これが定義されてないとき

#define _DAMAGE_NUMBER_UI_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//ダメージ数UIクラス
	//=============================================
	class CDamageNumberUI : public CObject2D
	{
	public:
		static const int PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CDamageNumberUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CDamageNumberUI()override;
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
		 * @param [in]数
		 * @return
		 */
		static CDamageNumberUI* Create(D3DXVECTOR3 pos, int num);
	private:
		static constexpr float SIZE = 30.0f;
		static const std::string TEX_NAME;
		float m_fLife;
		float m_num;
	};
}

#endif