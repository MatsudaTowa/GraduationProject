//=============================================
//
//����[result.cpp]
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
//�R���X�g���N�^
//=============================================
My::CResult::CResult()
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CResult::~CResult()
{
}

//=============================================
//������
//=============================================
HRESULT My::CResult::Init()
{
    //������
    CScene::Init();
    CREATE_CAMERA(new CResultCamera);

    //�n�ʐ���
    CField::Create(VEC3_RESET_ZERO, FIELD_SIZE, new CField);

    //�v���C���[����
    CPlayer::Create(new CResultPlayer);

    CResult_Screen::Create(D3DXVECTOR3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF, 0.0f));

   //CResult_Ui::Create(D3DXVECTOR3(SCREEN_WIDTH * HALF, SCREEN_HEIGHT * HALF + 250.0f, 0.0f));

    return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CResult::Uninit()
{
    CObject::ReleaseAll();
}

//=============================================
//�X�V
//=============================================
void My::CResult::Update()
{
    //�e�C���v�b�g�擾
    CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
    CInputMouse* pMouse = GET_INPUT_MOUSE;

    if (pKeyboard->GetTrigger(DIK_RETURN) 
        || pMouse->GetTrigger(0))
    {
        //�^�C�g���ɖ߂�
        GET_FADE->SetFade(CScene::MODE::MODE_TITLE);
    }
}

//=============================================
//�`��
//=============================================
void My::CResult::Draw()
{
}