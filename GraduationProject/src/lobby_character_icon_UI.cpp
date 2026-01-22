//===================================================================================
// 
// ロビーのキャラクターアイコンUI [ lobby_character_icon_UI.cpp ]
// Author : Umeda Souma
// 
//===================================================================================
#include "lobby_character_icon_UI.h"

//===================================================================================
// コンストラクタ
//===================================================================================
My::CLobbyCharacterIconUI::CLobbyCharacterIconUI(int nPriority):
	m_Idx(0)
{
}

//===================================================================================
// デストラクタ
//===================================================================================
My::CLobbyCharacterIconUI::~CLobbyCharacterIconUI()
{
}

//===================================================================================
// 初期化処理
//===================================================================================
HRESULT My::CLobbyCharacterIconUI::Init()
{
	// テクスチャの登録
	BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist("data/TEXTURE/lobby_character_icon.png")));

	// サイズの設定
	SetSize(D3DXVECTOR2(160.0f, 120.0f));

	// 頂点座標の設定
	SetVtx();

	return S_OK;
}

//===================================================================================
// 終了処理
//===================================================================================
void My::CLobbyCharacterIconUI::Uninit()
{
	CObject2D::Uninit();
}

//===================================================================================
// 更新処理
//===================================================================================
void My::CLobbyCharacterIconUI::Update()
{
	CObject2D::Update();
}

//===================================================================================
// 描画処理
//===================================================================================
void My::CLobbyCharacterIconUI::Draw()
{
	CObject2D::Draw();
}

My::CLobbyCharacterIconUI* My::CLobbyCharacterIconUI::Create(D3DXVECTOR3 pos, D3DXCOLOR color)
{
	CLobbyCharacterIconUI* pInstance = new CLobbyCharacterIconUI();
	pInstance->SetPos(pos);

	// 色の設定
	pInstance->SetColor(color);

	pInstance->Init();
	return pInstance;
}
