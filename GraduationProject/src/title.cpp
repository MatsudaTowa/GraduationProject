//=============================================
//
//�^�C�g������[title.cpp]
//Author Matsuda Towa
//
//=============================================
#include "title.h"
#include "player.h"
#include"player.h"
#include "field.h"
#include "title_camera.h"

namespace
{
    const D3DXVECTOR3 FIELD_SIZE = { 1000.0f,0.0f,1000.0f };
}

//=============================================
//�R���X�g���N�^
//=============================================
My::CTitle::CTitle()
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CTitle::~CTitle()
{
}

//=============================================
//������
//=============================================
HRESULT My::CTitle::Init()
{
    CREATE_CAMERA(NEW CTitleCamera);

    //�n�ʐ���
    CField::Create(VEC3_RESET_ZERO, FIELD_SIZE,new CField);

    CTitle_Screen::Create({ SCREEN_WIDTH * 0.5f,250.0f,0.0f });

    return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CTitle::Uninit()
{
	//�I�u�W�F�N�g��j��
    CObject::ReleaseAll();
}

//=============================================
//�X�V
//=============================================
void My::CTitle::Update()
{
	//�C���v�b�g�擾
    CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
    CInputMouse* pMouse = GET_INPUT_MOUSE;

    if (pKeyboard->GetTrigger(DIK_RETURN) || pMouse->GetTrigger(0))
    {
		//�`���[�g���A���ɑJ��
		GET_FADE->SetFade(CScene::MODE::MODE_GAME);
    }
}

//=============================================
//�`��
//=============================================
void My::CTitle::Draw()
{
}