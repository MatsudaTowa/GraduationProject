//============================================
//
// エフェクトマネージャー[EffekseerManager.h]
// Author:Uedakou
// 
//============================================
#include "effect_manager.h"       // エフェクシアマネージャー
#include <EffekseerRendererDX9.h>   // 
#include <Effekseer.h>
#include "manager.h"   // 全体マネージャー

/// <summary>
/// コンストラク
/// </summary>
My::CEffectManager::CEffectManager()
{
    Init();
}
/// <summary>
/// デストラクタ
/// </summary>
My::CEffectManager::~CEffectManager()
{
}
/// <summary>
/// 初期化処理
/// </summary>
/// <returns>初期化結果</returns>
HRESULT My::CEffectManager::Init()
{
    // マネージャー
    CManager* pManager = CManager::GetInstance(); // マネージャー取得
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice(); // デバイス取得

    // Effekseerマネージャ生成（最大同時エフェクト数1000）
    m_managerRef = Effekseer::Manager::Create(1000);

    // DX9レンダラー生成（最大描画パーティクル数8000）
    m_rendererRef = EffekseerRendererDX9::Renderer::Create(pDevice, 8000);

    // テクスチャ読み込み
    m_managerRef->SetTextureLoader(m_rendererRef->CreateTextureLoader());
    m_managerRef->SetModelLoader(m_rendererRef->CreateModelLoader());
    m_managerRef->SetMaterialLoader(m_rendererRef->CreateMaterialLoader());
    //m_managerRef->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::DefaultCurveLoader>());

    // パーティクルタイプのレンダラー登録
    m_managerRef->SetSpriteRenderer(m_rendererRef->CreateSpriteRenderer());
    m_managerRef->SetRibbonRenderer(m_rendererRef->CreateRibbonRenderer());
    m_managerRef->SetRingRenderer(m_rendererRef->CreateRingRenderer());
    m_managerRef->SetTrackRenderer(m_rendererRef->CreateTrackRenderer());
    m_managerRef->SetModelRenderer(m_rendererRef->CreateModelRenderer());

    // Effekseer描画用カメラを設定
    Effekseer::Matrix44 proj, view;
    D3DXMATRIX dxView, dxProj;
    pDevice->GetTransform(D3DTS_VIEW, &dxView);
    pDevice->GetTransform(D3DTS_PROJECTION, &dxProj);

    memcpy(&view, &dxView, sizeof(D3DXMATRIX));
    memcpy(&proj, &dxProj, sizeof(D3DXMATRIX));

    m_rendererRef->SetCameraMatrix(view);
    m_rendererRef->SetProjectionMatrix(proj);

    auto texLoader = m_rendererRef->CreateTextureLoader();
    if (texLoader == nullptr) {
        std::cerr << "Error: CreateTextureLoader returned nullptr\n";
    }
    else {
        m_managerRef->SetTextureLoader(texLoader);
    }

    return S_OK;
}
/// <summary>
/// 終了処理
/// </summary>
void My::CEffectManager::Uninit()
{
    if (m_managerRef != nullptr) {
        m_managerRef = nullptr;
    }

    if (m_rendererRef != nullptr) {
        m_rendererRef = nullptr;
    }
}
/// <summary>
/// 更新処理
/// </summary>
void My::CEffectManager::Update()
{
    if (m_managerRef == nullptr) return;

    // フレーム更新
    m_managerRef->Update();

}
/// <summary>
/// 描画処理
/// </summary>
void My::CEffectManager::Draw()
{
}