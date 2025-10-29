//=============================================
//
//2D台形オブジェクト[object2D.h]
//Author 笹生一颯
//
//=============================================
#ifndef _OBJECT2D_TRAPEZOID_H_ //これが定義されてないとき

#define _OBJECT2D_TRAPEZOID_H_
#include "main.h"
#include "object2d.h"

namespace My
{
	/** @brief 2Dオブジェクトクラス*/
	class CObject2D_Trapezoid : public CObject2D
	{
	public:
		static constexpr int NUM_VERTEX = 4;

		/**
		 * @brief コンストラクタ
		 * @param [in]nPriority
		 */
		CObject2D_Trapezoid(int nPriority);

		/**
		 * @brief デストラクタ
		 */
		~CObject2D_Trapezoid()override;

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
		 * @brief 台形用の頂点生成
		 * @param [in]位置
		 * @param [in]大きさ
		 * @param [in]色
		 * @param [in]半径（円状配置用）
		 * @param [in]角度（円状配置用）
		 */
		void SetTrapeVtx(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col,float Angle);

		/**
		 * @brief 台形用の頂点生成
		 * @param [in]中心点
		 * @param [in]色
		 * @param [in]内半径
		 * @param [in]外半径
		 * @param [in]開始角度
		 * @param [in]終了角度
		 */
		void SetTrapeVtx_Radial(D3DXVECTOR3 pos, D3DXCOLOR col, float Rad_In, float Rad_Out, float Angle_Start, float Angle_End);

		/**
		* @brief 台形用色変更（互換性無しのため）
		* @param [in]色
		*/
		void ChangeColor(D3DXCOLOR col);
	};
}

#endif