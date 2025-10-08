//=============================================
//
//エリア[area.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _AREA_H_ //これが定義されてないとき

#define _AREA_H_
#include "main.h"
#include "object2D_triangle.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	 * @brief エリアクラス
	 * @copydoc CObject2D_Triangle
	 */
	class CArea : public CObject2D_Triangle
	{
	public:
		static constexpr int PRIORITY = 20;  //!<描画順

		/**
		 * @brief コンストラクタ
		 * @param プライオリティ
		 */
		CArea(int nPriority = PRIORITY);

		/**
		 * @brief デストラクタ
		 */
		~CArea()override;

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
		 * @param [in]頂点の位置
		 * @return エリアのポインタ
		 */
		static CArea* Create(D3DXVECTOR2 triangle_vtx[NUM_VERTEX]);

		/**
		 * @brief 選択しているか取得
		 * @return 
		 */
		inline bool GetSelect()
		{
			return m_isSelect;
		}

		/**
		 * @brief 選択されているか設定
		 * @param [in]isSelect
		 */
		inline void SetSelect(bool isSelect)
		{
			m_isSelect = isSelect;
		}
	private:
		bool m_isSelect; //選択されているか
	};
}

#endif