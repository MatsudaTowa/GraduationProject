//===========================================================================================
// 
// キャストされたカードのトータルダメージ [total_damage_UI.cpp]
// // Author Umeda Souma
// 
//===========================================================================================
#include "total_damage_UI.h"

//宣言
const std::string My::CTotalDamageUI::TEX_NAME = "data\\TEXTURE\\number.png";

My::CTotalDamageUI::CTotalDamageUI(int nPriority)
	:CObject2D(nPriority)
{
}

My::CTotalDamageUI::~CTotalDamageUI()
{
}

HRESULT My::CTotalDamageUI::Init()
{
	SetSize({ 50,50 });
	SetColor(COLOR_WHITE);
	SetDivision(10);
	SetVtx();

	return S_OK;
}

void My::CTotalDamageUI::Uninit()
{
	CObject2D::Uninit();
}

void My::CTotalDamageUI::Update()
{
	SetTexVtx();
}

void My::CTotalDamageUI::Draw()
{
	CObject2D::Draw();
}

My::CTotalDamageUI* My::CTotalDamageUI::Create(D3DXVECTOR3 pos, int num)
{
	CTotalDamageUI* pTotalDamageUI = new CTotalDamageUI();
	pTotalDamageUI->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(CTotalDamageUI::TEX_NAME)));
	pTotalDamageUI->SetPos(pos);
	pTotalDamageUI->Init();
	pTotalDamageUI->SetTexPos({ num * 0.1f, 1.0f });

	return pTotalDamageUI;
}