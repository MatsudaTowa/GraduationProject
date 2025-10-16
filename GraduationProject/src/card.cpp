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
	m_IsChoice(false),
	m_outpos({0,0,0}),
	m_StateNum(CCardState::CARD_NEUTRAL)
{
	//if (m_pTop == nullptr)
	//{// top ���ݒ肳��Ă��Ȃ�������
	//	m_pPrev = nullptr;	// 
	//	m_pNext = nullptr;	// 
	//	m_pTop = this;		// �ŏ��̃|�C���^�Ƃ��Đݒ�
	//	m_pCur = this;		// �Ō���̃|�C���^�Ƃ��Đݒ�
	//}
	//else
	//{
	//	m_pNext = nullptr;			// ���̃|�C���^���k���ɂ��Ă���
	//	m_pPrev = m_pCur;			// ���̎��_�ł̍Ō����O�̃|�C���^�ɐݒ肷��
	//	m_pCur = this;				// �Ō�������g�ɐݒ肷��
	//	m_pPrev->m_pNext = this;	// �O�̃|�C���^�̎��̃|�C���^�Ɏ��g��ݒ肷��
	//}
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
	scale *= 1.0f;                                                                                                                                             

	SetScale(scale);

	m_pState = new CCardStateNeutral();
	m_pState->Init();
	m_StateNum = CCardState::CARD_NEUTRAL;

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
	//ChangeState(CCardState::CARD_NEUTRAL);

	// �J�����̈ʒu�Ɗp�x�ɍ��킹��
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//SetPos({ pCamera->GetPosV().x,pCamera->GetPosV().y - 100.0f,pCamera->GetPosV().z + 30.0f });

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += 0.3f;

	// TODO : �}�E�X���������悤�Ƃ��܂���

	//POINT mouse;
	//GetCursorPos(&mouse);
	//ScreenToClient(GET_HWND, &mouse);
	////D3DXVECTOR3 pos = GetPos();

	//D3DXVECTOR3 outpos;
	//float sX=(float)mouse.x, sY=(float)mouse.y;

	////�N���C�A���g�T�C�Y���擾
	//RECT rc;
	//GetClientRect(GET_HWND, &rc);
	//float width = (float)(rc.right - rc.left);
	//float height = (float)(rc.bottom - rc.top);
	//
	//D3DXMATRIX View, Proj;
	//pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);	// �r���[�s��̎擾
	//pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);	// ���e�s��̎擾

	////D3DXVECTOR3 pos = { GetPos().x,GetPos().z,GetPos().y };
	//D3DXVECTOR3 pos = GetPos();

	//CalcScreenToXZ(&m_outpos, pos, sX, sY, width, height, &View, &Proj);
	////CalcScreenToXZ(&m_outpos, GetPos(), mouse.x, mouse.y, SCREEN_WIDTH, SCREEN_WIDTH, &View, &Proj);

	//if (m_outpos.x >= -50 && m_outpos.x < 50)
	//{
	//	ChangeState(CCardState::CARD_PICKUP);
	//}

	//if (GET_COLISION->CheckColisionCircle({ sX,0.0f,sY}, 20.0f, pos).colision)
	//{
	//	ChangeState(CCardState::CARD_PICKUP);
	//}

	m_pState->Update(this);

	SetRot(rot);
}

//===========================================================================================================
// �`�揈��
//===========================================================================================================
void My::CCard::Draw()
{
	CObjectX::Draw();

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(GET_HWND, &mouse);
	float x = (float)mouse.x, y = (float)mouse.y;

	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\noutpos = %f , %f , %f \n"
		" cardpos = %f , %f , %f \n"
		" mousepos = %f , %f \n\n"
		" cardstate : %d\n\n\n\n\n\n"
		" [ ������@ ] \n"
		" < �� �� > �ŃJ�[�h�I�� \n"
		" < J > �I�������J�[�h���L���X�g \n"
		" < 1,2,3 > �ŃX�e�C��Ԃɂ��� \n"
		" ���Ԃ��o�߂�����J�[�h���g���K�[��������� \n",
		m_outpos.x, m_outpos.y, m_outpos.z,
		GetPos().x, GetPos().y, GetPos().z
		,x, y,
		m_StateNum);
	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
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
		// �����X�e�[�g�������ꍇ������ʂ�Ȃ�
		if (m_StateNum == state)
			return;

		// �폜
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

		case CCardState::CARD_TRIGGER:
			m_pState = new CCardStateTrigger();
			break;

		default:
			assert(1);
			break;
		}

		// �X�e�[�g�ԍ����ۑ����Ă���
		m_StateNum = state;

		// ������
		m_pState->Init(this);
		m_pState->Init();
	}
}



