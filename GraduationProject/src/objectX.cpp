//=============================================
//
//Xファイルオブジェクト[objectX.cpp]
//Author Matsuda Towa
//
//=============================================
#include "objectX.h"
#include "renderer.h"
#include "manager.h"
/**
 * @brief コンストラクタ
 * @param [in]プライオリティ
 */
My::CObjectX::CObjectX(int nPriority):CObject(nPriority),
m_scale(VEC3_RESET_ONE),			//スケール初期化
m_pos(VEC3_RESET_ZERO),				//位置初期化
m_rot(VEC3_RESET_ZERO),				//方向初期化
m_minpos(VEC3_RESET_ZERO),			//最小の頂点座標初期化
m_maxpos(VEC3_RESET_ZERO),			//最大の頂点座標初期化
m_mtxWorld(),						//ワールドマトリックス初期化
m_pMesh(),							//メッシュ情報初期化
m_pBuffMat(),						//マテリアル情報初期化
m_dwNumMat(INT_ZERO),				//マテリアル数初期化
m_col(COLOR_WHITE),					//カラー初期化
m_pTexture()						//テクスチャポインタ初期化		
{
}

/**
 * @brief デストラクタ
 */
My::CObjectX::~CObjectX()
{
}

/**
 * @brief 初期化
 * @return 成功したか
 */
HRESULT My::CObjectX::Init()
{
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CObjectX::Uninit()
{
	//メッシュの破棄
	if (m_pMesh != nullptr)
	{
		m_pMesh->Release();
		m_pMesh = nullptr;
	}

	//マテリアルの破棄
	if (m_pBuffMat != nullptr)
	{
		m_pBuffMat->Release();
		m_pBuffMat = nullptr;
	}

	for (int nCnt = INT_ZERO; nCnt < MAX_TEX; ++nCnt)
	{
		if (m_pTexture[nCnt] != nullptr)
		{
			m_pTexture[nCnt]->Release();
			m_pTexture[nCnt] = nullptr;
		}
	}
	Release();

}

/**
 * @brief 更新
 */
void My::CObjectX::Update()
{
}

/**
 * @brief 描画
 */
void My::CObjectX::Draw()
{
	if (m_pMesh != nullptr && m_pBuffMat != nullptr)
	{
		//デバイスの取得
		CRenderer* pRender = GET_RENDERER;
		LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
		D3DMATERIAL9 matDef; //現在のマテリアルの保存
		D3DXMATRIX mtxScale, mtxRot, mtxTrans; //計算用マトリックス

				//現在を取得
		pDevice->GetMaterial(&matDef);

		//マトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//αテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//スケールを反映
		D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 法線の自動正規化を有効に
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

		D3DXMATERIAL* pMat; //マテリアル

		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; ++nCntMat)
		{
			//今描画するマテリアル情報格納
			D3DMATERIAL9 DrawMat = pMat[nCntMat].MatD3D;

			DrawMat.Diffuse = m_col;

			//マテリアルの設定
			pDevice->SetMaterial(&DrawMat);

			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);

			//パーツの設定
			m_pMesh->DrawSubset(nCntMat);

		}

		//αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		// 法線の自動正規化を無効に
		pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);

		//保存してたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

/**
 * @brief テクスチャ設定
 * @param [in]テクスチャポインタ
 * @param [in]番号
 */
void My::CObjectX::BindTexture(LPDIRECT3DTEXTURE9 pTex, int Idx)
{
	m_pTexture[Idx] = pTex;
}

/**
 * @brief Xファイル設定
 * @param [in]マテリアル情報
 * @param [in]マテリアル数
 * @param [in]メッシュ情報
 */
