//=============================================
//
//カードを選択出来る範囲[selsection_range.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _SELECTION_RANGE_H_ //これが定義されてないとき

#define _SELECTION_RANGE_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	//=============================================
	//カードを選択出来る範囲
	//=============================================
	class CSelectionRange : public CObject2D
	{
	public:
		static const int PRIORITY = 10;  //描画順

		/**
		 * @brief コンストラクタ
		 * @param [in]プライオリティ
		 */
		CSelectionRange(int nPriority = PRIORITY);
		/**
		 * @brief デストラクタ
		 */
		~CSelectionRange()override;
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
		static CSelectionRange* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	private:
	};
}

#endif