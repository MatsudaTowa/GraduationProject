//============================================
//
// �G�t�F�N�g[Effekseer.cpp]
// Author:Uedakou
// 
//============================================
//#include "DxLib.h"
#include "Effekseer.h"  // �G�t�F�N�V�A
#include "../../system/manager.h"  // �}�l�[�W���[
#include "EffekseerManager.h"   // �G�t�F�N�V�A


// �R���X�g���N�^
My::Effect::Effect():
    CObject(4)
    , playCount(0)
{
}

// �f�X�g���N�^
My::Effect::~Effect()
{
}

// ������
HRESULT My::Effect::Init()
{
    My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();

    // �G�t�F�N�g�ǂݍ���
    std::wstring effectPathW = L"data/Flame.efk";
    const char16_t* effectPathC16 = reinterpret_cast<const char16_t*>(effectPathW.c_str());
    g_effectRef = Effekseer::Effect::Create(managerRef, effectPathC16);
    if (g_effectRef == nullptr) {
        printf("Effekseer effect load failed: %ls\n", effectPathW.c_str());
        return E_FAIL;
    }

    // �Đ��i�J�������ʂ�����ɔz�u�j
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
/// �X�V
/// </summary>
/// <param name="playPosition">�Đ����W</param>
void My::Effect::Update()
{
    My::CEffekseerManager* pEffekseerManager = CManager::GetInstance()->GetEffekseerManager();

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();
    // ����I�ɃG�t�F�N�g���Đ�����
    if (!(playCount % EffectPlayInterval))
    {
        // �Đ��i�J�������ʂ�����ɔz�u�j
        g_handle = managerRef->Play(g_effectRef, 0.0f, 0.0f, 5.0f);
    }

    // �Đ��J�E���g��i�߂�
    playCount++;
}

// �`��
void My::Effect::Draw()
{
    My::CManager* pManager = CManager::GetInstance();   // �}�l�[�W���[�擾
    My::CEffekseerManager* pEffekseerManager = pManager->GetEffekseerManager(); // �G�t�F�N�V�A�}�l�[�W���[�擾
    LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();   // �f�o�C�X�擾

    Effekseer::ManagerRef managerRef = pEffekseerManager->GetManager();
    EffekseerRenderer::RendererRef  rendererRef = pEffekseerManager->GetRenderer();

    if (managerRef == nullptr) return;

    // �r���[�ƃv���W�F�N�V�����𖈃t���[���X�V�iDX���ŃJ���������ꍇ�j
    Effekseer::Matrix44 proj, view;
    D3DXMATRIX dxView, dxProj;
    pDevice->GetTransform(D3DTS_VIEW, &dxView);
    pDevice->GetTransform(D3DTS_PROJECTION, &dxProj);
    memcpy(&view, &dxView, sizeof(D3DXMATRIX));
    memcpy(&proj, &dxProj, sizeof(D3DXMATRIX));
    rendererRef->SetCameraMatrix(view);
    rendererRef->SetProjectionMatrix(proj);

    // Effekseer�`��
    rendererRef->BeginRendering();
    managerRef->Draw();
    rendererRef->EndRendering();
}

// �N���G�C�g
My::Effect* My::Effect::create(const std::string sFilepas)
{
    Effect* p = new Effect();

    p->Init();

    return p;
}
