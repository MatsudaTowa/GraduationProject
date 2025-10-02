//=============================================
//
//�����Ǘ��unumber.cpp�v
// Author���c�i�v
//
//=============================================
#include "number.h"
#include "manager.h"

namespace
{
	const int NUM_TEXTURE = 2;
	const std::string TEXTURE_NAME[NUM_TEXTURE]
	{
		 "data\\TEXTURE\\number_000.png",
		 "data\\TEXTURE\\number_001.png"
	};
}

//=============================================
//�R���X�g���N�^
//=============================================
My::CNumber_2D::CNumber_2D(int nPriority) : CObject2D(nPriority),
m_mintex(VEC2_RESET_ZERO),			//�e�N�X�`���̍ŏ����W
m_maxtex(VEC2_RESET_ZERO)			//�e�N�X�`���̍ő���W
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CNumber_2D::~CNumber_2D()
{
}

//=============================================
//������
//=============================================
HRESULT My::CNumber_2D::Init()
{
	//�e�N���X�̏�����
	CObject2D::Init();

	//���_����
	NumberVtx();

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CNumber_2D::Uninit()
{
	//�e�N���X�̏I��
	CObject2D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CNumber_2D::Update()
{
	//�e�N���X�̍X�V
	CObject2D::Update();

	//���_����
	NumberVtx();
}

//=============================================
//�`��
//=============================================
void My::CNumber_2D::Draw()
{
	//�e�N���X�̕`��
	CObject2D::Draw();
}

//=============================================
//����
//=============================================
My::CNumber_2D* My::CNumber_2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int idx)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CNumber_2D* pNumber = new CNumber_2D;

	// null�Ȃ�null��Ԃ�
	if (pNumber == nullptr) { return nullptr; }

	pNumber->SetPos(pos); //pos�ݒ�

	pNumber->SetSize(size); //size�ݒ�

	pNumber->SetType(OBJECT_TYPE_NUMBER); //�^�C�v�ݒ�

	pNumber->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME[idx]))); //�e�N�X�`���ݒ�

	pNumber->Init(); //����������

	return pNumber;
}

//=============================================
//�����̐ݒ�
//=============================================
void My::CNumber_2D::SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col)
{
	m_mintex.x = fNumPos1;
	m_maxtex.x = fNumPos2;
	SetColor(col);
}

//=============================================
//������p�̒��_����
//=============================================
void My::CNumber_2D::NumberVtx()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	LPDIRECT3DVERTEXBUFFER9 pBuff = GetVtxBuff();

	if (pBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &pBuff, NULL);
	}
	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	pBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(GetPos().x-GetSize().x
		, GetPos().y -GetSize().y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetPos().x + GetSize().x
		, GetPos().y -GetSize().y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GetPos().x -GetSize().x
		, GetPos().y + GetSize().y
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetPos().x+GetSize().x
		, GetPos().y + GetSize().y
		, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = GetColor();
	pVtx[1].col = GetColor();
	pVtx[2].col = GetColor();
	pVtx[3].col = GetColor();

	pVtx[0].tex = D3DXVECTOR2(m_mintex.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_maxtex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_mintex.x, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_maxtex.x, 1.0f);

	//�A�����b�N
	pBuff->Unlock();

	SetVtxBuff(pBuff);
}

//tex�p�X
const std::string My::CNumber_3D::TEXTURE_NAME = "data\\TEXTURE\\number_000.png";

//=============================================
//�R���X�g���N�^
//=============================================
My::CNumber_3D::CNumber_3D(int nPriority):CBillboard(nPriority), m_mintex(D3DXVECTOR2(0.0f, 0.0f)), m_maxtex(D3DXVECTOR2(0.0f, 0.0f))
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CNumber_3D::~CNumber_3D()
{
}

//=============================================
//������
//=============================================
HRESULT My::CNumber_3D::Init()
{
	//�e�N���X�̏�����
	CObject3D::Init();

	//���_����
	NumberVtx();

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CNumber_3D::Uninit()
{
	//�e�N���X�̏I��
	CObject3D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CNumber_3D::Update()
{
	//�e�N���X�̍X�V
	CObject3D::Update();

	//���_����
	NumberVtx();
}

//=============================================
//�`��
//=============================================
void My::CNumber_3D::Draw()
{
	//�e�N���X�̕`��
	CBillboard::Draw();

}

//=============================================
//����
//=============================================
My::CNumber_3D* My::CNumber_3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CNumber_3D* pNumber = new CNumber_3D;

	// null�Ȃ�null��Ԃ�
	if (pNumber == nullptr) { return nullptr; }

	pNumber->SetPos(pos); //pos�ݒ�

	pNumber->SetSize(size); //size�ݒ�

	pNumber->SetType(OBJECT_TYPE_NUMBER); //�^�C�v�ݒ�

	pNumber->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME))); //�e�N�X�`���ݒ�

	pNumber->Init(); //����������

	return pNumber;
}

//=============================================
//�����̐ݒ�
//=============================================
void My::CNumber_3D::SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col)
{
	m_mintex.x = fNumPos1;
	m_maxtex.x = fNumPos2;
	SetColor(col);
}

//=============================================
//������p�̒��_����TODO:Object�̂ق���
//=============================================
void My::CNumber_3D::NumberVtx()
{
	CRenderer* pRender = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 pBuff = GetVtxBuff();

	if (pBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pBuff, NULL);
	}
	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	pBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-GetSize().x
		,GetSize().y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetSize().x
		,GetSize().y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetSize().x
		,-GetSize().y
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+GetSize().x
		,-GetSize().y
		, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].nor = 
	pVtx[1].nor = 
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = GetColor();

	pVtx[0].tex = D3DXVECTOR2(m_mintex.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_maxtex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_mintex.x, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_maxtex.x, 1.0f);

	//�A�����b�N
	pBuff->Unlock();

	SetVtxBuff(pBuff);
}
