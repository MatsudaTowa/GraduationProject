//=============================================
//
//カード情報[zone_num_UI_BG.cpp]
//Author Matsuda Towa
//
//=============================================
#include "zone_num_UI_BG.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\zone_num_UI_BG.png";
}

//=============================================
// コンストラクタ
//=============================================
My::CZoneNumUI_BG::CZoneNumUI_BG(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CZoneNumUI_BG::~CZoneNumUI_BG()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CZoneNumUI_BG::Init()
{
	CObject2D::Init();
	SetColor(COLOR_WHITE);
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CZoneNumUI_BG::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CZoneNumUI_BG::Update()
{
	CObject2D::Update();
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CZoneNumUI_BG::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CZoneNumUI_BG* My::CZoneNumUI_BG::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CZoneNumUI_BG* pObject = new CZoneNumUI_BG;

	// nullならnullを返す
	if (pObject == nullptr) { return nullptr; }

	//テクスチャ取得
	CTexture* pTexture = GET_TEXTURE;

	pObject->SetPos(pos); //pos設定
	pObject->SetSize(size); //サイズ設定
	pObject->BindTexture(pTexture->GetAddress(pTexture->Regist(TEX_NAME))); //テクスチャ設定
	pObject->Init(); //初期化処理

	return pObject;
}
