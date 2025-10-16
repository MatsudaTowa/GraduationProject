//============================================
//
// ビルボード[billbord.cpp]
// Author:Uedakou
// 
//============================================
#include "object_billboard.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <strsafe.h>

namespace My
{

	//============================================
	// コンスト
	//============================================
	CObjectBillbord::CObjectBillbord()
	{
		m_pTexture = nullptr;
		m_pVtxBuff = nullptr;

	}
	CObjectBillbord::CObjectBillbord(const int nPriority) :
		CObject(nPriority)
	{
		m_pTexture = nullptr;
		m_pVtxBuff = nullptr;

	}
	//============================================
	// デストラクタ
	//============================================
	CObjectBillbord::~CObjectBillbord()
	{
	}
	//============================================
	// 初期化
	//============================================
	HRESULT CObjectBillbord::Init()
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイスへのポインタ

		// 頂点バッファの生成
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
		X x = GetX();

		VERTEX_3D* pVtx;	// 頂点ポインタ
		// 頂点バッファロックをし、頂点情報を
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos = D3DXVECTOR3(-x.scl.x * 0.5f, x.scl.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(x.scl.x * 0.5f, x.scl.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-x.scl.x * 0.5f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(x.scl.x * 0.5f, 0.0f, 0.0f);

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
	// 終了処理
	//============================================
	void CObjectBillbord::Uninit()
	{
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
	void CObjectBillbord::Update()
	{
	}
	//============================================
	// 描画
	//============================================
	void CObjectBillbord::Draw()
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		D3DXMATRIX mtxTrans;  // 計算用マトリクス
		D3DXMATRIX matView;

		X x = GetX();
		if (!pDevice) {
			// デバイスが無効な場合のエラーハンドリング
			return;
		}

		// ワールドマトリクスの初期化
		D3DXMatrixIdentity(&m_mtxWorld);

		// ビューマトリクスの取得
		if (FAILED(pDevice->GetTransform(D3DTS_VIEW, &matView))) {
			// ビューマトリクス取得失敗時の処理
			return;
		}

		// ビューマトリクスの逆行列を計算
		if (D3DXMatrixInverse(&m_mtxWorld, NULL, &matView) == NULL) {
			// 逆行列計算失敗時の処理
			return;
		}

		// ワールドマトリクスの位置を初期化
		m_mtxWorld._41 = 0.0f;
		m_mtxWorld._42 = 0.0f;
		m_mtxWorld._43 = 0.0f;

		// オブジェクトの位置を反映
		D3DXMatrixTranslation(&mtxTrans, x.pos.x, x.pos.y, x.pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 頂点バッファを設定
		if (m_pVtxBuff) {
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
		}
		else {
			// 頂点バッファが無効な場合の処理
			return;
		}

		// Zバッファの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		// Zバッファの比較を通常の比較方法に戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		// アルファテストを有効化
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

		// 頂点フォーマット設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャ設定
		pDevice->SetTexture(0, m_pTexture);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// Zバッファ設定を元に戻す
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// アルファテストを無効化
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	}
	//============================================
	// テクスチャUV設定
	//============================================
	void CObjectBillbord::SetTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1)
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
	void CObjectBillbord::SetColor(D3DXCOLOR col)
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
	// テクスチャ設定
	//============================================
	void CObjectBillbord::SetTexture(const char aFileName[MAX_TXT])
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

	void CObjectBillbord::SetTexture(const LPDIRECT3DTEXTURE9 pTex)
	{
		m_pTexture = pTex;
	}
	//============================================
	// UV追加
	//============================================
	void CObjectBillbord::AddTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1)
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
	CObjectBillbord* CObjectBillbord::create(D3DXVECTOR3 pos, D3DXVECTOR3 siz)
	{
		CObjectBillbord* pBillbord = new CObjectBillbord();


		pBillbord->SetPos(pos);
		pBillbord->SetScl(siz);


		pBillbord->Init();

		return pBillbord;
	}
	CObjectBillbord* CObjectBillbord::create(int nPriorith, D3DXVECTOR3 pos, D3DXVECTOR3 siz)
	{
		CObjectBillbord* pBillbord = new CObjectBillbord(nPriorith);


		pBillbord->SetPos(pos);
		pBillbord->SetScl(siz);


		pBillbord->Init();

		return pBillbord;
	}
}