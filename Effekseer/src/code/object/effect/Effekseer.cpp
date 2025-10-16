//============================================
//
// エフェクト[Effekseer.cpp]
// Author:Uedakou
// 
//============================================
//#include "DxLib.h"
#include "Effekseer.h"  // エフェクシア
#include "../../system/manager.h"  // マネージャー
#include "EffekseerManager.h"   // エフェクシア


// コンストラクタ
My::Effect::Effect():
    CObject(4)
    , playCount(0)
{
}

// デストラクタ
My::Effect::~Effect()
{
}

// 初期化
HRESULT My::Effect::Init()
{
    My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();

    // エフェクト読み込み
    std::wstring effectPathW = L"data/Flame.efk";
    const char16_t* effectPathC16 = reinterpret_cast<const char16_t*>(effectPathW.c_str());
    g_effectRef = Effekseer::Effect::Create(managerRef, effectPathC16);
    if (g_effectRef == nullptr) {
        printf("Effekseer effect load failed: %ls\n", effectPathW.c_str());
        return E_FAIL;
    }

    // 再生（カメラ正面あたりに配置）
    g_handle = managerRef->Play(g_effectRef, 0.0f, 0.0f, 5.0f);

    return S_OK;
}

void My::Effect::Uninit()
{
    if (g_effectRef != nullptr) {
        g_effectRef = nullptr;
    }
}

/// <summary>
/// 更新
/// </summary>
/// <param name="playPosition">再生座標</param>
void My::Effect::Update()
{
    My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();
    // 定期的にエフェクトを再生する
    if (!(playCount % EffectPlayInterval))
    {
        // 再生（カメラ正面あたりに配置）
        g_handle = managerRef->Play(g_effectRef, 0.0f, 0.0f, 5.0f);
    }

    // 再生カウントを進める
    playCount++;
}

// 描画
void My::Effect::Draw()
{
    My::CManager* pManager = CManager::GetInstance();   // マネージャー取得
    My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // エフェクシアマネージャー取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();   // デバイス取得

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();
    EffekseerRenderer::RendererRef  rendererRef = pEffekseerManager->GetRenderer();

    if (managerRef == nullptr) return;

    // ビューとプロジェクションを毎フレーム更新（DX側でカメラ動く場合）
    Effekseer::Matrix44 proj, view;
    D3DXMATRIX dxView, dxProj;
    pDevice->GetTransform(D3DTS_VIEW, &dxView);
    pDevice->GetTransform(D3DTS_PROJECTION, &dxProj);
    memcpy(&view, &dxView, sizeof(D3DXMATRIX));
    memcpy(&proj, &dxProj, sizeof(D3DXMATRIX));
    rendererRef->SetCameraMatrix(view);
    rendererRef->SetProjectionMatrix(proj);

    // Effekseer描画
    rendererRef->BeginRendering();
    managerRef->Draw();
    rendererRef->EndRendering();
}

// クリエイト
My::Effect* My::Effect::create(const std::string sFilepas)
{
    Effect* p = new Effect();

    p->Init();

    return p;
}
