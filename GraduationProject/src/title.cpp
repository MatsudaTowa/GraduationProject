//=============================================
//
//タイトル処理[title.cpp]
//Author Matsuda Towa
//
//=============================================
#include "title.h"
#include "player.h"
#include"player.h"
#include "field.h"
#include "title_camera.h"

namespace
{
    const D3DXVECTOR3 FIELD_SIZE = { 1000.0f,0.0f,1000.0f };
}

//=============================================
//コンストラクタ
//=============================================
My::CTitle::CTitle()
{
}

//=============================================
//デストラクタ
//=============================================
My::CTitle::~CTitle()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CTitle::Init()
{
    CREATE_CAMERA(NEW CTitleCamera);

    //地面生成
    CField::Create(VEC3_RESET_ZERO, FIELD_SIZE,new CField);

    CTitle_Screen::Create({ SCREEN_WIDTH * 0.5f,250.0f,0.0f });

    return S_OK;
}

//=============================================
//終了
//=============================================
void My::CTitle::Uninit()
{
	//オブジェクトを破棄
    CObject::ReleaseAll();
}

//=============================================
//更新
//=============================================
void My::CTitle::Update()
{
	//インプット取得
    CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
    CInputMouse* pMouse = GET_INPUT_MOUSE;

    if (pKeyboard->GetTrigger(DIK_RETURN) || pMouse->GetTrigger(0))
    {
		//チュートリアルに遷移
		GET_FADE->SetFade(CScene::MODE::MODE_GAME);
    }
}

//=============================================
//描画
//=============================================
void My::CTitle::Draw()
{
}