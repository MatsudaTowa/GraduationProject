//=============================================
//
//2D三角形オブジェクト[object2D_trianglefan.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT2D_TRIANGLEFAN_H_ //これが定義されてないとき

#define _OBJECT2D_TRIANGLEFAN_H_
#include "main.h"
#include "object2D_triangle.h"

/** @brief My 名前空間 */
namespace My
{

	/** @brief 三角形ファン2Dクラス */
	class CObject2D_TriangleFan : public CObject2D_Triangle
	{
	public:
		static constexpr int NUM_VERTEX = 7;
		static constexpr int MAX_VERTEX = 100;

		/**
		 * @brief コンストラクタ
		 * @param [in]nPriority
		 */
		CObject2D_TriangleFan(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CObject2D_TriangleFan()override;

		/**
		 * @brief 初期化
		 * @return 成功したか
		 */
		HRESULT Init()override;

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
		 * @brief 頂点生成
		 */
		void SetVtx() override;

		/**
		* @brief 頂点位置設定
		*/
		void SetVtxPos();

		/**
		* @brief 頂点生成
		* @param [in]radius 半径
		* @param [in]partnum 分割数
		*/
		void SetVtx(float radius,float partnum);

		/**
		 * @brief triangleVtxの代入
		 * @param [in]triangleVtx
		 * @param [in]頂点番号
		 */
		inline void SetTriangleVtx(D3DXVECTOR2 triangleVtx, int idx)
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
		/**
		* @brief 中心座標
		*/
		D3DXVECTOR2 m_CenterPos;
		D3DXVECTOR2 m_OldCenterPos;

		/**
		* @brief 頂点座標
		*/
		D3DXVECTOR2 m_triangleVtx[MAX_VERTEX] = {};

		/**
		* 分割数
		*/
		int m_Divisionnum = 0;
	};
}
#endif