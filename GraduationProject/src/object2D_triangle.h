//=============================================
//
//2D三角形オブジェクト[object2D_triangle.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT2D_TRIANGLE_H_ //これが定義されてないとき

#define _OBJECT2D_TRIANGLE_H_
#include "main.h"
#include "object2D.h"

/** @brief My 名前空間 */
namespace My
{

	/** @brief アニメーション2Dクラス */
	class CObject2D_Triangle : public CObject2D
	{
	public:
		static constexpr int NUM_VERTEX = 3;

		/**
		 * @brief コンストラクタ
		 * @param [in]nPriority
		 */
		CObject2D_Triangle(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CObject2D_Triangle()override;

		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init()override;

		/**
		 * @brief 更新 
		 */
		void Update()override;

		/**
		 * @brief 描画
		 */
		void Draw()override;

		/**
		 * @brief 頂点生成
		 */
		void SetVtx() override;

		/**
		 * @brief triangleVtxの代入
		 * @param [in]triangleVtx
		 * @param [in]頂点番号
		 */
		inline void SetTriangleVtx(D3DXVECTOR2 triangleVtx,int idx)
		{
			m_triangleVtx[idx] = triangleVtx;
		}

		/**
		 * @brief triangleVtxの取得
		 * @param [in]頂点番号
		 * @return triangleVtx
		 */
		inline D3DXVECTOR2 GetTriangleVtx(int idx)
		{
			return m_triangleVtx[idx];
		}
	private:
		D3DXVECTOR2 m_triangleVtx[NUM_VERTEX] = {};
	};
}
#endif