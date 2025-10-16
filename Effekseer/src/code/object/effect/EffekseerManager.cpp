//============================================
//
// �G�t�F�N�g�}�l�[�W���[[EffekseerManager.h]
// Author:Uedakou
// 
//============================================
#include "EffekseerManager.h"       // �G�t�F�N�V�A�}�l�[�W���[
#include <EffekseerRendererDX9.h>   // 
#include <Effekseer.h>
#include "../../system/manager.h"   // �S�̃}�l�[�W���[

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
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice(); // �f�o�C�X�擾

    // Effekseer�}�l�[�W�������i�ő哯���G�t�F�N�g��1000�j
    m_managerRef = Effekseer::Manager::Create(1000);

    // DX9�����_���[�����i�ő�`��p�[�e�B�N����8000�j
    m_rendererRef = EffekseerRendererDX9::Renderer::Create(pDevice, 8000);

    // �p�[�e�B�N���^�C�v�̃����_���[�o�^
    m_managerRef->SetSpriteRenderer(m_rendererRef->CreateSpriteRenderer());
    m_managerRef->SetRibbonRenderer(m_rendererRef->CreateRibbonRenderer());
    m_managerRef->SetRingRenderer(m_rendererRef->CreateRingRenderer());
    m_managerRef->SetTrackRenderer(m_rendererRef->CreateTrackRenderer());
    m_managerRef->SetModelRenderer(m_rendererRef->CreateModelRenderer());

    // Effekseer�`��p�J������ݒ�
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

    // �t���[���X�V
    m_managerRef->Update();

}

void My::CEffekseerManager::Draw()
{
}