void My::CObjectX::BindXFile(LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPD3DXMESH pMesh)
{
	pBuffMat->AddRef();
	pMesh->AddRef();

	m_pBuffMat = pBuffMat;
	m_dwNumMat = dwNumMat;
	m_pMesh = pMesh;

	D3DXMATERIAL* pMat; //マテリアルポインタ

	if (m_pBuffMat != nullptr)
	{
		pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

		for (int nCntMat = INT_ZERO; nCntMat < (int)m_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				LPDIRECT3DDEVICE9 pDevice = GET_RENDERER->GetDevice();
				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nCntMat]
				);

			}
		}
	}

	int nNumVtx; //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE* pVtxBuff; //頂点バッファのポインタ

		//頂点数の取得
	nNumVtx = m_pMesh->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	m_minpos = D3DXVECTOR3(100000.0f, 1000000.0f, 1000000.0f); //モデルの最小位置に値が入るよう極端な値代入
	m_maxpos = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); //モデルの最大位置に値が入るよう極端な値代入

	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtx; ++nCntVtx)
	{
		//頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//x座標の最大値最小値チェック
		if (vtx.x > m_maxpos.x)
		{
			m_maxpos.x = vtx.x;
		}
		if (vtx.x < m_minpos.x)
		{
			m_minpos.x = vtx.x;
		}

		//y座標の最大値最小値チェック
		if (vtx.y > m_maxpos.y)
		{
			m_maxpos.y = vtx.y;
		}
		if (vtx.y < m_minpos.y)
		{
			m_minpos.y = vtx.y;
		}

		//z座標の最大値最小値チェック
		if (vtx.z > m_maxpos.z)
		{
			m_maxpos.z = vtx.z;
		}
		if (vtx.z < m_minpos.z)
		{
			m_minpos.z = vtx.z;
		}

		// 次の頂点に進む
		pVtxBuff += sizeFVF;
	}

	m_pMesh->UnlockVertexBuffer();
}

/**
 * @brief 最小値取得
 * @return 頂点の最小値
 */
D3DXVECTOR3& My::CObjectX::GetMinPos()
{
	if (m_pMesh != nullptr && m_pBuffMat != nullptr)
	{
		int nNumVtx; //頂点数
		DWORD sizeFVF; //頂点フォーマットのサイズ
		BYTE* pVtxBuff; //頂点バッファのポインタ

			//頂点数の取得
		nNumVtx = m_pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		m_minpos = D3DXVECTOR3(100000.0f, 1000000.0f, 1000000.0f); //モデルの最小位置
		m_maxpos = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); //モデルの最大位置

		//頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; ++nCntVtx)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x座標の最小値チェック
			if (vtx.x < m_minpos.x)
			{
				m_minpos.x = vtx.x;
			}

			//y座標の最小値チェック
			if (vtx.y < m_minpos.y)
			{
				m_minpos.y = vtx.y;
			}

			//z座標の最小値チェック
			if (vtx.z < m_minpos.z)
			{
				m_minpos.z = vtx.z;
			}

			// 次の頂点に進む
			pVtxBuff += sizeFVF;
		}

		m_pMesh->UnlockVertexBuffer();
	}
	
	return m_minpos;
}

/**
 * @brief 最大値取得
 * @return 頂点の最大値
 */
D3DXVECTOR3& My::CObjectX::GetMaxPos()
{
	if (m_pMesh != nullptr && m_pBuffMat != nullptr)
	{
		int nNumVtx; //頂点数
		DWORD sizeFVF; //頂点フォーマットのサイズ
		BYTE* pVtxBuff; //頂点バッファのポインタ

			//頂点数の取得
		nNumVtx = m_pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		m_minpos = D3DXVECTOR3(100000.0f, 1000000.0f, 1000000.0f); //モデルの最小位置
		m_maxpos = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); //モデルの最大位置

	//頂点バッファのロック
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; ++nCntVtx)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//x座標の最大値チェック
			if (vtx.x > m_maxpos.x)
			{
				m_maxpos.x = vtx.x;
			}

			//y座標の最大値チェック
			if (vtx.y > m_maxpos.y)
			{
				m_maxpos.y = vtx.y;
			}

			//z座標の最大値チェック
			if (vtx.z > m_maxpos.z)
			{
				m_maxpos.z = vtx.z;
			}
			// 次の頂点に進む
			pVtxBuff += sizeFVF;
		}

		m_pMesh->UnlockVertexBuffer();
	}
	
	return m_maxpos;
}