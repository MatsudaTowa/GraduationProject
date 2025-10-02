//=============================================
//
//シーン[scene.cpp]
//Author Matsuda Towa
//
//=============================================
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "manager.h"

//モード初期化
My::CScene::MODE My::CScene::m_Mode = CScene::MODE::MODE_TITLE;

//=============================================
//コンストラクタ
//=============================================
My::CScene::CScene()
{
}

//=============================================
//デストラクタ
//=============================================
My::CScene::~CScene()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CScene::Init()
{
    return S_OK;
}

//=============================================
//終了
//=============================================
void My::CScene::Uninit()
{
}

//=============================================
//更新
//=============================================
void My::CScene::Update()
{
}

//=============================================
//描画
//=============================================
void My::CScene::Draw()
{
}

//=============================================
//生成
//=============================================
My::CScene* My::CScene::Create(MODE mode)
{
    // インスタンス生成
    CScene* pScene = nullptr;

    //カメラを全部削除しシーンごとで生成
    CLEAR_CAMERA;

    // 条件分岐
    switch (mode)
    {
    case MODE_TITLE: // タイトル
        //delete pScene;
        pScene = new CTitle;
        break;

    case MODE_GAME: // ゲーム
        //delete pScene;
        pScene = new CGame;
        break;
    case MODE_RESULT: //リザルト
        pScene = new CResult;
        break;
    default:
        assert(false);
        break;
    }

    if (pScene != nullptr)
    {
        pScene->m_Mode = mode;
    }

    //今のシーンの初期化
    pScene->Init();

    //映すカメラを初期化
    SET_CAMERA_IDX(INT_ZERO);

    CManager::GetInstance()->GetModel()->Unload();

    return pScene;
}