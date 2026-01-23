//===================================================================================
// 
// ロビーのキャラクターアイコンUI [ lobby_character_icon_UI.cpp ]
// Author : Umeda Souma
// 
//===================================================================================
#include "lobby_character_icon_UI.h"
#include "active_scene_player_state.h"
#include "active_scene_manager.h"

const char* TEX_KIND_ICON[] =
{
	"data/TEXTURE/lobby_character_icon.png",
	"data/TEXTURE/check.png",
	"data/TEXTURE/check_not.png"
};

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
	// サイズの設定
	//SetSize(D3DXVECTOR2(160.0f, 120.0f));

	// 頂点座標の設定
	SetVtx();

	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();

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
	// 頂点座標の設定
	SetVtx();
}

//===================================================================================
// 描画処理
//===================================================================================
void My::CLobbyCharacterIconUI::Draw()
{
	CObject2D::Draw();
}

My::CLobbyCharacterIconUI* My::CLobbyCharacterIconUI::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR color,unsigned int kind)
{
	CLobbyCharacterIconUI* pInstance = new CLobbyCharacterIconUI();

	// 位置の設定
	pInstance->SetPos(pos);

	// サイズの設定
	pInstance->SetSize(D3DXVECTOR2(size.x, size.y));

	// 色の設定
	pInstance->SetColor(color);

	// テクスチャの登録
	pInstance->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_KIND_ICON[kind])));

	pInstance->Init();
	return pInstance;
}

void My::CLobbyCharacterIconUI::SwitchKindTexture(bool check)
{
	if (check)
	{
		BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_KIND_ICON[1])));
	}
	else
	{
		BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_KIND_ICON[2])));
	}
}
