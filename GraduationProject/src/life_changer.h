//============================================================================================================
//
// 体力UIの変動・管理・取得用 「life_changer.h」
// Author : 笹生一颯
//
//============================================================================================================
#ifndef _LIFE_CHANGER_H_
#define _LIFE_CHANGER_H_

//===================================================
// ヘッダーファイルインクルード
//===================================================
#include "main.h"
#include "manager.h"

//===================================================
// クラス定義
//===================================================
namespace My
{
	class CLife_changer
	{
	public:

		/**
		* @brief コンストラクタ
		*/
		CLife_changer();

		/**
		* @brief デストラクタ
		*/
		~CLife_changer();

		/**
		* @brief 初期化
		* @return 成功したか
		*/
		HRESULT Init();

		/**
		* @brief 終了
		*/
		void Uninit();

		/**
		* @brief 更新
		*/
		void Update();

		/**
		* @brief ダメージ（0未満にはしない）
		* @param [in]ダメージ量(float)
		*/
		void ApplyDamage(float Damage)
		{
			//反映
			m_fLife -= Damage;
			if (m_fLife <= FLOAT_ZERO)
			{
				m_fLife = FLOAT_ZERO;
			}
		}

		/**
		* @brief 回復（最大値超過対策済み）
		* @param [in]回復量(float)
		*/
		void ApplyHeal(float Heal)
		{
			//反映
			m_fLife += Heal;
			if (m_fLife >= m_fLife_Max)
			{//超過したら最大値に合わせる
				m_fLife = m_fLife_Max;
			}
		}

		/**
		* @brief 最大体力『入力』（多分開始時以外触らない）
		* @param [in]回復量(float)
		*/
		void DefMax(float MaxHP)
		{
			//反映
			m_fLife_Max = MaxHP;
			m_fLife = MaxHP;
		}

		/**
		* @brief 最大体力『上昇』(現在HPは上昇しない)
		* @param [in]回復量(float)
		*/
		void IncreaseMax(float IncMax)
		{
			//反映
			m_fLife_Max += IncMax;
		}

		/**
		* @brief 最大体力『減少』(最大を超過した時の調整つき)
		* @param [in]回復量(float)
		*/
		void DecreaseMax(float DecMax)
		{
			//反映
			m_fLife_Max -= DecMax;

			//調整
			if (m_fLife > m_fLife_Max)
			{
				m_fLife = m_fLife_Max;
			}
		}

		/**
		* @brief 体力割合取得（ゲージ変化目的）
		* @return 現在の体力の割合
		*/

		float GetLifeRaito()
		{
			return m_fRaito;
		}

		/**
		* @brief 自身のインスタンスを生成取得
		*/
		static CLife_changer* GetInstance()
		{
			static CLife_changer Ins;
			return &Ins;
		}

		/**
		* @brief 自身のインスタンスを生成取得
		*/
		static CLife_changer* Create();

	private:
		/**
		* @brief 現在の体力
		*/
		float m_fLife;

		/**
		* @brief 最大体力
		*/
		float m_fLife_Max;

		/**
		* @brief 現HP割合
		*/
		float m_fRaito;
	};
}


#endif // !_LIIFE_FRAME_H_
