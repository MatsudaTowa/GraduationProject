//============================================
//
// ポリゴン３D[object3D.cpp]
// Author:Uedakou
// 
//============================================
#include "object_3D.h"	// オブジェクト3D
#include "../../system/manager.h"	// マネージャー
//#include "../../system/pch.h"		// プリコンパイル


//============================================
// コンスト
//============================================
My::CObject3D::CObject3D()
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	BlockX = 1;
	BlockY = 1;
}
My::CObject3D::CObject3D(const int nPriority) :
	CObject(nPriority)
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_col = ::D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	BlockX = 1;
	BlockY = 1;
}
//============================================
// デストラ
//============================================
My::CObject3D::~CObject3D()
{
}
//============================================
// 初期化
//============================================
HRESULT My::CObject3D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイスへのポインタ
	X x = GetX();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);


	VERTEX_3D* pVtx;	// 頂点ポインタ
	// 頂点バッファロックをし、頂点情報を
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(-m_siz.x * x.scl.x * 0.5f, m_siz.y * x.scl.y, m_siz.z * x.scl.z * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(m_siz.x * x.scl.x * 0.5f, m_siz.y * x.scl.y, m_siz.z * x.scl.z * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-m_siz.x * x.scl.x * 0.5f, 0, -m_siz.z * x.scl.z * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(m_siz.x * x.scl.x * 0.5f, 0, -m_siz.z * x.scl.z * 0.5f);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点ロックをアンロック
	m_pVtxBuff->Unlock();
	return S_OK;
}
//============================================
// 終了
//============================================
void My::CObject3D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}
//============================================
// 更新
//============================================
void My::CObject3D::Update()
{
}
//============================================
// 描画
//============================================
void My::CObject3D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリクス

	X x = GetX();

	// ワールドマトリクスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 向き
	D3DXMatrixRotationYawPitchRoll(&mtxRot, x.rot.y, x.rot.x, x.rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, x.pos.x, x.pos.y, x.pos.z);
	//D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリクスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// 頂点バッファをデータストリーム設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	// 頂点フォーマット設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャ
	pDevice->SetTexture(0, m_pTexture);
	//	ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0, 2);
}
//============================================
// テクスチャUV設定
//============================================
void My::CObject3D::SetTexUV(::D3DXVECTOR2 tex0, ::D3DXVECTOR2 tex1)
{
	VERTEX_3D* pVtx;	// 頂点ポインタ
	// 頂点バッファロックをし、頂点情報を
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(tex0.x, tex0.y);
	pVtx[1].tex = D3DXVECTOR2(tex1.x, tex0.y);
	pVtx[2].tex = D3DXVECTOR2(tex0.x, tex1.y);
	pVtx[3].tex = D3DXVECTOR2(tex1.x, tex1.y);

	// 頂点ロックをアンロック
	m_pVtxBuff->Unlock();
}
//============================================
// 色設定
//============================================
void My::CObject3D::SetColor(D3DXCOLOR col)
{

	VERTEX_3D* pVtx;	// 頂点ポインタ
	// 頂点バッファロックをし、頂点情報を
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;
	// 頂点ロックをアンロック
	m_pVtxBuff->Unlock();
}
//============================================
// 分割設定設定
//============================================
void My::CObject3D::SetBlock(int nX, int nY)
{
	X x = GetX();
	VERTEX_3D* pVtx;	// 頂点ポインタ
	// 頂点バッファロックをし、頂点情報を
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3(-m_siz.x * x.scl.x * 0.5f * (float)nX, m_siz.y * x.scl.y * (float)nY, m_siz.z * x.scl.z * 0.5f * (float)nY);
	pVtx[1].pos = D3DXVECTOR3(m_siz.x * x.scl.x * 0.5f * (float)nX, m_siz.y * x.scl.y * (float)nY, m_siz.z * x.scl.z * 0.5f * (float)nY);
	pVtx[2].pos = D3DXVECTOR3(-m_siz.x * x.scl.x * 0.5f * (float)nX, 0 * (float)nY, -m_siz.z * x.scl.z * 0.5f * (float)nY);
	pVtx[3].pos = D3DXVECTOR3(m_siz.x * x.scl.x * 0.5f * (float)nX, 0 * (float)nY, -m_siz.z * x.scl.z * 0.5f * (float)nY);

	pVtx[0].tex = D3DXVECTOR2(0.0f * (float)nX, 0.0f * (float)nY);
	pVtx[1].tex = D3DXVECTOR2(1.0f * (float)nX, 0.0f * (float)nY);
	pVtx[2].tex = D3DXVECTOR2(0.0f * (float)nX, 1.0f * (float)nY);
	pVtx[3].tex = D3DXVECTOR2(1.0f * (float)nX, 1.0f * (float)nY);


	// 頂点ロックをアンロック
	m_pVtxBuff->Unlock();
}
//============================================
// テクスチャ設定
//============================================
void My::CObject3D::SetTexture(const char aFileName[MAX_TXT])
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	LPDIRECT3DTEXTURE9 pTex = nullptr;

	// テクスチャ
	D3DXCreateTextureFromFile(
		pDevice,
		aFileName,
		&pTex);
	if (pTex != nullptr)
	{
		SetTexture(pTex);
	}
}

void My::CObject3D::SetTexture(const LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}
//============================================
// UV追加
//============================================
void My::CObject3D::AddTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1)
{
	VERTEX_3D* pVtx;	// 頂点ポインタ
	// 頂点バッファロックをし、頂点情報を
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex += D3DXVECTOR2(tex0.x, tex0.y);
	pVtx[1].tex += D3DXVECTOR2(tex1.x, tex0.y);
	pVtx[2].tex += D3DXVECTOR2(tex0.x, tex1.y);
	pVtx[3].tex += D3DXVECTOR2(tex1.x, tex1.y);

	// 頂点ロックをアンロック
	m_pVtxBuff->Unlock();
}
//============================================
// 生成
//============================================
My::CObject3D* My::CObject3D::create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 siz)
{
	CObject3D* pObject3D = new CObject3D;

	pObject3D->SetPos(pos);
	pObject3D->SetRot(rot);
	pObject3D->SetSiz(siz);
	pObject3D->Init();

	return pObject3D;
}
