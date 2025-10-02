//=============================================
//
//�^�C�g����UI[title_Screen.cpp]
//Author Matsuda Towa
//
//=============================================
#include "title_screen.h"
#include "manager.h"
#include "object.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\002.png";
	const D3DXVECTOR2 SCREEN_SIZE = {512.0f,256.0f };
}

//=============================================
//�R���X�g���N�^
//=============================================
My::CTitle_Screen::CTitle_Screen(int nPriority):CObject2D(nPriority)
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CTitle_Screen::~CTitle_Screen()
{
}

//=============================================
//������
//=============================================
HRESULT My::CTitle_Screen::Init()
{
	//�e�N���X�̏��������Ă�
	CObject2D::Init();

	//�e�N�X�`�����W�ݒ�
	SetTexPos(VEC2_RESET_ONE);

	//�J���[�擾
	D3DXCOLOR col = GetColor();

	col = COLOR_WHITE;

	//�J���[����
	SetColor(col);

	SetSize(SCREEN_SIZE);

	//���_�ݒ�
	SetVtx();
	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CTitle_Screen::Uninit()
{
	//�e�N���X�̏I���Ă�
	CObject2D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CTitle_Screen::Update()
{
	//�e�N���X�̍X�V�Ă�
	CObject2D::Update();
}

//=============================================
//�`��
//=============================================
void My::CTitle_Screen::Draw()
{
	//�e�N���X�̕`��Ă�
	CObject2D::Draw();
}

//=============================================
//����
//=============================================
My::CTitle_Screen* My::CTitle_Screen::Create(D3DXVECTOR3 pos)
{
	CTexture* pTexture = GET_TEXTURE;
	CTitle_Screen* pTitle_Screen = new CTitle_Screen;

	//null�Ȃ�null��Ԃ�
	if (pTitle_Screen == nullptr) {return nullptr;}

	pTitle_Screen->SetPos(pos); //pos�ݒ�

	pTitle_Screen->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));

	pTitle_Screen->SetType(OBJECT_TYPE_TITLE); //�^�C�v�ݒ�

	pTitle_Screen->Init();

	return pTitle_Screen;
}
