//============================================================================================================
//
// エナジーUI枠 「energy_charge.cpp」
// Author : 笹生一颯
//
//============================================================================================================
#include "energy_charge.h"
#include "energy_gauge.h"

namespace
{

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
}

//======================================================================
// 更新
//======================================================================
void My::CEnergy_Charge::Update()
{
	//ローカル変数宣言
	float SpendTime = m_fTimeSpend;
	int ChargeTime = m_fChargeTime;

	#ifdef _DEBUG
	//インプット取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetKeyboard();

	//平常化
	if (pKeyboard->GetTrigger(DIK_1))
	{
		SpeedChange(FLOAT_ONE);
		TimeChange(180.0f);
		m_nEffectLv = INT_ZERO;
	}

	//速度：加速
	else if (pKeyboard->GetTrigger(DIK_2))
	{
		if (m_nEffectLv < 5) m_nEffectLv++;
		SpeedChange(FLOAT_ONE + 0.2f * m_nEffectLv);
	}

	//速度：減速
	else if (pKeyboard->GetTrigger(DIK_3))
	{
		if (m_nEffectLv < 5) m_nEffectLv++;
		SpeedChange(FLOAT_ONE - 0.1f * m_nEffectLv);
	}

	//速度：停止
	else if (pKeyboard->GetTrigger(DIK_4))
	{
		SpeedChange(FLOAT_ZERO);
	}

	//速度：減退
	else if (pKeyboard->GetTrigger(DIK_5))
	{
		SpeedChange(-FLOAT_ONE);
	}

	//必要時間：半減
	else if (pKeyboard->GetTrigger(DIK_6))
	{
		TimeChange(90.0f);
	}

	//必要時間：倍化
	else if (pKeyboard->GetTrigger(DIK_7))
	{
		TimeChange(360.0f);
	}

	#endif //_DEBUG

	//描画部分に今の状態を投げるところ
	if (m_fChargeSpeed > SPEED_DEF)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_SPDUP,m_nEffectLv);
	}
	else if (m_fChargeSpeed < SPEED_DEF && m_fChargeSpeed > FLOAT_ZERO)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_SPDDOWN,m_nEffectLv);
	}
	else if (m_fChargeSpeed == FLOAT_ZERO)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_STOP,5);
	}
	else if (m_fChargeSpeed <= FLOAT_ZERO)
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_BACKWARD,5);
	}
	else
	{
		CEnergy_Gauge::EffectSet(CEnergy_Gauge::EFFECT::EFFECT_NORMAL,5);
	}

	//速度分だけ充電
	m_fTimeSpend += m_fChargeSpeed;

	//充電完了したときやいろんな理由でタメが０以下になるとき
	if (m_fTimeSpend >= m_fChargeTime || m_fTimeSpend <= FLOAT_ZERO)
	{
		m_fTimeSpend = FLOAT_ZERO;
	}

	//充電割合を計算（受け渡し用）
	float Raito =  (SpendTime/ ChargeTime) *10;
	SetPolyCnt(Raito);
}

//======================================================================
// 回復に必要な時間の変更
//======================================================================
void My::CEnergy_Charge::TimeChange(float NewChargeTime)
{
	CEnergy_Charge* pGauge = GetInstance();

	pGauge->m_fChargeTime = NewChargeTime;
	pGauge->m_bEffect_T = true;
}


//======================================================================
//回復速度の変更
//======================================================================
void My::CEnergy_Charge::SpeedChange(float NewChargeSpeed)
{
	CEnergy_Charge* pGauge = GetInstance();

	pGauge->m_fChargeSpeed = NewChargeSpeed;
	pGauge->m_bEffect_S = true;
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