//=============================================
//
//準備できているかのテキスト表記[ready_txt.cpp]
//Author Matsuda Towa
//
//=============================================
#include "death_icon.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\death_icon.png";
	const float POLY_SIZE = 45.0f;
}

//=============================================
// コンストラクタ
//=============================================
My::CDeathIcon::CDeathIcon(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CDeathIcon::~CDeathIcon()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CDeathIcon::Init()
{
	SetColor(COLOR_WHITE);
	SetSize({ POLY_SIZE,POLY_SIZE });

	//テクスチャ読み込み
	BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_NAME)));
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CDeathIcon::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CDeathIcon::Update()
{
	SetVtx();
	CObject2D::Update();
}

//=============================================
// 描画
//=============================================
void My::CDeathIcon::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CDeathIcon* My::CDeathIcon::Create(D3DXVECTOR3 pos)
{
	CDeathIcon* pIcon = new CDeathIcon;
	if (pIcon == nullptr) { return nullptr; }

	pIcon->SetPos(pos);
	pIcon->Init();
	return pIcon;
}
