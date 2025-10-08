//=============================================
//
//2D�O�p�`�I�u�W�F�N�g[object2D_triangle.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D_triangle.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CObject2D_Triangle::CObject2D_Triangle(int nPriority):CObject2D(nPriority),
m_triangleVtx()
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CObject2D_Triangle::~CObject2D_Triangle()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CObject2D_Triangle::Init()
{
	return S_OK;
}

//=============================================
// �X�V
//=============================================
void My::CObject2D_Triangle::Update()
{
}

//=============================================
// �`��
//=============================================
void My::CObject2D_Triangle::Draw()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

    SetVtxBuff(vtxBuff);
}

//=============================================
// ���_����
//=============================================
void My::CObject2D_Triangle::SetVtx()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    // �O�p�`�͒��_3��
    if (vtxBuff == nullptr)
    {
        pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
    }

    VERTEX_2D* pVtx;
    vtxBuff->Lock(0, 0, (void**)&pVtx, 0);


    for (int i = 0; i < NUM_VERTEX; ++i)
    {
        pVtx[i].pos = D3DXVECTOR3(m_triangleVtx[i].x, m_triangleVtx[i].y, 0.0f);

        pVtx[i].rhw = 1.0f;
        pVtx[i].col = GetColor();
        pVtx[i].tex = D3DXVECTOR2(0, 0); // �e�N�X�`���g��Ȃ�
    }

    vtxBuff->Unlock();
    SetVtxBuff(vtxBuff);
}
