//=============================================
//
//3DTemplate[object3D.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object3D.h"
#include "renderer.h"
#include "manager.h"

/**
 * @brief �R���X�g���N�^
 * @param [in]�v���C�I���e�B
 */
My::CObject3D::CObject3D(int nPriority):CObject(nPriority),
m_col(COLOR_WHITE),					//�J���[������
m_pos(VEC3_RESET_ZERO),				//�ʒu������
m_rot(VEC3_RESET_ZERO),				//����������
m_size(VEC3_RESET_ZERO),			//�T�C�Y������
m_tex_pos(VEC2_RESET_ZERO),			//�e�N�X�`�����W������
m_mtxWorld(),						//���[���h�}�g���b�N�X������
m_pTexture(),						//�e�N�X�`���Ǘ��|�C���^������
m_pVtxBuff()						//���_�o�b�t�@������
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CObject3D::~CObject3D()
{
}

/**
 * @brief ������
 * @return ����������
 */
HRESULT My::CObject3D::Init()
{
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CObject3D::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

/**
 * @brief �X�V
 */
void My::CObject3D::Update()
{
}

/**
 * @brief �`��
 */
void My::CObject3D::Draw()
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/**
 * @brief �e�N�X�`���ݒ�
 * @param [in]�e�N�X�`���|�C���^
 */
void My::CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

/**
 * @brief ���_����
 * @param [in]�@���x�N�g��
 */
void My::CObject3D::SetVtx(D3DXVECTOR3 nor)
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	if (m_pVtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x
		, m_size.y
		, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_size.x
		, m_size.y
		, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(-m_size.x
		, -m_size.y
		, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_size.x
		, -m_size.y
		, -m_size.z);


	//�@���̐ݒ�
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = nor;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�A�����b�N
	m_pVtxBuff->Unlock();

}


/**
 * @brief ���_����(�g�k�A��])
 * @param [in]�@���x�N�g��
 * @param [in]����
 * @param [in]���_�̋���
 */
void My::CObject3D::SetVtx(D3DXVECTOR3 nor, float fAngle, float fLength)
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	if (m_pVtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	}
	VERTEX_3D* pVtx;
	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-m_size.x + sinf(m_rot.z - (D3DX_PI - fAngle)) * fLength
		, m_size.y + cosf(m_rot.z - (D3DX_PI - fAngle)) * fLength
		, m_size.z);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fLength
		, m_size.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fLength
		, m_size.z);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fLength
		, -m_size.y + cosf(m_rot.z - fAngle) * fLength
		, -m_size.z);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fLength
		, -m_size.y + cosf(m_rot.z + fAngle) * fLength
		, -m_size.z);

	//�@���̐ݒ�
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = nor;

	//���_�J���[�̐ݒ�
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�A�����b�N
	m_pVtxBuff->Unlock();

}