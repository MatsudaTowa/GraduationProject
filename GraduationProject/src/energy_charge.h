//============================================================================================================
//
// エナジーUI枠 「energy_charge.h」
// Author : 笹生一颯
//
//============================================================================================================

#ifndef _ENERGY_CHARGE_H_
#define _ENERGY_CHARGE_H_
#include "main.h"

namespace My
{
	class CEnergy_Charge
	{
	public:
		/**
		* @brief コンストラクタ
		* @param [in]プライオリティ
		*/
		CEnergy_Charge();

		/**
		* @brief デストラクタ
		*/
		~CEnergy_Charge();
		
		/**
		* @brief 初期化
		* @return 成功したかどうか
		*/
		HRESULT Init();
		
		/**
		* @brief 終了
		*/
		void Uninit();
		
		/**
		* @brief 更新
		* @param [in] 経過した時間
		* @param [in] チャージにかかる時間
		*/
		void Update(float spend_time,float charge_time);

		/**
		* @brief 回復に必要な時間の変更
		* @param [in]変更後の時間
		*/
		void TimeChange(float NewChargeTime);

		/**
		* @brief 回復時間変動
		* @param [in]変更後の速度
		*/
		void SpeedChange(float NewChargeSpeed);

		/**
		* @brief 充電何割か教えるやつ
		* @return 何割たまっているか（int型）
		*/
		void SetPolyCnt(float Raito)
		{
			m_fChargeRaito = Raito;
		}
		float GetPolyCnt()
		{
			return m_fChargeRaito;
		}

		/**
		* @brief 自身のインスタンスを生成取得
		*/
		static CEnergy_Charge* GetInstance()
		{
			static CEnergy_Charge Ins;
			return &Ins;
		}

		/**
		* @brief 生成
		*/
		static CEnergy_Charge* Create();

	private:
		/**
		* @brief 充電割合(渡す用)
		*/
		static float m_fChargeRaito;

		/**
		* @brief 経過した時間
		*/
		float m_fTimeSpend;	

		/**
		* @brief エナジーの回復に必要な時間
		*/
		float m_fChargeTime;		
		
		/**
		* @brief エナジーの回復速度
		*/
		float m_fChargeSpeed;		
		
		/**
		* @brief 効果強度
		*/
		int m_nEffectLv;

		/**
		* @brief 効果中かどうか（必要時間）
		*/
		bool m_bEffect_T;			
		
		/**
		* @brief 効果中かどうか（速度）
		*/
		bool m_bEffect_S;	

	};
}

#endif