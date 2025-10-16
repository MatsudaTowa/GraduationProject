//===========================================
// 
// ポリゴン2[object_2D.cpp]
// Auther:UedaKou
// 
//===========================================
#include "object_2D.h"	// オブジェクト２D
#include "../../system/manager.h"	// 全体マネージャー

#include "../../system/main.h"	// メイン

namespace My
{
	// 静的メンバ定数
	const D3DXCOLOR CObject2D::s_colDef = { 1.0f, 1.0f, 1.0f, 1.0f };	// 色初期設定
	const int CObject2D::s_nNumVertices = 4;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CObject2D::CObject2D()
	{
		m_pVtxBuff = nullptr;				// バーテックス初期化
		m_pTexture = nullptr;				// テクスチャ初期化
		m_col = s_colDef;					// 色初期化
		m_nNumVertices = s_nNumVertices;	// 超点数
	}
	/// <summary>
	/// コンストラクタ(優先度付き)
	/// </summary>
	/// <param name="nPriority">優先度</param>
	CObject2D::CObject2D(int nPriority) :
		CObject(nPriority)
	{
		m_pVtxBuff = nullptr;				// バーテックス初期化
		m_pTexture = nullptr;				// テクスチャ初期化
		m_col = s_colDef;					// 色初期化
		m_nNumVertices = s_nNumVertices;	// 超点数
	}
	//============================================
	// デストラクト
	//============================================
	CObject2D::~CObject2D()
	{
		// 初期化処理
		Uninit();
	}
	//============================================
	// 初期化
	//============================================
	HRESULT CObject2D::Init()
	{
		// 全体マネージャー取得
		CManager* pManager = CManager::GetInstance();
		// デバイス取得
		LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();// デバイスへのポインタ

		// トランスフォーム取得
		X x = GetX();

		// デバイスの取得
		pDevice->CreateVertexBuffer(
			sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			NULL);


		VERTEX_2D* pVtx;		// 頂点情報へのポインタ
		// 頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標更新
		pVtx[0].pos = D3DXVECTOR3(x.pos.x - (m_siz.x * x.scl.x * 0.5f), x.pos.y - (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(x.pos.x + (m_siz.x * x.scl.x * 0.5f), x.pos.y - (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(x.pos.x - (m_siz.x * x.scl.x * 0.5f), x.pos.y + (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(x.pos.x + (m_siz.x * x.scl.x * 0.5f), x.pos.y + (m_siz.y * x.scl.y * 0.5f), 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点バッファをロック
		m_pVtxBuff->Unlock();
		return S_OK;

	}
	//============================================
	// 終了
	//============================================
	void CObject2D::Uninit()
	{
		// テクスチャの破棄
		if (m_pTexture != nullptr)
		{
			m_pTexture->Release();
			m_pTexture = nullptr;
		}

		// 頂点バッファの破棄
		if (m_pVtxBuff != nullptr)
		{
			m_pVtxBuff->Release();
			m_pVtxBuff = nullptr;
		}
	}
	//============================================
	// 更新
	//============================================
	void CObject2D::Update()
	{
	}
	//============================================
	// 描画
	//============================================
	void CObject2D::Draw()
	{
		// 全体マネージャー
		CManager* pManager = CManager::GetInstance();
		// デバイスへのポインタ
		LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();

		// 頂点バッファにデータをストリームに設定
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャ
		pDevice->SetTexture(0, m_pTexture);

		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}
	/// <summary>
	/// トランスフォームを設定
	/// </summary>
	/// <param name="x">設定するトランスフォーム</param>
	void CObject2D::SetX(X x)
	{
		// トランスフォーム設定
		CObject::SetX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// トランスフォームを設定
	/// </summary>
	/// <param name="pos">設定する位置</param>
	/// <param name="rot">設定する向き</param>
	/// <param name="siz">設定するスケール</param>
	void CObject2D::SetX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 scl)
	{
		// トランスフォーム設定
		CObject::SetPos(pos);
		CObject::SetRot(rot);
		CObject::SetScl(scl);

		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 位置を設定します
	/// </summary>
	/// <param name="pos">設定する座標</param>
	void CObject2D::SetPos(D3DXVECTOR3 pos)
	{
		// 位置設定
		CObject::SetPos(pos);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 位置（x軸のみ）を設定
	/// </summary>
	/// <param name="x">設定する座標</param>
	void CObject2D::SetPosX(const float x)
	{
		CObject::SetPosX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 位置（y軸のみ）設定
	/// </summary>
	/// <param name="y">設定する座標</param>
	void CObject2D::SetPosY(const float y)
	{
		CObject::SetPosY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 位置（z軸のみ）を設定
	/// </summary>
	/// <param name="z">設定する座標</param>
	void CObject2D::SetPosZ(const float z)
	{
		CObject::SetPosZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 向きを設定する
	/// </summary>
	/// <param name="rot">設定したい角度</param>
	void CObject2D::SetRot(const D3DXVECTOR3 rot)
	{
		CObject::SetRot(rot);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 向き（x軸のみ）を設定する
	/// </summary>
	/// <param name="x">設定したい角度</param>
	void CObject2D::SetRotX(const float x)
	{
		CObject::SetRotX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 向き（y軸のみ）を設定する
	/// </summary>
	/// <param name="y">設定したい角度</param>
	void CObject2D::SetRotY(const float y)
	{
		CObject::SetRotY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 向き（z軸のみ）を設定する
	/// </summary>
	/// <param name="z">設定したい角度</param>
	void CObject2D::SetRotZ(const float z)
	{
		CObject::SetRotZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 拡大率を設定
	/// </summary>
	/// <param name="siz">設定したい拡大率</param>
	void CObject2D::SetScl(const D3DXVECTOR3 siz)
	{
		CObject::SetScl(siz);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 拡大率(x軸のみ)を設定
	/// </summary>
	/// <param name="x">設定したい拡大率</param>
	void CObject2D::SetSclX(const float x)
	{
		CObject::SetSclX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 拡大率(x軸のみ)を設定
	/// </summary>
	/// <param name="y">設定したい拡大率</param>
	void CObject2D::SetSclY(const float y)
	{
		CObject::SetSclY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// 拡大率(x軸のみ)を設定
	/// </summary>
	/// <param name="z">設定したい拡大率</param>
	void CObject2D::SetSclZ(const float z)
	{
		// 拡大率設定
		CObject::SetSclZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		// 位置から頂点を設定
		SetVtxPos();
	}
	/// <summary>
	/// UV設定
	/// </summary>
	/// <param name="UV">設定したいUV（上下左右）</param>
	void CObject2D::SetUV(D3DXVECTOR4 UV)
	{
		VERTEX_2D* pVtx;		// 頂点情報へのポインタ
		// 頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(UV.z, UV.x);
		pVtx[1].tex = D3DXVECTOR2(UV.w, UV.x);
		pVtx[2].tex = D3DXVECTOR2(UV.z, UV.y);
		pVtx[3].tex = D3DXVECTOR2(UV.w, UV.y);

		// 頂点バッファをロック
		m_pVtxBuff->Unlock();
	}
	/// <summary>
	/// 頂点カラー設定
	/// </summary>
	/// <param name="col">設定したい頂点カラー</param>
	void CObject2D::SetColor(D3DXCOLOR col)
	{
		m_col = col;


		VERTEX_2D* pVtx;		// 頂点情報へのポインタ
		if (m_pVtxBuff != nullptr)
		{
			// 頂点バッファをロック
			m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// 頂点カラー設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			// 頂点バッファをロック
			m_pVtxBuff->Unlock();
		}
	}
	/// <summary>
	/// テクスチャを設定する
	/// 外部から取得したDirect3Dテクスチャを直接登録する
	/// </summary>
	/// <param name="ptex">Direct3Dテクスチャポインタ</param>
	void CObject2D::SetTexture(const LPDIRECT3DTEXTURE9 ptex)
	{
		// テクスチャポインタを設定
		m_pTexture = ptex;
	}
	/// <summary>
	/// テクスチャファイルを読み込んで設定する（std::string版）
	/// </summary>
	/// <param name="aName">テクスチャファイルパス</param>
	void CObject2D::SetTexture(const std::string aName)
	{
		// Direct3Dデバイスを取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイス取得

		LPDIRECT3DTEXTURE9 ptex;

		// テクスチャファイルを読み込む
		D3DXCreateTextureFromFile(
			pDevice,
			aName.c_str(),
			&ptex);

		// 読み込んだテクスチャを設定
		SetTexture(ptex);
	}
	/// <summary>
	/// トランスフォーム加算
	/// </summary>
	/// <param name="x">加算する座標・回転・スケールのセット</param>
	void CObject2D::AddX(X x)
	{
		CObject::AddX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// トランスフォームに指定値を加算する
	/// </summary>
	/// <param name="x">加算する座標・回転・スケールのセット</param>
	void CObject2D::AddX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 siz)
	{
		CObject::AddX(pos, rot, siz);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// 座標加算
	/// </summary>
	/// <param name="pos">加算したい座標</param>
	void CObject2D::AddPos(const D3DXVECTOR3 pos)
	{
		CObject::AddPos(pos);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// X座標を加算
	/// </summary>
	/// <param name="x">加算したい座標</param>
	void CObject2D::AddPosX(const float x)
	{
		CObject::AddPosX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Y座標を加算
	/// </summary>
	/// <param name="y">加算したい座標</param>
	void CObject2D::AddPosY(const float y)
	{
		CObject::AddPosY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Z座標を加算
	/// </summary>
	/// <param name="z">加算したい座標</param>
	void CObject2D::AddPosZ(const float z)
	{
		CObject::AddPosZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// 回転を加算（全軸）
	/// </summary>
	/// <param name="rot">加算したい回転（ラジアン単位）</param>
	void CObject2D::AddRot(const D3DXVECTOR3 rot)
	{
		CObject::AddRot(rot);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// X軸回転を加算
	/// </summary>
	/// <param name="x">加算したいX軸回転（ラジアン単位）</param>
	void CObject2D::AddRotX(const float x)
	{
		CObject::AddRotX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Y軸回転を加算
	/// </summary>
	/// <param name="y">加算したいY軸回転（ラジアン単位）</param>
	void CObject2D::AddRotY(const float y)
	{
		CObject::AddRotY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Z軸回転を加算
	/// </summary>
	/// <param name="z">加算したいZ軸回転（ラジアン単位）</param>
	void CObject2D::AddRotZ(const float z)
	{
		CObject::AddRotZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// スケールを加算
	/// </summary>
	/// <param name="siz">加算したいスケール</param>
	void CObject2D::AddScl(const D3DXVECTOR3 siz)
	{
		CObject::AddScl(siz);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// X軸スケールを加算
	/// </summary>
	/// <param name="x">加算したいX軸スケール</param>
	void CObject2D::AddSclX(const float x)
	{
		CObject::AddSclX(x);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Y軸スケールを加算
	/// </summary>
	/// <param name="y">加算したいX軸スケール</param>
	void CObject2D::AddSclY(const float y)
	{
		CObject::AddSclY(y);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// Z軸スケールを加算
	/// </summary>
	/// <param name="z">加算したいX軸スケール</param>
	void CObject2D::AddSclZ(const float z)
	{
		CObject::AddSclZ(z);
		if (m_pVtxBuff == nullptr)
		{
			return;
		}
		SetVtxPos();
	}
	/// <summary>
	/// 生成
	/// </summary>
	/// <param name="pos">生成したい位置</param>
	/// <param name="siz">生成したい大きさ</param>
	/// <returns>生成したオブジェクト</returns>
	CObject2D* CObject2D::create(D3DXVECTOR3 pos, D3DXVECTOR3 siz)
	{
		// インスタンスを確保
		CObject2D* pObject = new CObject2D;
		// インスタンスを確保を失敗していたら
		if (!pObject)
		{
			return nullptr;
		}

		pObject->SetPos(pos);	// 位置を設定
		pObject->SetSiz(siz);	// 大きさ設定
		if (!pObject->Init())	// 初期化
		{
			delete pObject;
			pObject = nullptr;
			return nullptr;
		}

		// インスタンスを返す
		return pObject;
	}
	/// <summary>
	/// 生成優先度付き
	/// </summary>
	/// <param name="nPriority">優先度</param>
	/// <param name="pos">生成したい位置</param>
	/// <param name="siz">生成したい大きさ</param>
	/// <returns>生成したオブジェクト</returns>
	CObject2D* CObject2D::create(int nPriority, D3DXVECTOR3 pos, D3DXVECTOR3 siz)
	{
		// インスタンスを確保
		CObject2D* pObject = new CObject2D(nPriority);

		pObject->SetPos(pos);	// 位置を設定
		pObject->SetSiz(siz);	// 大きさ設定
		if (!pObject->Init())	// 初期化
		{
			delete pObject;
			pObject = nullptr;
			return nullptr;
		}

		// インスタンスを返す
		return pObject;
	}
	/// <summary>
	/// 頂点情報を設定
	/// </summary>
	void CObject2D::SetVtxPos()
	{
		//位置を設定
		X x = GetX();

		VERTEX_2D* pVtx;		// 頂点情報へのポインタ
		// 頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標更新
		pVtx[0].pos = D3DXVECTOR3(x.pos.x - (m_siz.x * x.scl.x * 0.5f), x.pos.y - (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(x.pos.x + (m_siz.x * x.scl.x * 0.5f), x.pos.y - (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(x.pos.x - (m_siz.x * x.scl.x * 0.5f), x.pos.y + (m_siz.y * x.scl.y * 0.5f), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(x.pos.x + (m_siz.x * x.scl.x * 0.5f), x.pos.y + (m_siz.y * x.scl.y * 0.5f), 0.0f);

		// 頂点バッファをロック
		m_pVtxBuff->Unlock();
	}
}