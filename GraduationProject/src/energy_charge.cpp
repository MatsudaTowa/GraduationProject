//============================================================================================================
//
// エナジーたまり具合の表示 「energy_charge.cpp」
// Author : 笹生一颯
//
//============================================================================================================
#include "energy_charge.h"
#include "energy_gauge.h"
#include "active_scene_player_state.h"

namespace
{
	constexpr float CHARGE_DEF = 180.0f;//デフォルトのエナジーがたまるまでの時間
	constexpr float SPEED_DEF = 1.0f;//デフォルトのチャージ速度
}

//=====================================================================
//静的メンバ初期化
//=====================================================================
float My::CEnergy_Charge::m_fChargeRaito = FLOAT_ZERO;

//======================================================================
// コンストラクタ
//======================================================================
My::CEnergy_Charge::CEnergy_Charge():
	m_fChargeSpeed(SPEED_DEF),
	m_fChargeTime(CHARGE_DEF),
	m_fTimeSpend(FLOAT_ZERO),
	m_bEffect_S(false),
	m_bEffect_T(false),
	m_nEffectLv(INT_ZERO)
{
}
//======================================================================
// デストラクタ
//======================================================================
My::CEnergy_Charge::~CEnergy_Charge()
{
}

//======================================================================
// 初期化
//======================================================================
HRESULT My::CEnergy_Charge::Init()
{
	return S_OK;
}
//======================================================================
// 終了
//======================================================================
void My::CEnergy_Charge::Uninit()
{
	m_fTimeSpend = FLOAT_ZERO;
	m_nEffectLv = INT_ZERO;
	m_fChargeSpeed = SPEED_DEF;
	m_fChargeTime = CHARGE_DEF;
}

//======================================================================
// 更新
//======================================================================
void My::CEnergy_Charge::Update(float spend_time, int charge_time)
{
	//ローカル変数宣言
	float SpendTime = spend_time;
	int ChargeTime = charge_time;

	//floatの絶対値で差を算出
	float EffectLv = 1.0f - std::fabs(ChargeTime - CPlayerDuelState::ENERGY_UP_FRAME) / CPlayerDuelState::ENERGY_UP_FRAME;

	//描画部分に今の状態を投げるところ
	if (ChargeTime < CPlayerDuelState::ENERGY_UP_FRAME)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_SPDUP, EffectLv);
	}
	else if (ChargeTime > CPlayerDuelState::ENERGY_UP_FRAME)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_SPDDOWN, EffectLv);
	}
	else
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_NORMAL, 5);
	}
	/*else if (m_fChargeSpeed == FLOAT_ZERO)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_STOP,5);
	}
	else if (m_fChargeSpeed <= FLOAT_ZERO)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_BACKWARD,5);
	}*/

	//充電割合を計算（受け渡し用）
	float Raito =  (SpendTime/ ChargeTime) *10;
	SetPolyCnt(Raito);
}

//======================================================================
//生成
//======================================================================
My::CEnergy_Charge* My::CEnergy_Charge::Create()
{
	CEnergy_Charge* Charge = nullptr;
	Charge = new CEnergy_Charge();
	Charge->Init();
	return Charge;
}
//---------------------------------------------------------------------------------------

//======================================================================
//
//======================================================================