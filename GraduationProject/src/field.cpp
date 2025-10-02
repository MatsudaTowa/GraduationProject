//=============================================
//
//3DTemplate[field.cpp]
//Author Matsuda Towa
//
//=============================================
#include "field.h"
#include "manager.h"

namespace
{
	std::string TEX_NAME = "data\\TEXTURE\\tatami.jpeg";
}

//=============================================
//�R���X�g���N�^
//=============================================
My::CField::CField(int nPriority) : CObject3D(nPriority)
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CField::~CField()
{
}

//=============================================
//������
//=============================================
HRESULT My::CField::Init()
{
	SetColor(COLOR_GREEN);

	//�e�N�X�`���ݒ�
	CTexture* pTexture = GET_TEXTURE;
	BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));

	//���_�ݒ�
	SetVtx(D3DXVECTOR3(0.0f,1.0f,0.0f));
	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CField::Uninit()
{
	CObject3D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CField::Update()
{

}

//=============================================
//�`��
//=============================================
void My::CField::Draw()
{
	// �e�N���X�̕`��
	CObject3D::Draw();
}

//=============================================
//  ����
//=============================================
My::CField* My::CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CField* field)
{
	CField* pField = field;

	// null�Ȃ�null��Ԃ�
	if (pField == nullptr) { return nullptr; }

	//�e�N�X�`���擾
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	pField->SetPos(pos); //pos�ݒ�
	pField->SetSize(size); //�T�C�Y�ݒ�
	//pField->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME))); //�e�N�X�`���ݒ�
	pField->SetType(OBJECT_TYPE_FIELD); //�^�C�v�ݒ�
	pField->Init(); //���̏���������

	return pField;
}