//=============================================
//
//�A�j���[�V����3D[object3D_anim.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object3D_anim.h"
#include "manager.h"

/**
 * @brief �R���X�g���N�^
 * @param [in]�v���C�I���e�B
 */
My::CObject3D_Anim::CObject3D_Anim(int nPriority) :CObject3D(nPriority),
m_pCnt(nullptr)	//�A�j���[�V�����J�E���g������
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CObject3D_Anim::~CObject3D_Anim()
{
}

/**
 * @brief ������
 * @return ����������
 */
HRESULT My::CObject3D_Anim::Init()
{
	if (m_pCnt == nullptr)
	{
		m_pCnt = new CCount;
	}
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CObject3D_Anim::Uninit()
{
	if (m_pCnt != nullptr)
	{
		delete m_pCnt;
		m_pCnt = nullptr;
	}
	CObject3D::Uninit();
}

/**
 * @brief �X�V
 */
void My::CObject3D_Anim::Update()
{
	//�e�N�X�`�����W�擾
	D3DXVECTOR2 tex_pos = GetTexPos();
}

/**
 * @brief �`��
 */
void My::CObject3D_Anim::Draw()
{
	CObject3D::Draw();
}

/**
 * @brief �A�j���[�V��������
 * @param [in]�e�N�X�`�����W
 * @param [in]�e�N�X�`���ړ���
 */
void My::CObject3D_Anim::AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move)
{
	if (m_pCnt->CountUp())
	{//�t���[�������B������
		//�e�N�X�`�������炷
		tex_pos.x += tex_move.x;
		m_tex_move = tex_move;

		m_pCnt->SetCnt(INT_ZERO);
	}

	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_3D* pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̍��W�w��
	pVtx[0].tex = D3DXVECTOR2(tex_pos.x
		, tex_pos.y);
	pVtx[1].tex = D3DXVECTOR2(tex_pos.x + tex_move.x
		, tex_pos.y);
	pVtx[2].tex = D3DXVECTOR2(tex_pos.x
		, tex_pos.y + tex_move.y);
	pVtx[3].tex = D3DXVECTOR2(tex_pos.x + tex_move.x
		, tex_pos.y + tex_move.y);

	//�A�����b�N
	vtxBuff->Unlock();

	SetVtxBuff(vtxBuff);

	//tex�ݒ�
	SetTexPos(tex_pos);
}