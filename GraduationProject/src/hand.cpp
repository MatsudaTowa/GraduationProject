//===========================================================================================================================================================
// 
// ��D�̏��� [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card.h"
#include "game.h"

My::CHand::CHand() :
	m_SelectNum(0),
	m_TotalNum(0),
	m_IsPassStart(false)
{
	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}
}

My::CHand::~CHand()
{
}

//===========================================================================================================
// ����������
//===========================================================================================================
void My::CHand::Init()
{
	m_SelectNum = 0;
	m_TotalNum = 0;
	m_IsPassStart = false;

	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}
	// �J�����̈ʒu�Ɗp�x�ɍ��킹��
	CCamera* pCamera = CManager::GetInstance()->GetCamera(CGame::GAME_CAMERA);
	m_CenterPos = { pCamera->GetPosV().x, pCamera->GetPosV().y - 100.0f, pCamera->GetPosV().z + 30.0f };
}

//===========================================================================================================
// �J�n������
//===========================================================================================================
void My::CHand::Start()
{
	// ��x�ʂ�����ʂ�Ȃ�
	if (m_IsPassStart)
		return;

	HandDraw(START_HANDS);
	m_IsPassStart = true;
}

//===========================================================================================================
// �I��
//===========================================================================================================
void My::CHand::Select()
{
}

//===========================================================================================================
// ��D������
//===========================================================================================================
void My::CHand::HandDraw(int drawnum)
{
	/*
	* @brief ��D�����Ă�J�[�h�̖����𒴂�����h���[�����Ȃ�
	* TODO : �����̏����͒������Ƃ���MAX�̖����ɂ��邩�A�����Ȃ��悤�ɂ��邩����
	*/
	if (m_TotalNum >= MAX_HANDSCARD)
		return;	

	// TODO : ����̎����R�D�������A�R�D�̑��������炷
	
	for (int i = 0; i < drawnum; i++)
	{
		m_pCard[m_TotalNum] = CCard::Create();
		SetHandCardPos();
		m_TotalNum++;	// ��D�̑����𑝂₷
	}
}

My::CHand* My::CHand::Create()
{
	CHand* pHand = new CHand();

	pHand->Init();

	return pHand;
}

void My::CHand::SetHandCardPos()
{
	float posInterbal = 20.0f;


	D3DXVECTOR3 firstpos;	// ��ԍ����̈ʒu(��D�̍ŏ��̈ʒu)
	float xpos;
	xpos = m_CenterPos.x - (m_TotalNum* posInterbal);
	int a = 0;

	if (m_TotalNum == 0)
	{
		a = 0;
	}
	else
	{
		a = 1;
	}

	for (int i = 0; i <= m_TotalNum; i++)
	{
		firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };

		if (i != 0)
		{
			m_pCard[i]->SetPos({ m_pCard[i - 1]->GetPos().x+posInterbal, m_pCard[i - 1]->GetPos().y, m_pCard[i - 1]->GetPos().z });
		}
		else
		{
			m_pCard[0]->SetPos({ firstpos.x + (a * (posInterbal * -0.5f)), firstpos.y, firstpos.z });
		}
		
		xpos += posInterbal;
	}
}
