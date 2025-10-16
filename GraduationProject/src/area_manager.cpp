//================================
//
//area_manager.cpp�ɕK�v�Ȑ錾[area_manager.cpp]
//Author:���c�i�v
// 
//================================
#include "area_manager.h"

//================================
// �R���X�g���N�^
//================================
My::CAreaManager::CAreaManager():m_isActive(false),
m_pArea(),
m_pCenterArea()
{
}

//================================
// �f�X�g���N�^
//================================
My::CAreaManager::~CAreaManager()
{
}

//================================
// ������
//================================
HRESULT My::CAreaManager::Init()
{
	return S_OK;
}

//================================
// �I��
//================================
void My::CAreaManager::Uninit()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] == nullptr) { continue; }
		m_pArea[i]->Uninit();
		m_pArea[i] = nullptr;
	}

	if (m_pCenterArea == nullptr) { return; }
	m_pCenterArea->Uninit();
	m_pCenterArea = nullptr;
}


//=============================================
//�G���A����
//=============================================
void My::CAreaManager::CreateArea()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] != nullptr) { continue; }

		//�O�p�`�̒��_���W���w��
		D3DXVECTOR2 triangle_vtx[CObject2D_Triangle::NUM_VERTEX];

		//�E�B���h�E�̒��S�ɂ�K�����_��
		D3DXVECTOR2 center = { SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF };
		triangle_vtx[1] = center;

		//���ꂼ��̒��_�ʒu���w��
		switch (i)
		{
		case CInputMouse::AREA::UP:
			triangle_vtx[0] = { SCREEN_WIDTH,FLOAT_ZERO };
			triangle_vtx[2] = { VEC2_RESET_ZERO };
			break;
		case CInputMouse::AREA::DOWN:
			triangle_vtx[0] = { FLOAT_ZERO,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::LEFT:
			triangle_vtx[0] = { FLOAT_ZERO,FLOAT_ZERO };
			triangle_vtx[2] = { FLOAT_ZERO,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::RIGHT:
			triangle_vtx[0] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,FLOAT_ZERO };
			break;
		default:
			break;
		}

		m_pArea[i] = CArea::Create(triangle_vtx);
	}

	if (m_pCenterArea != nullptr) { return; }
	m_pCenterArea = CCenterArea::Create();
	m_pCenterArea->Init();
}

//=============================================
//�G���A�I��
//=============================================
void My::CAreaManager::SelectArea()
{
	CInputMouse::AREA area = GET_INPUT_MOUSE->GetArea();

	if (area == CInputMouse::AREA::CENTER)
	{
		m_pCenterArea->SetSelect(true);
	}
	else
	{
		m_pArea[area]->SetSelect(true);	
	}

	for (int i = 0; i < CInputMouse::AREA::MAX; ++i)
	{
		if (i != area)
		{//�I������Ă���G���A�ȊO��
			if (i == CInputMouse::AREA::CENTER)
			{//�^�񒆂�������
				m_pCenterArea->SetSelect(false);
			}
			else
			{//�O�p�`��������
				m_pArea[i]->SetSelect(false);
			}
		}
	}
}
