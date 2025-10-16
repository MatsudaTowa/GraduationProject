//===========================================
// 
// オブジェクトX[object_X.cpp]
// Auther:UedaKou
// 
//===========================================
#include "object_X.h"	// オブジェクトX
#include "../../system/manager.h"	// マネージャー

namespace My
{
	//********************************************
	// モデルX
	//********************************************
	CModelX* CModelX::m_pTop;
	CModelX* CModelX::m_pCur;

	//============================================
	// コンストラクタ
	//============================================
	CModelX::CModelX()
	{
		for (int nCnt = 0; nCnt < MAX_TEX_X; nCnt++)
		{
			m_pTexture[nCnt] = nullptr;
		}
		m_pMesh = nullptr;
		m_pBuffMat = nullptr;
		m_dwNumMat = 0;
	}
	//============================================
	// デストラ
	//============================================
	CModelX::~CModelX()
	{
	}
	//============================================
	// リリースする
	//============================================
	void CModelX::ReleaseAll()
	{
		CModelX* pModel = m_pTop;
		CModelX* pNext;
		while (pModel != nullptr)
		{
			pNext = pModel;
			if (pModel->m_pBuffMat != nullptr)
			{
				pModel->m_pBuffMat->Release();
				pModel->m_pBuffMat = nullptr;
			}
			if (pModel->m_pMesh != nullptr)
			{
				pModel->m_pMesh->Release();
				pModel->m_pMesh = nullptr;
			}
			for (int nCntTex = 0; nCntTex < MAX_MODEL_X; nCntTex++)
			{
				if (pModel->m_pTexture[nCntTex] != nullptr)
				{
					pModel->m_pTexture[nCntTex]->Release();
					pModel->m_pTexture[nCntTex] = nullptr;
				}
			}
			delete pModel;
			pModel = pNext;
		}
	}

	//============================================
	// 読み込み
	//============================================
	int CModelX::Load(const char* aFileName)
	{
		LPD3DXBUFFER pBuffMat = nullptr;	// マテリアルへのポインタ
		DWORD dwNumMat = 0;					// マテリアルの数
		LPD3DXMESH pMesh = nullptr;			// メッシュ(頂点情報)へのポインタ

		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス取得
		D3DXLoadMeshFromX(
			aFileName,				// モデル名
			D3DXMESH_SYSTEMMEM,
			pDevice,				// デバイス
			NULL,					// NULL
			&pBuffMat,				// バッファマトリクス
			NULL,					// NULL
			&dwNumMat,				// マテリアル数
			&pMesh);				// メッシュ

		if (pBuffMat == nullptr)
		{
			Beep(1200, 300);

			return -1;
		}

		CModelX* pModel = m_pTop;	// オブジェクト
		CModelX* pNext = nullptr;				// 次オブジェクト
		int nCnt = 0;
		while (pModel != nullptr)
		{// トップが有たら
			pNext = pModel->m_pNext;// 次を格納
			if (pModel->m_pMesh == pMesh &&
				pModel->m_pBuffMat == pBuffMat &&
				pModel->m_dwNumMat == dwNumMat)
			{// 同じデータがあったら
				return nCnt;
			}
			pModel = pNext;

			nCnt++;
		}
		// データの入っていない所まで繰り返したら
		if (m_pTop == nullptr)
		{// トップが空ならトップを入れる
			pModel = new CModelX;
			m_pTop = pModel;
			m_pCur = pModel;
		}
		else
		{
			pModel = new CModelX;
			pModel->m_pPrev = m_pCur;
			m_pCur->m_pNext = pModel;
			m_pCur = pModel;
		}
		pModel->m_pBuffMat = pBuffMat;
		pModel->m_dwNumMat = dwNumMat;
		pModel->m_pMesh = pMesh;

		D3DXMATERIAL* pMat = (D3DXMATERIAL*)pModel->m_pBuffMat->GetBufferPointer();	// マテリアルデータへのポインタを取得
		for (int nCntMat = 0; nCntMat < (int)pModel->m_dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != nullptr)
			{// マテリアルファイルが存在する
				// テクスチャ
				D3DXCreateTextureFromFile(
					pDevice,
					pMat[nCntMat].pTextureFilename,
					&pModel->m_pTexture[nCntMat]);
			}
		}
		return nCnt;
	}
	//============================================
	// モデル取得
	//============================================
	CModelX* CModelX::GetModel(int nID)
	{
		int nCnt = 0;
		CModelX* pObject, * pNext;//	オブジェクト
		pObject = m_pTop;
		while (pObject != nullptr)
		{
			pNext = pObject->m_pNext;
			if (nCnt == nID)
			{
				return pObject;
			}
			pObject = pNext;
			nCnt++;
		}
		return nullptr;
	}



	int CObjectX::m_nNum = 0;
	//============================================
	// コンストラクタ
	//============================================
	CObjectX::CObjectX()
	{
		m_nNum++;
	}
	CObjectX::CObjectX(int nPriority) : CObject(nPriority)
	{
		m_nNum++;
	}
	//============================================
	// デストラクタ
	//============================================
	CObjectX::~CObjectX()
	{
		m_nNum--;
	}
	//============================================
	// 初期化
	//============================================
	HRESULT CObjectX::Init()
	{
		return S_OK;
	}
	//============================================
	// 終了
	//============================================
	void CObjectX::Uninit()
	{
	}
	//============================================
	// 更新
	//============================================
	void CObjectX::Update()
	{
	}
	//============================================
	// 描画
	//============================================
	void CObjectX::Draw()
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイス取得
		D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリクス	
		D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
		D3DXMATERIAL* pMat;						// マテリアルデータへのポインタ
		CModelX* pModel = CModelX::GetModel(m_nID);
		X x = GetX();

		// ワールドマトリクスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// 向き
		D3DXMatrixRotationYawPitchRoll(&mtxRot, x.rot.y, x.rot.x, x.rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, x.pos.x, x.pos.y, x.pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアル取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pModel->GetBuffMtx()->GetBufferPointer();

		// マテリアルサイズ分繰り返す
		for (int nCntMat = 0; nCntMat < (int)pModel->GetNumMat(); nCntMat++)
		{
			// 色
			//pMat[nCntMat].MatD3D.Diffuse.g = 0.0f;
			//pMat[nCntMat].MatD3D.Diffuse.a = 0.1f;

			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, pModel->GetTexture(nCntMat));

			//モデル(パーツ)の描画
			pModel->GetMesh()->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

	}
	//============================================
	// 生成
	//============================================
	CObjectX* CObjectX::create(const char* aFileName)
	{
		int nID = 0;
		nID = CModelX::Load(aFileName);
		if (nID != 0)
		{

		}
		CObjectX* pObjectX = new CObjectX;
		pObjectX->SetID(nID);
		pObjectX->Init();

		return pObjectX;
	}
	CObjectX* CObjectX::create(string aFileName)
	{
		int nID = 0;
		nID = CModelX::Load(aFileName.c_str());
		if (nID != 0)
		{

		}
		CObjectX* pObjectX = new CObjectX;
		pObjectX->SetID(nID);
		pObjectX->Init();

		return pObjectX;
	}
}