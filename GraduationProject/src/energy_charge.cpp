//============================================================================================================
//
// �G�i�W�[UI�g �uenergy_charge.cpp�v
// Author : �������D
//
//============================================================================================================
#include "energy_charge.h"
#include "energy_gauge.h"

namespace
{

}

//=====================================================================
//�ÓI�����o������
//=====================================================================
float My::CEnergy_Charge::m_fChargeRaito = FLOAT_ZERO;

//======================================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//======================================================================
My::CEnergy_Charge::~CEnergy_Charge()
{
}

//======================================================================
// ������
//======================================================================
HRESULT My::CEnergy_Charge::Init()
{
	return S_OK;
}
//======================================================================
// �I��
//======================================================================
void My::CEnergy_Charge::Uninit()
{
}

//======================================================================
// �X�V
//======================================================================
void My::CEnergy_Charge::Update()
{
	//���[�J���ϐ��錾
	float SpendTime = m_fTimeSpend;
	int ChargeTime = m_fChargeTime;

	#ifdef _DEBUG
	//�C���v�b�g�擾
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetKeyboard();

	//���퉻
	if (pKeyboard->GetTrigger(DIK_1))
	{
		SpeedChange(FLOAT_ONE);
		TimeChange(180.0f);
		m_nEffectLv = INT_ZERO;
	}

	//���x�F����
	else if (pKeyboard->GetTrigger(DIK_2))
	{
		if (m_nEffectLv < 5) m_nEffectLv++;
		SpeedChange(FLOAT_ONE + 0.2f * m_nEffectLv);
	}

	//���x�F����
	else if (pKeyboard->GetTrigger(DIK_3))
	{
		if (m_nEffectLv < 5) m_nEffectLv++;
		SpeedChange(FLOAT_ONE - 0.1f * m_nEffectLv);
	}

	//���x�F��~
	else if (pKeyboard->GetTrigger(DIK_4))
	{
		SpeedChange(FLOAT_ZERO);
	}

	//���x�F����
	else if (pKeyboard->GetTrigger(DIK_5))
	{
		SpeedChange(-FLOAT_ONE);
	}

	//�K�v���ԁF����
	else if (pKeyboard->GetTrigger(DIK_6))
	{
		TimeChange(90.0f);
	}

	//�K�v���ԁF�{��
	else if (pKeyboard->GetTrigger(DIK_7))
	{
		TimeChange(360.0f);
	}

	#endif //_DEBUG

	//�`�敔���ɍ��̏�Ԃ𓊂���Ƃ���
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

	//���x�������[�d
	m_fTimeSpend += m_fChargeSpeed;

	//�[�d���������Ƃ��₢���ȗ��R�Ń^�����O�ȉ��ɂȂ�Ƃ�
	if (m_fTimeSpend >= m_fChargeTime || m_fTimeSpend <= FLOAT_ZERO)
	{
		m_fTimeSpend = FLOAT_ZERO;
	}

	//�[�d�������v�Z�i�󂯓n���p�j
	float Raito =  (SpendTime/ ChargeTime) *10;
	SetPolyCnt(Raito);
}

//======================================================================
// �񕜂ɕK�v�Ȏ��Ԃ̕ύX
//======================================================================
void My::CEnergy_Charge::TimeChange(float NewChargeTime)
{
	CEnergy_Charge* pGauge = GetInstance();

	pGauge->m_fChargeTime = NewChargeTime;
	pGauge->m_bEffect_T = true;
}


//======================================================================
//�񕜑��x�̕ύX
//======================================================================
void My::CEnergy_Charge::SpeedChange(float NewChargeSpeed)
{
	CEnergy_Charge* pGauge = GetInstance();

	pGauge->m_fChargeSpeed = NewChargeSpeed;
	pGauge->m_bEffect_S = true;
}

//======================================================================
//����
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