//=============================================
//
//カード情報の背景[card_info_BG.cpp]
//Author Matsuda Towa
//
//=============================================
#include "card_info_BG.h"

//=============================================
// コンストラクタ
//=============================================
My::CCardInfoBG::CCardInfoBG(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCardInfoBG::~CCardInfoBG()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCardInfoBG::Init()
{
	SetColor({COLOR_BLACK.r, COLOR_BLACK.g, COLOR_BLACK.b, 0.5f});
	SetVtx();
	return S_OK;
}

//=============================================
//　終了
//=============================================
void My::CCardInfoBG::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCardInfoBG::Update()
{
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CCardInfoBG::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CCardInfoBG* My::CCardInfoBG::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CCardInfoBG* pBg = new CCardInfoBG;
	if(pBg == nullptr)return nullptr;

	pBg->SetPos(pos);
	pBg->SetSize(size);
	pBg->Init();
	return pBg;
}
