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
		*/
		static void CreateLifeUI(float GameHP);

		/**
		* @brief 頂点情報生成（台形）
		*/
		void SetVtx_S();

		/**
		* @brief ダメージ
		* @param [in]ダメージ量(float)
		*/
		static void ApplyDamage(float Damage)
		{
			//反映
			m_fLife -= Damage;
		}

		/**
		* @brief 回復
		* @param [in]回復量(float)
		*/
		static void ApplyHeal(float Heal)
		{
			//反映
			m_fLife += Heal;
		}

		/**
		* @brief 最大体力『入力』（多分開始時以外触らない）
		* @param [in]回復量(float)
		*/
		static void DefMax(float MaxHP)
		{
			//反映
			m_fLife_Max = MaxHP;
			m_fLife = MaxHP;
		}

		/**
		* @brief 最大体力『上昇』
		* @param [in]回復量(float)
		*/
		static void IncreaseMax(float IncMax)
		{
			//反映
			m_fLife_Max += IncMax;
		}

		/**
		* @brief 最大体力『減少』(最大を超過した時の調整つき)
		* @param [in]回復量(float)
		*/
		static void DecreaseMax(float DecMax)
		{
			//反映
			m_fLife_Max = DecMax;

			//調整
			if (m_fLife >= m_fLife_Max)
			{
				m_fLife = m_fLife_Max;
			}
		}

		/**
		* @brief 体力割合取得（ゲージ変化目的）
		* @return 現在の体力の割合 
		*/

		static float GetLifeRaito()
		{
			return m_fRaito;
		}

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
		* @brief 数値系の変数まとめ
		* @param 体力(float/static)
		* @param 最大体力(float/static)
		* @param 現在HPの割合(float)
		* @param 何番目まで描画していいか(int)
		*/

		/**
		* @brief 現在の体力
		*/
		static float m_fLife;

		/**
		* @brief 最大体力
		*/
		static float m_fLife_Max;

		/**
		* @brief 現HP割合
		*/
		static float m_fRaito;

		/**
		* @brief 何番目までOK？
		*/
		int m_nDrawSegNum;

	};
}

#endif // !_ENERGY_FRAME_H_
