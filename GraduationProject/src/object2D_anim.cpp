//=============================================
//
//�A�j���[�V����2D[object2D_anim.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D_anim.h"
#include "manager.h"

/**
 * @brief �R���X�g���N�^
 * @param [in]�v���C�I���e�B
 */
My::CObject2D_Anim::CObject2D_Anim(int nPriority):CObject2D(nPriority),
m_nAnimCnt(INT_ZERO)	//�A�j���[�V�����J�E���g������
{
}

/**
 * @brief �f�X�g���N�^
 */
My::CObject2D_Anim::~CObject2D_Anim()
{
}

/**
 * @brief ������
 * @return ����������
 */
HRESULT My::CObject2D_Anim::Init()
{
	return S_OK;
}

/**
 * @brief �I��
 */
void My::CObject2D_Anim::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CObject2D_Anim::Update()
{
	//�e�N�X�`�����W�擾
	D3DXVECTOR2 tex_pos = GetTexPos();
}

/**
 * @brief �`��
 */
void My::CObject2D_Anim::Draw()
{
	CObject2D::Draw();
}

/**
 * @brief �A�j���[�V��������
 * @param [in]�e�N�X�`�����W
 * @param [in]�e�N�X�`���ړ���
 */
void My::CObject2D_Anim::AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move)
{
	m_nAnimCnt++;

	if (m_nAnimCnt >= m_nAnimFrame)
	{//�t���[�������B������
		//�e�N�X�`�������炷
		tex_pos.x += tex_move.x;
		m_tex_move = tex_move;

		m_nAnimCnt = INT_ZERO;
	}

	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_2D* pVtx;

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

	//tex�ݒ�
 	SetTexPos(tex_pos);
}