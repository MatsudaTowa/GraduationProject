//=============================================
//
//�G���A[area.cpp]
//Author Matsuda Towa
//
//=============================================
#include "area.h"

namespace
{
	const D3DXCOLOR DEFAULT_COLOR = { 0.0f,0.0f,0.0f,0.7f };
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CArea::CArea(int nPriority):CObject2D_Triangle(nPriority),
m_isSelect(false),
m_isActive(false),
m_pCharacter(nullptr)
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CArea::~CArea()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CArea::Init()
{
	CObject2D_Triangle::Init();
	SetColor(DEFAULT_COLOR);
	SetVtx();
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CArea::Uninit()
{
	CObject2D_Triangle::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CArea::Update()
{
	SetVtx();

	if (!m_isActive)
	{
		SetColor(COLOR_NONE);
		return;
	}

	if (!m_isSelect || m_pCharacter == nullptr)
	{
		SetColor(DEFAULT_COLOR);
		return;
	}

	SetColor(COLOR_NONE);

	//TODO:�����ɑI�����ꂽ�J�[�h�̏������I
	if (GET_INPUT_MOUSE->GetTrigger(0))
	{
		int life = m_pCharacter->GetLife();
		if (life > 0)
		{
			--life;
		}
		m_pCharacter->SetLife(life);
	}
}

//=============================================
// �`��
//=============================================
void My::CArea::Draw()
{
	CObject2D_Triangle::Draw();
}

//=============================================
// ����
//=============================================
My::CArea* My::CArea::Create(D3DXVECTOR2 triangle_vtx[NUM_VERTEX])
{
	CArea* area = new CArea;
	if (area == nullptr)
	{
		return nullptr;
	}
	for (int i = 0; i < NUM_VERTEX; ++i)
	{
		area->SetTriangleVtx(triangle_vtx[i], i);
	}
	area->Init();
	return area;
}
