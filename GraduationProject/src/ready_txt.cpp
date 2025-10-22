//=============================================
//
//レディのテキスト表記[ready_txt.cpp]
//Author Matsuda Towa
//
//=============================================
#include "ready_txt.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\leady.png";
}

//=============================================
// コンストラクタ
//=============================================
My::CReadyTxt::CReadyTxt(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CReadyTxt::~CReadyTxt()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CReadyTxt::Init()
{
	CObject2D::Init();
	SetColor(COLOR_WHITE);

	//テクスチャ読み込み
	BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(&TEX_NAME)));
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CReadyTxt::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CReadyTxt::Update()
{
	CObject2D::Update();
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CReadyTxt::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CReadyTxt* My::CReadyTxt::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CReadyTxt* pTxt = new CReadyTxt;
	if(pTxt == nullptr)	{ return nullptr; }
	pTxt->SetPos(pos);
	pTxt->SetSize(size);
	pTxt->Init();
	return pTxt;
}
