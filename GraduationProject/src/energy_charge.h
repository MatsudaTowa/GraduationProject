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
		* @brief [ 定数 ]
		* @param デフォルトのエナジーがたまるまでの時間
		*/
		 static constexpr float CHARGE_DEF = 180.0f ;

		/**
		* @brief [ 定数 ]
		* @param デフォルトのチャージ速度
		*/
		 static constexpr float SPEED_DEF = 1.0f;

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
		*/
		void Update();

		/**
		* @brief 回復に必要な時間の変更
		* @param [in]変更後の時間
		*/
		static void TimeChange(float NewChargeTime);

		/**
		* @brief 回復時間変動
		* @param [in]変更後の速度
		*/
		static void SpeedChange(float NewChargeSpeed);

		/**
		* @brief 充電何割か教えるやつ
		* @return 何割たまっているか（int型）
		*/
		inline static void SetPolyCnt(float Raito)
		{
			m_fChargeRaito = Raito;
		}
		inline static float GetPolyCnt()
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