//===========================================================================================================================================================
// 
// ��D�̏��� [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card.h"
#include "card_state.h"
#include "game.h"

My::CHand::CHand() :
	m_SelectNum(-1),
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
	// �����o�ϐ�������
	m_SelectNum = -1;
	m_TotalNum = 0;
	m_IsPassStart = false;

	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}

	// �J�����̈ʒu�Ɗp�x�ɍ��킹��
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
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

	// �X�^�[�g���̃J�[�h�̖������h���[����
	HandDraw(START_HANDS);

	// �X�^�[�g��ʂ�����A���̎����ł̓X�^�[�g��ʂ�Ȃ�
	m_IsPassStart = true;
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void My::CHand::Update()
{
	// �L�[�{�[�h�擾
	CInputKeyboard* pkeyboad = CManager::GetInstance()->GetKeyboard();

	// ��D�h���[
	if (pkeyboad->GetTrigger(DIK_SPACE))
	{
		HandDraw(1);
	}

	// ��D�I��
	Select();
}

//===========================================================================================================
// �I��
//===========================================================================================================
void My::CHand::Select()
{
	// �L�[�{�[�h�擾
	// TODO : �}�E�XDE����
	CInputKeyboard* pKeyboad = CManager::GetInstance()->GetKeyboard();

	// �L�[�{�[�h�őI��
	if (pKeyboad->GetTrigger(DIK_RIGHTARROW))
	{// �E�I��

		m_SelectNum++;
		if (m_SelectNum >= m_TotalNum)
		{
			m_SelectNum = 0;
		}
	}
	if (pKeyboad->GetTrigger(DIK_LEFTARROW))
	{// ���I��

		m_SelectNum--;
		if (m_SelectNum < 0)
		{
			m_SelectNum = m_TotalNum - 1;
		}
	}

	// ���ׂẴJ�[�h��I�΂�Ă��Ȃ���Ԃɂ���
	for (int i = 0; i < m_TotalNum; i++)
	{// �������Ă��閇����
		if (m_pCard[i] != nullptr)
		{
			m_pCard[i]->ChangeState(new CCardStateNeutral);
		}
	}

	if (m_SelectNum <= -1)
		return;

	// �I�𒆂̃J�[�h�̃X�e�[�g��ς���
	if (m_pCard[m_SelectNum] != nullptr)
	{
		m_pCard[m_SelectNum]->ChangeState(new CCardStatePickup);
	}		
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

//===========================================================================================================
// ����
//===========================================================================================================
My::CHand* My::CHand::Create()
{
	CHand* pHand = new CHand();

	pHand->Init();

	return pHand;
}

//===========================================================================================================
// ��D�̃J�[�h�̍��W�ݒ�
//===========================================================================================================
void My::CHand::SetHandCardPos()
{
	D3DXVECTOR3 firstpos;	// ��ԍ����̈ʒu(��D�̍ŏ��̈ʒu)
	float posInterbal = 20.0f - (20 * m_TotalNum * 0.07f);	// ��D�ɕ\������Ă��鎞�̃J�[�h�̊Ԋu
	float xpos;	// �ꖇ�ڂ̃J�[�h��x���W

	// x���W�̐ݒ� = �Z���^�[ - (���݂̍��v���� * �J�[�h�̊Ԋu�̔���)
	xpos = m_CenterPos.x - (m_TotalNum* posInterbal*0.5f);

	for (int i = 0; i <= m_TotalNum; i++)
	{
		// �J�[�h�̍��W�̐ݒ�
		if (i != 0)
		{// �ꖇ�ڈȊO�͑O�̎�D�̈ʒu���Q�Ƃ��� "Interbal" �����ɂ��炷
			m_pCard[i]->SetPos({ m_pCard[i - 1]->GetPos().x+posInterbal, m_pCard[i - 1]->GetPos().y, m_pCard[i - 1]->GetPos().z });
		}
		else
		{
			// �ꖇ�ڂ̃J�[�h�̍��W(��ƂȂ�)
			firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };
			m_pCard[0]->SetPos(firstpos);
		}
		
		// �Ԋu���J����
		xpos += posInterbal*0.5f;
	}
}
