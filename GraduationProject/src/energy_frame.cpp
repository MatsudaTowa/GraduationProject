//==================================================================================================================
//
// �G�i�W�[UI�g �uenergy_frame.cpp�v
// Author : �������D
//
//==================================================================================================================
#include "energy_frame.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\UI\\Energy.png";
}

//=================================================================
// �R���X�g���N�^
//=================================================================
My::CEnergy_frame::CEnergy_frame(int nPriority) : CObject2D(nPriority)
{
}

//=================================================================
// �f�X�g���N�^
//=================================================================
My::CEnergy_frame::~CEnergy_frame()
{

}

//=================================================================
// ������
//=================================================================
HRESULT My::CEnergy_frame::Init()
{

	CObject2D::Init();

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	//�e�N�X�`���o�^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));//�e�N�X�`���ݒ�

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();


	return S_OK;
}

//=================================================================
// �I��
//=================================================================
void My::CEnergy_frame::Uninit()
{
	CObject2D::Uninit();
}

//=================================================================
// �X�V
//=================================================================
void My::CEnergy_frame::Update()
{
	CObject2D::Update();
}

//=================================================================
// �`��
//=================================================================
void My::CEnergy_frame::Draw()
{
	CObject2D::Draw();
}
//=================================================================
// ����
//=================================================================
My::CEnergy_frame* My::CEnergy_frame::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CEnergy_frame* pEnergyFrame = new CEnergy_frame(11);
	pEnergyFrame->SetPos(pos);
	pEnergyFrame->SetSize(size);
	pEnergyFrame->SetType(CObject::OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	pEnergyFrame->Init();
	return pEnergyFrame;
}