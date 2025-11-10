//=============================================
//
//2Dオブジェクト[object2D.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 */
My::CObject2D::CObject2D(int nPriority):CObject(nPriority),
m_pos(VEC3_RESET_ZERO),		//座標の初期化
m_rot(VEC3_RESET_ZERO),		//方向の初期化
m_tex_pos(VEC2_RESET_ZERO),	//テクスチャ座標の初期化
m_col(COLOR_NONE),			//色の初期化
m_size(VEC2_RESET_ZERO),	//サイズの初期化
m_pTexture(nullptr),		//テクスチャ管理ポインタ初期化
m_pVtxBuff(nullptr)			//頂点バッファポインタ初期化
{
}

/**
 * @brief デストラクタ
 */
My::CObject2D::~CObject2D()
{
}

/**
 * @brief 初期化
 * @return 成功したか
 */
HRESULT My::CObject2D::Init()
{
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CObject2D::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//自分自身の解放
	Release();
}

/**
 * @brief 更新
 */
void My::CObject2D::Update()
{
}

/**
 * @brief 描画
 */
void My::CObject2D::Draw()
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);
	//テクスチャ拡大時に色を近似値にする
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		2);

	//テクスチャ拡大時の色を線形補間
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
}

/**
 * @brief 頂点の設定
 */
void My::CObject2D::SetVtx()
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	if (m_pVtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
	VERTEX_2D* pVtx;
	//頂点バッファをロックし頂点情報へのポインタを取得

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_size.x
		, m_pos.y - m_size.y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x
		, m_pos.y - m_size.y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_size.x
		, m_pos.y + m_size.y
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x
		, m_pos.y + m_size.y
		, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 
	pVtx[1].rhw = 
	pVtx[2].rhw = 
	pVtx[3].rhw = FLOAT_ONE;

	//頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//アンロック
	m_pVtxBuff->Unlock();

}

/**
 * @brief 拡縮と回転用頂点の設定
 * @param [in]方向
 * @param [in]頂点の距離
 */
void My::CObject2D::SetVtx(float fAngle, float fLength)
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	if (m_pVtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}

	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - (D3DX_PI - fAngle)) * fLength
		, m_pos.y + cosf(m_rot.z - (D3DX_PI - fAngle)) * fLength
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + (D3DX_PI - fAngle)) * fLength
		, m_pos.y + cosf(m_rot.z + (D3DX_PI - fAngle)) * fLength
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z - fAngle) * fLength
		, m_pos.y + cosf(m_rot.z - fAngle) * fLength
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf(m_rot.z + fAngle) * fLength
		, m_pos.y + cosf(m_rot.z + fAngle) * fLength
		, 0.0f);

	//rhwの設定
	pVtx[0].rhw =
	pVtx[1].rhw =
	pVtx[2].rhw =
	pVtx[3].rhw = FLOAT_ONE;

	//頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = m_col;

	//アンロック
	m_pVtxBuff->Unlock();
}

/**
 * @brief ゲージ用の頂点の設定
 */
void My::CObject2D::SetGaugeVtx()
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	if (m_pVtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	}
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(m_pos.x
		, m_pos.y - m_size.y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_size.x
		, m_pos.y - m_size.y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x
		, m_pos.y 
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_size.x
		, m_pos.y
		, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 
	pVtx[1].rhw = 
	pVtx[2].rhw = 
	pVtx[3].rhw = FLOAT_ONE;

	//頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = m_col;

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	m_pVtxBuff->Unlock();
}

/**
 * @brief テクスチャ設定
 * @param [in]テクスチャポインタ
 */
void My::CObject2D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}