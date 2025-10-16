//================================
//
//area_manager.cppに必要な宣言[area_manager.cpp]
//Author:松田永久
// 
//================================
#include "area_manager.h"

//================================
// コンストラクタ
//================================
My::CAreaManager::CAreaManager():m_isActive(false),
m_pArea(),
m_pCenterArea()
{
}

//================================
// デストラクタ
//================================
My::CAreaManager::~CAreaManager()
{
}

//================================
// 初期化
//================================
HRESULT My::CAreaManager::Init()
{
	return S_OK;
}

//================================
// 終了
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
//エリア生成
//=============================================
void My::CAreaManager::CreateArea()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] != nullptr) { continue; }

		//三角形の頂点座標を指定
		D3DXVECTOR2 triangle_vtx[CObject2D_Triangle::NUM_VERTEX];

		//ウィンドウの中心にを必ず頂点に
		D3DXVECTOR2 center = { SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF };
		triangle_vtx[1] = center;

		//それぞれの頂点位置を指定
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
//エリア選択
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
		{//選択されているエリア以外の
			if (i == CInputMouse::AREA::CENTER)
			{//真ん中だったら
				m_pCenterArea->SetSelect(false);
			}
			else
			{//三角形だったら
				m_pArea[i]->SetSelect(false);
			}
		}
	}
}
