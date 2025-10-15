//============================================================================================================
//
// エナジーUI枠 「energy_gauge.h」
// Author : 笹生一颯
//
//============================================================================================================

#ifndef _ENERGY_GAUGE_H_
#define _ENERGY_GAUGE_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class CEnergy_Gauge : public CObject2D
	{
	public:
		/**
		* @brief [列挙型] 状態
		* @param 通常(1)
		* @param 加速(2)
		* @param 減速(3)
		* @param 停止(4)
		* @param 減退(5)
		*/
		enum class EFFECT
		{
			NONE = 0,
			EFFECT_NORMAL,
			EFFECT_SPDUP,
			EFFECT_SPDDOWN,
			EFFECT_STOP,
			EFFECT_BACKWARD,
			EFFECT_MAX
		};

		/**
		* @brief [定数]
		* @param エフェクトレベル１の時の色の濃さ
		*/
		static constexpr float EFFECT_LV1 = 0.2f;

		/**
		* @brief コンストラクタ
		* @param [in]プライオリティ
		*/
		CEnergy_Gauge(int nPriority);

		/**
		* @brief デストラクタ
		*/
		~CEnergy_Gauge()override;

		/**
		* @brief 初期化
		* @return 成功したかどうか
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
		* @return ゲージ部分のポリゴンのポインタ
		*/
		static CEnergy_Gauge* Create(D3DXVECTOR3 pos);

		/**
		* @brief ゲージに使うポリゴンをまとめて生成
		*/
		static void CreateEnergy();

		/**
		* @brief 状態設定
		* @param [in]どんな状態
		* @param [in]効果強度（最大５）
		*/
		inline static void EffectSet(EFFECT AnyEffect,int Level)
		{
			m_Effect = AnyEffect;
			m_nEffectLevel = Level;
		}

	private:
		/**
		* @brief カウントを取得する処理
		* @return 何番目のやつか
		*/
		inline static int GetPolyCnt()
		{
			return m_nPolyCount;
		}

		/**
		* @brief 今どんな状態？
		*/
		static EFFECT m_Effect;

		/**
		* @brief ゲージのポリゴンのカウンター
		*/
		static int m_nPolyCount;

		/**
		* @brief チャージされたエナジー今何割？（取得必要）
		*/
		static int m_nEnergyRaito;

		/**
		* @brief 効果強度（最大５）
		*/
		static int m_nEffectLevel;

		/**
		* @brief 全体で何番目か
		*/
		int m_nPolyNum;

		/**
		* @brief 描画していいゲージ？
		*/
		bool m_bDisplay;


	};
}

#endif