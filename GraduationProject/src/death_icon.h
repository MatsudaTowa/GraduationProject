//=============================================
//
//準備できているかのテキスト表記[ready_txt.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DEATH_ICON_H_ //これが定義されてないとき

#define _DEATH_ICON_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//死亡時のアイコン
	//=============================================
	class CDeathIcon : public CObject2D
	{
	public:
		static const int PRIORITY = 28;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CDeathIcon(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CDeathIcon()override;
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
		static CDeathIcon* Create(D3DXVECTOR3 pos);
	private:
	};
}

#endif