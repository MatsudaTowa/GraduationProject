//=============================================
//
//結果[result.cpp]
//Author Matsuda Towa
//
//=============================================
#include "result.h"
#include "game.h"
#include "result_screen.h"
#include "result_ui.h"
#include "result_camera.h"
#include "field.h"
#include "result_player.h"

namespace
{
    const D3DXVECTOR3 FIELD_SIZE = { 1000.0f,0.0f,1000.0f };
}
//=============================================
//コンストラクタ
//=============================================
My::CResult::CResult()
{
}

//=============================================
//デストラクタ
//=============================================
My::CResult::~CResult()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CResult::Init()
{
    //初期化
    CScene::Init();
    CREATE_CAMERA(new CResultCamera);

    //地面生成
    CField::Create(VEC3_RESET_ZERO, FIELD_SIZE, new CField);

    //プレイヤー生成
    CPlayer::Create(new CResultPlayer);

    CResult_Screen::Create(D3DXVECTOR3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF, 0.0f));

   //CResult_Ui::Create(D3DXVECTOR3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF + 250.0f, 0.0f));

    return S_OK;
}

//=============================================
//終了
//=============================================
void My::CResult::Uninit()
{
    CObject::ReleaseAll();
}

//=============================================
//更新
//=============================================
void My::CResult::Update()
{
    //各インプット取得
    CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
    CInputMouse* pMouse = GET_INPUT_MOUSE;

    if (pKeyboard->GetTrigger(DIK_RETURN) 
        || pMouse->GetTrigger(0))
    {
        //タイトルに戻る
        GET_FADE->SetFade(CScene::MODE::MODE_TITLE);
    }
}

//=============================================
//描画
//=============================================
void My::CResult::Draw()
{
}