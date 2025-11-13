//=====================================================================================
//
// 体力表示用ポリゴン「life_gauge.h」
// Author : 笹生一颯
//
//=====================================================================================
#ifndef _LIFE_GAUGE_H_
#define _LIFE_GAUGE_H_
#include "main.h"
#include "object2D_trapezoid.h"

namespace My
{
	class CLife_Gauge : public CObject2D_Trapezoid
	{
	public:

		/**
		* @brief コンストラクタ
		* @param [in]プライオリティ(int)
		*/
		CLife_Gauge(int nPriority);

		/**
		* @brief デストラクタ
		*/
		~CLife_Gauge()override;

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
		* @brief 生成（台形）
		* @param [in]位置(D3DXVECTOR3)
		* @param [in]半径(float)
		* @param [in]分割数(int)
		* @param [in]何番目のものか(int)
		* @return 体力のポリゴンのポインタ
		*/
		static CLife_Gauge* Create_S(D3DXVECTOR3 Pos, float Rad, int Segment, int Num);

		/**
		* @brief 体力関係まとめて生成
		* @param [in]このゲームでの最大HP(float)
		* @param [in]HPゲージの半径
		*/
		static void CreateLifeUI(float GameHP, float Rad);

		/**
		* @brief 頂点情報生成（台形）
		*/
		void SetVtx_S();

	private:
		/**
		* @brief 台形用変数まとめ
		* @param 円状配置用の半径(float)
		* @param 分割予定数(int)
		* @param これが何番目のものか(int)
		*/

		/**
		* @brief 円状配置のための半径
		*/
		float m_fTrapeRad;

		/**
		* @brief 分割数
		*/
		int m_nTrapeSeg;

		/**
		* @brief これは何番目？
		*/
		int m_nTrapeSegNum;

		/**
		* @brief 何番目までOK？
		*/
		int m_nDrawSegNum;

	};
}

#endif // !_ENERGY_FRAME_H_
