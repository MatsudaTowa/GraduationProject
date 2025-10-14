//===========================================================================================================================================================
// 
// �J�[�h�̏��� [card.cpp]
// Author : �~�c�@�u�^
// 
//===========================================================================================================================================================
#include "card.h"
#include "game.h"
#include "card_state.h"
#include <typeinfo>

//===========================================================================================================
// �R���X�g���N�^
//===========================================================================================================
My::CCard::CCard(int nPriority):CObjectX(nPriority),
	m_pState(nullptr),
	m_IsChoice(false)
{
}

//===========================================================================================================
// �f�X�g���N�^
//===========================================================================================================
My::CCard::~CCard()
{
}

//===========================================================================================================
// ����������
//===========================================================================================================
HRESULT My::CCard::Init()
{
	// ���f���ݒ�
	CModel*pModel= CManager::GetInstance()->GetModel();
	int Idx = pModel->Regist("data\\model\\card_traial_002.x");
	CModel::MODEL_INFO mi = pModel->GetModelInfo(Idx);
	BindXFile(mi.pBuffMat, mi.dwNumMat, mi.pMesh);

	CObjectX::Init();

	// �X�P�[��
	D3DXVECTOR3 scale = GetScale();

	scale.x *= 1.2f;
	scale *= 0.8f;                                                                                                                                             

	SetScale(scale);

	m_pState = new CCardStateNeutral();
	m_pState->Init();

	return S_OK;
}

//===========================================================================================================
// �I������
//===========================================================================================================
void My::CCard::Uninit()
{
	CObjectX::Uninit();
}

//===========================================================================================================
// �X�V����
//===========================================================================================================
void My::CCard::Update()
{
	// �J�����̈ʒu�Ɗp�x�ɍ��킹��
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	//SetPos({ pCamera->GetPosV().x,pCamera->GetPosV().y - 100.0f,pCamera->GetPosV().z + 30.0f });

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += 0.3f;

	m_pState->Update(this);

	SetRot(rot);
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void My::CCard::Draw()
{
	CObjectX::Draw();
}

//===========================================================================================================
// ��������
//===========================================================================================================
My::CCard* My::CCard::Create()
{
	CCard* pCard = new CCard(5);
	pCard->Init();

	return pCard;
}

//===========================================================================================================
// �X�e�[�g��ύX����
//===========================================================================================================
void My::CCard::ChangeState(CCardState* state)
{
	if (m_pState != nullptr)
	{
		//// �^���擾
		//const type_info& cur_state_id = typeid(m_pState);	// ���݂̃X�e�[�g��ID
		//const type_info& next_state_id = typeid(state);		// ���̃X�e�[�g��ID

		//const char* cur_state_name = cur_state_id.name();
		//const char* next_state_name = next_state_id.name();

		delete m_pState;
		m_pState = state;

		m_pState->Init(this);
		m_pState->Init();
	}
}

//===========================================================================================================
// �X�e�[�g��ύX����
//===========================================================================================================
void My::CCard::ChangeState(CCardState::CARD_STATE state)
{
	if (m_pState != nullptr)
	{
		if (m_StateNum == state)
			return;

		delete m_pState;
		m_pState = nullptr;

		switch (state)
		{
		case CCardState::CARD_NEUTRAL:
			m_pState = new CCardStateNeutral();
			break;

		case CCardState::CARD_PICKUP:
			m_pState = new CCardStatePickup();
			break;

		case CCardState::CARD_CAST:
			m_pState = new CCardStateCast();
			break;

		case CCardState::CARD_STAY:
			m_pState = new CCardStateStay();
			break;

		case CCardState::CARD_PLAY:
			m_pState = new CCardStatePlay();
			break;

		default:
			assert(1);
			break;
		}

		m_StateNum = state;

		m_pState->Init(this);
		m_pState->Init();
	}
}



