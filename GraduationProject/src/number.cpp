//=============================================
//
//数字管理「number.cpp」
// Author松田永久
//
//=============================================
#include "number.h"
#include "manager.h"

namespace
{
	const int NUM_TEXTURE = 2;
	const std::string TEXTURE_NAME[NUM_TEXTURE]
	{
		 "data\\TEXTURE\\number000.png",
		 "data\\TEXTURE\\number000.png"
	};
}

//=============================================
//コンストラクタ
//=============================================
My::CNumber_2D::CNumber_2D(int nPriority) : CObject2D(nPriority),
m_mintex(VEC2_RESET_ZERO),			//テクスチャの最小座標
m_maxtex(VEC2_RESET_ZERO)			//テクスチャの最大座標
{
}

//=============================================
//デストラクタ
//=============================================
My::CNumber_2D::~CNumber_2D()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CNumber_2D::Init()
{
	//親クラスの初期化
	CObject2D::Init();

	//頂点生成
	NumberVtx();

	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CNumber_2D::Uninit()
{
	//親クラスの終了
	CObject2D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CNumber_2D::Update()
{
	//親クラスの更新
	CObject2D::Update();

	//頂点生成
	NumberVtx();
}

//=============================================
//描画
//=============================================
void My::CNumber_2D::Draw()
{
	//親クラスの描画
	CObject2D::Draw();
}

//=============================================
//生成
//=============================================
My::CNumber_2D* My::CNumber_2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size,int idx)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CNumber_2D* pNumber = new CNumber_2D;

	// nullならnullを返す
	if (pNumber == nullptr) { return nullptr; }

	pNumber->SetPos(pos); //pos設定

	pNumber->SetSize(size); //size設定

	pNumber->SetType(OBJECT_TYPE_NUMBER); //タイプ設定

	pNumber->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME[idx]))); //テクスチャ設定

	pNumber->Init(); //初期化処理

	return pNumber;
}

//=============================================
//数字の設定
//=============================================
void My::CNumber_2D::SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col)
{
	m_mintex.x = fNumPos1;
	m_maxtex.x = fNumPos2;
	SetColor(col);
}

//=============================================
//数字専用の頂点生成
//=============================================
void My::CNumber_2D::NumberVtx()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	LPDIRECT3DVERTEXBUFFER9 pBuff = GetVtxBuff();

	if (pBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &pBuff, NULL);
	}
	VERTEX_2D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	pBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(GetPos().x-GetSize().x
		, GetPos().y -GetSize().y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetPos().x + GetSize().x
		, GetPos().y -GetSize().y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GetPos().x -GetSize().x
		, GetPos().y + GetSize().y
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetPos().x+GetSize().x
		, GetPos().y + GetSize().y
		, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = GetColor();
	pVtx[1].col = GetColor();
	pVtx[2].col = GetColor();
	pVtx[3].col = GetColor();

	pVtx[0].tex = D3DXVECTOR2(m_mintex.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_maxtex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_mintex.x, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_maxtex.x, 1.0f);

	//アンロック
	pBuff->Unlock();

	SetVtxBuff(pBuff);
}

//texパス
const std::string My::CNumber_3D::TEXTURE_NAME = "data\\TEXTURE\\number000.png";

//=============================================
//コンストラクタ
//=============================================
My::CNumber_3D::CNumber_3D(int nPriority):CBillboard(nPriority), m_mintex(D3DXVECTOR2(0.0f, 0.0f)), m_maxtex(D3DXVECTOR2(0.0f, 0.0f))
{
}

//=============================================
//デストラクタ
//=============================================
My::CNumber_3D::~CNumber_3D()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CNumber_3D::Init()
{
	//親クラスの初期化
	CObject3D::Init();

	//頂点生成
	NumberVtx();

	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CNumber_3D::Uninit()
{
	//親クラスの終了
	CObject3D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CNumber_3D::Update()
{
	//親クラスの更新
	CObject3D::Update();

	//頂点生成
	NumberVtx();
}

//=============================================
//描画
//=============================================
void My::CNumber_3D::Draw()
{
	//親クラスの描画
	CBillboard::Draw();

}

//=============================================
//生成
//=============================================
My::CNumber_3D* My::CNumber_3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CNumber_3D* pNumber = new CNumber_3D;

	// nullならnullを返す
	if (pNumber == nullptr) { return nullptr; }

	pNumber->SetPos(pos); //pos設定

	pNumber->SetSize(size); //size設定

	pNumber->SetType(OBJECT_TYPE_NUMBER); //タイプ設定

	pNumber->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME))); //テクスチャ設定

	pNumber->Init(); //初期化処理

	return pNumber;
}

//=============================================
//数字の設定
//=============================================
void My::CNumber_3D::SetNumber(float fNumPos1, float fNumPos2, D3DXCOLOR col)
{
	m_mintex.x = fNumPos1;
	m_maxtex.x = fNumPos2;
	SetColor(col);
}

//=============================================
//数字専用の頂点生成TODO:Objectのほうで
//=============================================
void My::CNumber_3D::NumberVtx()
{
	CRenderer* pRender = CManager::GetInstance()->GetRenderer();

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 pBuff = GetVtxBuff();

	if (pBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &pBuff, NULL);
	}
	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	pBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-GetSize().x
		,GetSize().y
		, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetSize().x
		,GetSize().y
		, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetSize().x
		,-GetSize().y
		, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+GetSize().x
		,-GetSize().y
		, 0.0f);

	//rhwの設定
	pVtx[0].nor = 
	pVtx[1].nor = 
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);;

	//頂点カラーの設定
	pVtx[0].col =
	pVtx[1].col =
	pVtx[2].col =
	pVtx[3].col = GetColor();

	pVtx[0].tex = D3DXVECTOR2(m_mintex.x, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_maxtex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_mintex.x, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_maxtex.x, 1.0f);

	//アンロック
	pBuff->Unlock();

	SetVtxBuff(pBuff);
}
