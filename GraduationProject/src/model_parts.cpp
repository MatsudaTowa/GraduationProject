//=============================================
// 
//親子関係のパーツ[model.h]
//Author Matsuda Towa
//
//=============================================
#include "model_parts.h"
#include "manager.h"

//=============================================
//コンストラクタ
//=============================================
My::CModel_Parts::CModel_Parts():m_oldpos(),m_ModelInfo()
{
}

//=============================================
//デストラクタ
//=============================================
My::CModel_Parts::~CModel_Parts()
{
}

//=============================================
//削除
//=============================================
void My::CModel_Parts::Unload()
{
	for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->s_dwNumMat; nCntMat++)
	{
		if (m_pTexture[nCntMat] != nullptr)
		{
			m_pTexture[nCntMat]->Release();
			m_pTexture[nCntMat] = nullptr;
		}
	}
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		if (m_ModelInfo[nCnt].s_pBuffMat != nullptr)
		{//マテリアル破棄
			m_ModelInfo[nCnt].s_pBuffMat->Release();
			m_ModelInfo[nCnt].s_pBuffMat = nullptr;
			m_ModelInfo[nCnt].s_dwNumMat = 0;
		}
		if (m_ModelInfo[nCnt].s_pMesh != nullptr)
		{//メッシュ破棄
			m_ModelInfo[nCnt].s_pMesh->Release();
			m_ModelInfo[nCnt].s_pMesh = nullptr;
		}

		if (m_ModelInfo[nCnt].s_ModelName != nullptr)
		{//モデルネーム破棄
			m_ModelInfo[nCnt].s_ModelName = nullptr;
		}
	}

	if (CManager::GetInstance()->GetModel() != nullptr)
	{
		CManager::GetInstance()->GetModel()->Unload();
	}
	
	m_nNumAll = 0;
	delete this;
}

//=============================================
//モデル設定
//=============================================
void My::CModel_Parts::BindXFile(LPD3DXBUFFER pBuffMat, DWORD dwNumMat, LPD3DXMESH pMesh)
{

	pBuffMat->AddRef();
	pMesh->AddRef();

	m_ModelInfo->s_pBuffMat = pBuffMat;
	m_ModelInfo->s_dwNumMat = dwNumMat;
	m_ModelInfo->s_pMesh = pMesh;

	int nNumVtx; //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE* pVtxBuff; //頂点バッファのポインタ

		//頂点数の取得
	nNumVtx = m_ModelInfo->s_pMesh->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_ModelInfo->s_pMesh->GetFVF());

	m_minpos = D3DXVECTOR3(100000.0f, 1000000.0f, 1000000.0f); //モデルの最小位置
	m_maxpos = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); //モデルの最大位置

	//頂点バッファのロック
	m_ModelInfo->s_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
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

	m_ModelInfo->s_pMesh->UnlockVertexBuffer();
}

//=============================================
//描画
//=============================================
void My::CModel_Parts::Draw()
{
	if (m_ModelInfo->s_pMesh != nullptr && m_ModelInfo->s_pBuffMat != nullptr)
	{
		//デバイスの取得
		CRenderer* pRender = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
		D3DMATERIAL9 matDef; //現在のマテリアルの保存
		D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

		//マトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//αテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		D3DXMATRIX mtxParent; //親の行列取得

		if (m_pParent != nullptr)
		{
			//親のワールドマトリックス取得
			mtxParent = m_pParent->GetMtxWorld();
		}
		else
		{
			//最新のワールド返還行列を取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		D3DXMATERIAL* pMat; //マテリアル

		pMat = (D3DXMATERIAL*)m_ModelInfo->s_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->s_dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);

			//パーツの設定
			m_ModelInfo->s_pMesh->DrawSubset(nCntMat);
		}
		//αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		//現在を取得
		pDevice->GetMaterial(&matDef);

		//保存してたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================
//描画(materialの色変更)
//=============================================
void My::CModel_Parts::Draw(D3DXCOLOR col)
{
	if (m_ModelInfo->s_pMesh != nullptr && m_ModelInfo->s_pBuffMat != nullptr)
	{
		//デバイスの取得
		CRenderer* pRender = CManager::GetInstance()->GetRenderer();
		LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
		D3DMATERIAL9 matDef; //現在のマテリアルの保存
		D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

		//マトリックスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		//αテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		D3DXMATRIX mtxParent; //親の行列取得

		if (m_pParent != nullptr)
		{
			//親のワールドマトリックス取得
			mtxParent = m_pParent->GetMtxWorld();
		}
		else
		{
			//最新のワールド返還行列を取得
			pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
		}

		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		D3DXMATERIAL* pMat; //マテリアル

		pMat = (D3DXMATERIAL*)m_ModelInfo->s_pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_ModelInfo->s_dwNumMat; nCntMat++)
		{
			//今描画するマテリアル情報格納
			D3DMATERIAL9 DrawMat = pMat[nCntMat].MatD3D;

			DrawMat.Diffuse = col;

			//マテリアルの設定
			pDevice->SetMaterial(&DrawMat);

			//テクスチャの設定
			pDevice->SetTexture(0, m_pTexture[nCntMat]);

			//パーツの設定
			m_ModelInfo->s_pMesh->DrawSubset(nCntMat);
		}
		//αテストを無効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

		//現在を取得
		pDevice->GetMaterial(&matDef);

		//保存してたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
}

//=============================================
//ワールドマトリックスを変換する
//=============================================
void My::CModel_Parts::ConversionMtxWorld()
{
	//デバイスの取得
	CRenderer* pRender = CManager::GetInstance()->GetRenderer();
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

	D3DXMATRIX mtxParent; //親の行列取得

	if (m_pParent != nullptr)
	{
		//親のワールドマトリックス取得
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		//最新のワールド返還行列を取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

}


//=============================================
//生成
//=============================================
My::CModel_Parts* My::CModel_Parts::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,char* pModel_Path)
{
	CModel_Parts*pModelParts = new CModel_Parts;
	CModel* pModel = CManager::GetInstance()->GetModel();
	if (pModelParts == nullptr)
	{
		return nullptr;
	}

	pModelParts->m_pos = pos; //頂点代入
	pModelParts->m_rot = rot; //頂点代入

	//Xファイル読み込み
	pModelParts->BindXFile(pModel->GetModelInfo(pModel->Regist(pModel_Path)).pBuffMat, //マテリアル取得
		pModel->GetModelInfo(pModel->Regist(pModel_Path)).dwNumMat, //マテリアル数取得
		pModel->GetModelInfo(pModel->Regist(pModel_Path)).pMesh); //メッシュ情報取得

	return pModelParts;
}

//=============================================
//親パーツの設定
//=============================================
void My::CModel_Parts::SetParent(CModel_Parts* pParent)
{
	m_pParent = pParent;
}

//=============================================
//ワールドマトリックスの取得
//=============================================
D3DXMATRIX& My::CModel_Parts::GetMtxWorld()
{
	return m_mtxWorld;
}

//=============================================
//モデル情報取得
//=============================================
My::CModel_Parts::MODEL_INFO My::CModel_Parts::GetModelInfo(int nIdx)
{
	return m_ModelInfo[nIdx];
}

