//============================================================================================================
//
// �G�i�W�[UI�g �uenergy_gauge.cpp�v
// Author : �������D
//
//============================================================================================================
#include "energy_gauge.h"
#include "energy_charge.h"
#include "energy_frame.h"

namespace
{

}

//=====================================================================
//�ÓI�����o������
//=====================================================================
My::CEnergy_Gauge::EFFECT My::CEnergy_Gauge::m_Effect = My::CEnergy_Gauge::EFFECT::EFFECT_NORMAL;
int My::CEnergy_Gauge::m_nPolyCount = INT_ZERO;
int My::CEnergy_Gauge::m_nEnergyRaito = INT_ZERO;
int My::CEnergy_Gauge::m_nEffectLevel = INT_ZERO;

//=====================================================================
//�R���X�g���N�^
//=====================================================================
My::CEnergy_Gauge::CEnergy_Gauge(int nPriority) : CObject2D(nPriority),
m_nPolyNum(INT_ZERO),
m_bDisplay(false)
{
	m_nPolyCount++;
}
//=====================================================================
//�f�X�g���N�^
//=====================================================================
My::CEnergy_Gauge::~CEnergy_Gauge()
{
	m_nPolyCount--;
}

//=====================================================================
//������
//=====================================================================
HRESULT My::CEnergy_Gauge::Init()
{
	CObject2D::Init();

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();

	return S_OK;
}

//=====================================================================
//�I��
//=====================================================================
void My::CEnergy_Gauge::Uninit()
{
	CObject2D::Uninit();
}

//=====================================================================
//�X�V
//=====================================================================
void My::CEnergy_Gauge::Update()
{
	//���[�J���ϐ��錾
	float EffectColor = 1.0f - EFFECT_LV1 * m_nEffectLevel;;//���ʋ��x�ɂ��F�̕ϓ�

	//�����`���[�W����Ă邩�������Ă��炤
	m_nEnergyRaito = CEnergy_Charge::GetPolyCnt();

	//���x�̕ω��ŐF�ω�

	switch (m_Effect)
	{
	case EFFECT::EFFECT_NORMAL:
		SetColor({1.0f,1.0f,0.0f,1.0f});
		SetVtx();
		break;
	case EFFECT::EFFECT_SPDUP:
		SetColor({ 1.0f,EffectColor,0.0f,1.0f });
		SetVtx();
		break;
	case EFFECT::EFFECT_SPDDOWN:

		SetColor({ EffectColor,1.0f,0.0f,1.0f });
		SetVtx();
		break;
	case EFFECT::EFFECT_STOP:
		SetColor({ 0.0f,0.0f,1.0f,1.0f });
		SetVtx();
		break;
	case EFFECT::EFFECT_BACKWARD:
		SetColor({ 1.0f,0.0f,1.0f,1.0f });
		SetVtx();
		break;
	default:
		break;
	}

	if (m_nPolyNum < m_nEnergyRaito || m_nEnergyRaito >= 10)
	{
		m_bDisplay = true;
	}
	else
	{
		m_bDisplay = false;
	}
}

//=====================================================================
//�`��
//=====================================================================
void My::CEnergy_Gauge::Draw()
{
	if (m_bDisplay == true)
	{
		CObject2D::Draw();
	}
}

//=====================================================================
//����
//=====================================================================
My::CEnergy_Gauge* My::CEnergy_Gauge::Create(D3DXVECTOR3 pos)
{
	int PolyCnt = CEnergy_Gauge::GetPolyCnt();

	CEnergy_Gauge* pEnergy = new CEnergy_Gauge(11);
	pEnergy->SetPos(pos);
	pEnergy->SetSize({ 13.0f,23.0f });
	pEnergy->SetType(CObject::OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	pEnergy->m_nPolyNum = PolyCnt;
	return pEnergy;
}

//=====================================================================
//�܂Ƃ߂Đ���
//=====================================================================
void My::CEnergy_Gauge::CreateEnergy()
{
	CEnergy_Charge::Create();
	CEnergy_frame::Create({ 1150.0f, 580.0f,FLOAT_ZERO }, {130.0f,130.0f});
	for (int i = 0; i < 10; i++)
	{
		CEnergy_Gauge::Create({ 1039.0f+ (i * 28.0f),497.0f,FLOAT_ZERO });
	}
}

//=====================================================================
//
//=====================================================================

//=====================================================================
//
//=====================================================================