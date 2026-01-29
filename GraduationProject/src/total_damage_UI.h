//=======================================================================================================--
// 
// 合計ダメージ表示UI [total_damage_UI.h]
// // Author Umeda Souma
// 
//=======================================================================================================--
#ifndef _TOTAL_DAMAGE_UI_H_ //これが定義されてないとき
#define _TOTAL_DAMAGE_UI_H_

#include "main.h"
#include "object2D.h"

namespace My
{
	class CTotalDamageUI : public CObject2D
	{
		public:
		static const int PRIORITY = 28;  //描画順
		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CTotalDamageUI(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CTotalDamageUI()override;
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
		static CTotalDamageUI* Create(D3DXVECTOR3 pos, int num);

	private:
		static const std::string TEX_NAME;
	};

}


#endif