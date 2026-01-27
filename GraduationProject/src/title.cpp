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
#include "raknet.h"
#include "card_client.h"

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
    CREATE_CAMERA(new CTitleCamera);

    //地面生成
    CField::Create(VEC3_RESET_ZERO, FIELD_SIZE,new CField);

    CTitle_Screen::Create({ SCREEN_WIDTH * 0.5f,250.0f,0.0f });

    //BGMの設定
    CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================
//終了
//=============================================
void My::CTitle::Uninit()
{
    //BGMを止める
    CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_TITLE);

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
        CRakNet::GetInstance()->SetOnline(false);

		//チュートリアルに遷移
		GET_FADE->SetFade(CScene::MODE::MODE_GAME);

        //受信していなかったらリクエストを送る
        if (!CCard_Client::GetInstance()->GetIsReceiveCardInfo())
        {
            CCard_Client::GetInstance()->RequestAllCard();
        }
        
    }

    //左シフト押したらオンライン
    if (pKeyboard->GetTrigger(DIK_LSHIFT))
    {
        //オンライン
        CRakNet::GetInstance()->SetOnline(true);

        //チュートリアルに遷移
        GET_FADE->SetFade(CScene::MODE::MODE_GAME);

        //受信していなかったらリクエストを送る
        if (!CCard_Client::GetInstance()->GetIsReceiveCardInfo())
        {
            CCard_Client::GetInstance()->RequestAllCard();
        }
    }
}

//=============================================
//描画
//=============================================
void My::CTitle::Draw()
{
}