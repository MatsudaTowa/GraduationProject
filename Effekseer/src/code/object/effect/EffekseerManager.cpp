//============================================
//
// エフェクトマネージャー[EffekseerManager.h]
// Author:Uedakou
// 
//============================================
#include "EffekseerManager.h"       // エフェクシアマネージャー
#include <EffekseerRendererDX9.h>   // 
#include <Effekseer.h>
#include "../../system/manager.h"   // 全体マネージャー

My::CEffekseerManager::CEffekseerManager()
{
    Init();
}

My::CEffekseerManager::~CEffekseerManager()
{
}

HRESULT My::CEffekseerManager::Init()
{
    CManager* pManager = CManager::GetInstance();
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice(); // デバイス取得

    // Effekseerマネージャ生成（最大同時エフェクト数1000）
    m_managerRef = Effekseer::Manager::Create(1000);

    // DX9レンダラー生成（最大描画パーティクル数8000）
    m_rendererRef = EffekseerRendererDX9::Renderer::Create(pDevice, 8000);

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

    return S_OK;
}

void My::CEffekseerManager::Uninit()
{
    if (m_managerRef != nullptr) {
        m_managerRef = nullptr;
    }

    if (m_rendererRef != nullptr) {
        m_rendererRef = nullptr;
    }
}

void My::CEffekseerManager::Update()
{
    if (m_managerRef == nullptr) return;

    // フレーム更新
    m_managerRef->Update();

}

void My::CEffekseerManager::Draw()
{
}