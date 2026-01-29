#include "deck_num_UI_bg.h"

namespace
{
	const D3DXVECTOR2 POLY_SIZE = { 40.0f,55.0f };
	const std::string TEX_NAME = "data\\TEXTURE\\cardframe\\reverse_test.png";
}
My::CDeckNumUI_BG::CDeckNumUI_BG(int nPriority):CObject2D(nPriority)
{
}

My::CDeckNumUI_BG::~CDeckNumUI_BG()
{
}

HRESULT My::CDeckNumUI_BG::Init()
{
	CObject2D::Init();
	SetColor(COLOR_WHITE);
	SetSize(POLY_SIZE);
	BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_NAME)));
	SetVtx();
	return S_OK;
}

void My::CDeckNumUI_BG::Uninit()
{
	CObject2D::Uninit();
}

void My::CDeckNumUI_BG::Update()
{
	SetVtx();
}

void My::CDeckNumUI_BG::Draw()
{
	CObject2D::Draw();
}

My::CDeckNumUI_BG* My::CDeckNumUI_BG::Create(D3DXVECTOR3 pos)
{
	CDeckNumUI_BG* pObj = new CDeckNumUI_BG;
	pObj->SetPos(pos);
	pObj->Init();
	return pObj;
}
