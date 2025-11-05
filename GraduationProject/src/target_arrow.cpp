#include "target_arrow.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\TargetArrow.png";
}

My::CTargetArrow::CTargetArrow()
{
}

My::CTargetArrow::~CTargetArrow()
{
}

HRESULT My::CTargetArrow::Init()
{
	SetPos(VEC3_RESET_ZERO);
	SetSize({500, 500});

	//テクスチャ登録
	CTexture* pTexture = GET_TEXTURE;
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));	// テクスチャ設定

	//頂点設定
	SetVtx();

	CObject2D::Init();

	return S_OK;
}

void My::CTargetArrow::Uninit()
{
	CObject2D::Uninit();
}

void My::CTargetArrow::Update()
{
}

void My::CTargetArrow::Draw()
{
	CObject2D::Draw();
}
