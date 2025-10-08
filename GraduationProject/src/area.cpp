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
m_isSelect(false)
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

	if (!m_isSelect)
	{
		SetColor(DEFAULT_COLOR);
		return;
	}

	SetColor(COLOR_NONE);
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
