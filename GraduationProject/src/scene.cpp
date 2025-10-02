//=============================================
//
//�V�[��[scene.cpp]
//Author Matsuda Towa
//
//=============================================
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "manager.h"

//���[�h������
My::CScene::MODE My::CScene::m_Mode = CScene::MODE::MODE_TITLE;

//=============================================
//�R���X�g���N�^
//=============================================
My::CScene::CScene()
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CScene::~CScene()
{
}

//=============================================
//������
//=============================================
HRESULT My::CScene::Init()
{
    return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CScene::Uninit()
{
}

//=============================================
//�X�V
//=============================================
void My::CScene::Update()
{
}

//=============================================
//�`��
//=============================================
void My::CScene::Draw()
{
}

//=============================================
//����
//=============================================
My::CScene* My::CScene::Create(MODE mode)
{
    // �C���X�^���X����
    CScene* pScene = nullptr;

    //�J������S���폜���V�[�����ƂŐ���
    CLEAR_CAMERA;

    // ��������
    switch (mode)
    {
    case MODE_TITLE: // �^�C�g��
        //delete pScene;
        pScene = new CTitle;
        break;

    case MODE_GAME: // �Q�[��
        //delete pScene;
        pScene = new CGame;
        break;
    case MODE_RESULT: //���U���g
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

    //���̃V�[���̏�����
    pScene->Init();

    //�f���J������������
    SET_CAMERA_IDX(INT_ZERO);

    CManager::GetInstance()->GetModel()->Unload();

    return pScene;
}