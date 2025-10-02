//=============================================
//
//���U���g[result_ui.cpp]
//Author Kasai Keisuke
//
//=============================================
#include "result_ui.h"

//=============================================
//�R���X�g���N�^
//=============================================
My::CResult_Ui::CResult_Ui(int nPriority):CObject2D(nPriority)
{

}

//=============================================
//�f�X�g���N�^
//=============================================
My::CResult_Ui::~CResult_Ui()
{

}

//=============================================
//����������
//=============================================
HRESULT My::CResult_Ui::Init()
{
	//�e�N���X�̏������������Ă�
	CObject2D::Init();

	//�������g�̃T�C�Y�擾
	D3DXVECTOR2 size = GetSize();
	
	size = D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);

	//�e�N�X�`�����W�ݒ�
	SetTexPos(VEC2_RESET_ONE);

	//�T�C�Y����
	SetSize(size);

	//�J���[�擾
	D3DXCOLOR col = GetColor();

	col = COLOR_WHITE;

	//�J���[����
	SetColor(col);

	//���_�ݒ�
	SetVtx();

	return S_OK;
}

//=============================================
//�I������
//=============================================
void My::CResult_Ui::Uninit()
{
	//�e�N���X�̏I���������Ă�
	CObject2D::Uninit();
}

//=============================================
//�X�V����
//=============================================
void My::CResult_Ui::Update()
{
	// �e�N���X�̍X�V�������Ă�
	CObject2D::Update();
}

//=============================================
//�`�揈��
//=============================================
void My::CResult_Ui::Draw()
{
	// �e�N���X�̕`�揈�����Ă�
	CObject2D::Draw();
}

//=============================================
//��������
//=============================================
My::CResult_Ui* My::CResult_Ui::Create(D3DXVECTOR3 pos)
{
	CResult_Ui* pResult_Ui = new CResult_Ui();

	//null�Ȃ�null��Ԃ�
	if (pResult_Ui == nullptr) { return nullptr; }

	//pos�ݒ�
	pResult_Ui->SetPos(pos);

	//�^�C�v�ݒ�
	pResult_Ui->SetType(OBJECT_TYPE_RESULT);

	//������
	pResult_Ui->Init();

	return pResult_Ui;
}
