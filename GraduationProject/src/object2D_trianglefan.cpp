//=============================================
//
//2D三角形オブジェクト[object2D_triangle.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D_trianglefan.h"
#include <DxLib.h>
#include <math.h>

//=============================================
// コンストラクタ
//=============================================
My::CObject2D_TriangleFan::CObject2D_TriangleFan(int nPriority) :CObject2D_Triangle(nPriority),
m_triangleVtx(),
m_Divisionnum(180)
{
}

//=============================================
// デストラクタ
//=============================================
My::CObject2D_TriangleFan::~CObject2D_TriangleFan()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CObject2D_TriangleFan::Init()
{
    CTexture* pTex = GET_TEXTURE;
    BindTexture(pTex->GetAddress(pTex->Regist("data/TEXTURE/cardwaitframe_03.png")));

    SetColor(COLOR_WHITE);

    SetVtx();

    return S_OK;
}

void My::CObject2D_TriangleFan::Uninit()
{
    CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CObject2D_TriangleFan::Update()
{
    m_Divisionnum--;

    if (m_Divisionnum < 0)
        Uninit();

}

//=============================================
// 描画
//=============================================
void My::CObject2D_TriangleFan::Draw()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);

    //テクスチャの設定
    pDevice->SetTexture(0, GetTexture());

    pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, m_Divisionnum);

    SetVtxBuff(vtxBuff);
}

//=============================================
// 頂点生成
//=============================================
void My::CObject2D_TriangleFan::SetVtx()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    // 三角形は頂点3つ
    if (vtxBuff == nullptr)
    {
        pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_Divisionnum*3, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
    }

    VERTEX_2D* pVtx;
    vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    m_CenterPos = { GetPos().x,GetPos().y };

    float x, y;

	float r = 150.0f;
    float angle = 0.0f;
    float a = 0.0f;

    for (int i = 0; i <= m_Divisionnum; ++i)
    {
        if (i == 0)
        {
            pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, 0.0f);
            pVtx[i].rhw = 1.0f;
            pVtx[i].col = COLOR_WHITE;
			pVtx[i].tex = D3DXVECTOR2(0.5f, 0.5f); // テクスチャ使わない
        }
        else
        {
            x = r * cosf(angle);
            y = r * sinf(angle);

			float u = 0.5f + cosf(angle) * 0.5f;
			float v = 0.5f + sinf(angle) * 0.5f;

            pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x + x, m_CenterPos.y + y, 0.0f);

            if(i == m_Divisionnum)
            {
                pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x + r, m_CenterPos.y, 0.0f);
			}

            pVtx[i].rhw = 1.0f;
            pVtx[i].col = COLOR_WHITE;
            pVtx[i].tex = D3DXVECTOR2(u, v); // テクスチャ使わない

            angle += (2.0f * D3DX_PI) / m_Divisionnum;
            a += 1.0f / m_Divisionnum;
        }
    }


    //for (int i = 0; i < NUM_VERTEX; ++i)
    //{
    //    pVtx[i].pos = D3DXVECTOR3(m_triangleVtx[i].x, m_triangleVtx[i].y, 0.0f);

    //    pVtx[i].rhw = 1.0f;
    //    pVtx[i].col = GetColor();
    //    pVtx[i].tex = D3DXVECTOR2(0, 0); // テクスチャ使わない
    //}

    vtxBuff->Unlock();
    SetVtxBuff(vtxBuff);

    m_OldCenterPos = m_CenterPos;
}

void My::CObject2D_TriangleFan::SetVtxPos()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    // 三角形は頂点3つ
    if (vtxBuff == nullptr)
    {
        pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * m_Divisionnum * 3, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
    }

    VERTEX_2D* pVtx;
    vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

    m_CenterPos = { GetPos().x,GetPos().y };

    float x, y;

    float r = 150.0f;
    float angle = 0.0f;
    float a = 0.0f;

    for (int i = 0; i <= m_Divisionnum; ++i)
    {
        if (i == 0)
        {
            pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x, m_CenterPos.y, 0.0f);
        }
        else
        {
            x = r * cosf(angle);
            y = r * sinf(angle);

            float u = 0.5f + cosf(angle) * 0.5f;
            float v = 0.5f + sinf(angle) * 0.5f;

            pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x + x, m_CenterPos.y + y, 0.0f);

            if (i == m_Divisionnum)
            {
                pVtx[i].pos = D3DXVECTOR3(m_CenterPos.x + r, m_CenterPos.y, 0.0f);
            }

            pVtx[i].tex = D3DXVECTOR2(u, v); // テクスチャ使わない

            angle += (2.0f * D3DX_PI) / m_Divisionnum;
            a += 1.0f / m_Divisionnum;
        }
    }
    vtxBuff->Unlock();
    SetVtxBuff(vtxBuff);

    m_OldCenterPos = m_CenterPos;
}
