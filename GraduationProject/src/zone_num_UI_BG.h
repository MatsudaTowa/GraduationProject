//=============================================
//
//カード情報[zone_num_UI_BG.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ZONE_NUM_UI_BG_H_ //これが定義されてないとき

#define _ZONE_NUM_UI_BG_H_
#include "main.h"
#include "object2D.h"
#include "card_info_base.h"
namespace My
{
	//=============================================
	//カード情報の背景
	//=============================================
	class CZoneNumUI_BG : public CObject2D
	{
	public:
		static const int BG_PRIORITY = 8;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CZoneNumUI_BG(int nPriority = BG_PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CZoneNumUI_BG()override;
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
		 * @return 背景のポインタ
		 */
		static CZoneNumUI_BG* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	private:
	};
}

#endif