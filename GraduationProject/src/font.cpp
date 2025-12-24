//=============================================
//
// フォント表示 [font.cpp]
// Author: Matsuda Towa
//
//=============================================
#include "font.h"
#include "font_texture_manager.h"
#include <tchar.h>
#include <string>


//=============================================
// コンストラクタ
//=============================================
My::CFont::CFont(int nPriority) : CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CFont::~CFont()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CFont::Init()
{
	SetVtx();

	return S_OK;
}

void My::CFont::Uninit()
{
	CObject2D::Uninit();
}

void My::CFont::Update()
{
	SetVtx();
}

void My::CFont::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	LPDIRECT3DTEXTURE9 tex = GetTexture();

	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CFont* My::CFont::Create(D3DXVECTOR3 pos, float size, int thickness, int idx, WCHAR txt)
{
	CFont* pFont = new CFont;

	if (pFont == nullptr) return nullptr;

	pFont->SetPos(pos);
	pFont->SetSize({ size, size });
	pFont->SetType(OBJECT_TYPE_FONT);

	CFontTextureManager::FontTextureStuts key = CFontTextureManager::GetInstance()->Regist(idx, txt, (int)size, thickness);

	// テクスチャをセット
	LPDIRECT3DTEXTURE9 tex = CFontTextureManager::GetInstance()->GetAddress(key);
	pFont->BindTexture(tex);

	pFont->Init();
	return pFont;
}
