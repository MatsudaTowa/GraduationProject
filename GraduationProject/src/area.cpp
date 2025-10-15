//=============================================
//
//エリア[area.cpp]
//Author Matsuda Towa
//
//=============================================
#include "area.h"

namespace
{
	const D3DXCOLOR DEFAULT_COLOR = { 0.0f,0.0f,0.0f,0.7f };
}

//=============================================
// コンストラクタ
//=============================================
My::CArea::CArea(int nPriority):CObject2D_Triangle(nPriority),
m_isSelect(false),
m_isActive(false),
m_pCharacter(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CArea::~CArea()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CArea::Init()
{
	CObject2D_Triangle::Init();
	SetColor(DEFAULT_COLOR);
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CArea::Uninit()
{
	CObject2D_Triangle::Uninit();
}

//=============================================
// 更新
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

	//TODO:ここに選択されたカードの処理を！
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
// 描画
//=============================================
void My::CArea::Draw()
{
	CObject2D_Triangle::Draw();
}

//=============================================
// 生成
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
