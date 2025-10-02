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
 * @brief コンストラクタ
 * @param [in]プライオリティ
 */
My::CObject3D::CObject3D(int nPriority):CObject(nPriority),
m_col(COLOR_WHITE),					//カラー初期化
m_pos(VEC3_RESET_ZERO),				//位置初期化
m_rot(VEC3_RESET_ZERO),				//方向初期化
m_size(VEC3_RESET_ZERO),			//サイズ初期化
m_tex_pos(VEC2_RESET_ZERO),			//テクスチャ座標初期化
m_mtxWorld(),						//ワールドマトリックス初期化
m_pTexture(),						//テクスチャ管理ポインタ初期化
m_pVtxBuff()						//頂点バッファ初期化
{
}

/**
 * @brief デストラクタ
 */
My::CObject3D::~CObject3D()
{
}

/**
 * @brief 初期化
 * @return 成功したか
 */
HRESULT My::CObject3D::Init()
{
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CObject3D::Uninit()
{
	if (m_pTexture != nullptr)
	{
		m_pTexture = nullptr;
	}

	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	Release();
}

/**
 * @brief 更新
 */
void My::CObject3D::Update()
{
}

/**
 * @brief 描画
 */
void My::CObject3D::Draw()
{
	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//マトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

/**
 * @brief テクスチャ設定
 * @param [in]テクスチャポインタ
 */
void My::CObject3D::BindTexture(LPDIRECT3DTEXTURE9 pTex)
{
	m_pTexture = pTex;
}

/**
 * @brief 頂点生成
 * @param [in]法線ベクトル
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
	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	//頂点座標の設定
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


	//法線の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = nor;

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
 * @brief 頂点生成(拡縮、回転)
 * @param [in]法線ベクトル
 * @param [in]方向
 * @param [in]頂点の距離
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
	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, reinterpret_cast<void**>(&pVtx), 0);

	//頂点座標の設定
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

	//法線の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = nor;

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