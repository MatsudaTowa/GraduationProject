//=============================================
//
//タイトルのUI[title_Screen.cpp]
//Author Matsuda Towa
//
//=============================================
#include "title_screen.h"
#include "manager.h"
#include "object.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\002.png";
	const D3DXVECTOR2 SCREEN_SIZE = {512.0f,256.0f };
}

//=============================================
//コンストラクタ
//=============================================
My::CTitle_Screen::CTitle_Screen(int nPriority):CObject2D(nPriority)
{
}

//=============================================
//デストラクタ
//=============================================
My::CTitle_Screen::~CTitle_Screen()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CTitle_Screen::Init()
{
	//親クラスの初期化を呼ぶ
	CObject2D::Init();

	//テクスチャ座標設定
	SetTexPos(VEC2_RESET_ONE);

	//カラー取得
	D3DXCOLOR col = GetColor();

	col = COLOR_WHITE;

	//カラーを代入
	SetColor(col);

	SetSize(SCREEN_SIZE);

	//頂点設定
	SetVtx();
	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CTitle_Screen::Uninit()
{
	//親クラスの終了呼ぶ
	CObject2D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CTitle_Screen::Update()
{
	//親クラスの更新呼ぶ
	CObject2D::Update();
}

//=============================================
//描画
//=============================================
void My::CTitle_Screen::Draw()
{
	//親クラスの描画呼ぶ
	CObject2D::Draw();
}

//=============================================
//生成
//=============================================
My::CTitle_Screen* My::CTitle_Screen::Create(D3DXVECTOR3 pos)
{
	CTexture* pTexture = GET_TEXTURE;
	CTitle_Screen* pTitle_Screen = new CTitle_Screen;

	//nullならnullを返す
	if (pTitle_Screen == nullptr) {return nullptr;}

	pTitle_Screen->SetPos(pos); //pos設定

	pTitle_Screen->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));

	pTitle_Screen->SetType(OBJECT_TYPE_TITLE); //タイプ設定

	pTitle_Screen->Init();

	return pTitle_Screen;
}
