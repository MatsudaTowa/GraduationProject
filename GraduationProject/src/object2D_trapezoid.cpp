//=============================================
//
//2D三角形オブジェクト[object2D_triangle.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D_trapezoid.h"

//=============================================
// コンストラクタ
//=============================================
My::CObject2D_Trapezoid::CObject2D_Trapezoid(int nPriority) :CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CObject2D_Trapezoid::~CObject2D_Trapezoid()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CObject2D_Trapezoid::Init()
{
    return S_OK;
}

//=============================================
// 更新
//=============================================
void My::CObject2D_Trapezoid::Update()
{
}

//=============================================
// 描画
//=============================================
void My::CObject2D_Trapezoid::Draw()
{
    CRenderer* pRender = GET_RENDERER;
    LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

    LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

    pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);

    pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

    SetVtxBuff(vtxBuff);
}

//=============================================
// 頂点生成
//=============================================
void My::CObject2D_Trapezoid::SetTrapeVtx(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col,float Angle)
{
	CRenderer* pRender = GET_RENDERER;
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_2D* pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//4点のローカル座標設定
	D3DXVECTOR3 pos_L[NUM_VERTEX];
	pos_L[0] = D3DXVECTOR3(-size.x, -size.y, FLOAT_ZERO);//左上にあたる場所
	pos_L[1] = D3DXVECTOR3(size.x, -size.y, FLOAT_ZERO);//右上にあたる場所
	pos_L[2] = D3DXVECTOR3(-size.x * 0.8f, size.y, FLOAT_ZERO);//左下にあたる場所
	pos_L[3] = D3DXVECTOR3(size.x * 0.8f, size.y, FLOAT_ZERO);//右下にあたる場所

	//回転行列作成
	D3DXMATRIX matRot; 
	float Angle_L = D3DXToRadian(Angle);
	D3DXMatrixRotationZ(&matRot, Angle_L);

	//頂点座標の設定
	for (int i = 0; i < NUM_VERTEX; i++)
	{
		D3DXVECTOR3 rot;
		D3DXVec3TransformCoord(&rot, &pos_L[i], &matRot);
		pVtx[i].pos = rot + pos;
	}

	//pVtx[0].pos = D3DXVECTOR3(pos.x - size.x
	//	, pos.y - size.y
	//	, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(pos.x + size.x
	//	, pos.y - size.y
	//	, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(pos.x - size.x * 0.75f
	//	, pos.y + size.y
	//	, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(pos.x + size.x * 0.75f
	//	, pos.y + size.y
	//	, 0.0f);


	//rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = FLOAT_ONE;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	pVtx[0].tex =
		pVtx[1].tex =
		pVtx[2].tex =
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	vtxBuff->Unlock();
	SetVtxBuff(vtxBuff);
}

//=============================================
// 円状配置用頂点生成
//=============================================
void My::CObject2D_Trapezoid::SetTrapeVtx_Radial(D3DXVECTOR3 pos, D3DXCOLOR col, float Rad_In, float Rad_Out, float Angle_Start, float Angle_End)
{
	CRenderer* pRender = GET_RENDERER;
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_2D* pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//内側
	pVtx[0].pos = {pos.x + cosf(Angle_Start) * Rad_In,
	pos.y + sinf(Angle_Start) * Rad_In,
	FLOAT_ZERO};

	pVtx[1].pos = { pos.x + cosf(Angle_Start) * Rad_Out,
	pos.y + sinf(Angle_Start) * Rad_Out,
	FLOAT_ZERO };

	//外側
	pVtx[2].pos = { pos.x + cosf(Angle_End) * Rad_In,
	pos.y + sinf(Angle_End) * Rad_In,
	FLOAT_ZERO };

	pVtx[3].pos = { pos.x + cosf(Angle_End) * Rad_Out,
	pos.y + sinf(Angle_End) * Rad_Out,
	FLOAT_ZERO };

	//rhwの設定
	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = FLOAT_ONE;

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	pVtx[0].tex =
		pVtx[1].tex =
		pVtx[2].tex =
		pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);

	vtxBuff->Unlock();
	SetVtxBuff(vtxBuff);
}

//=============================================
// 円状配置用頂点生成
//=============================================
void My::CObject2D_Trapezoid::ChangeColor(D3DXCOLOR col)
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	VERTEX_2D* pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	vtxBuff->Unlock();
	SetVtxBuff(vtxBuff);
}

//=============================================
// 頂点生成
//=============================================
void My::CObject2D_Trapezoid::SetVtx()
{

}
