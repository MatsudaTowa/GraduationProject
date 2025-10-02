//=============================================
//
//影「shadow.h」
// Author松田永久
//
//=============================================
#include "shadow.h"
#include "manager.h"

const std::string My::CShadow::TEXTURE_NAME = "data\\TEXTURE\\effect000.jpg";

//=============================================
//コンストラクタ
//=============================================
My::CShadow::CShadow(int nPriority):CObject3D(nPriority)
{
}

//=============================================
//デストラクタ
//=============================================
My::CShadow::~CShadow()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CShadow::Init()
{
	CObject3D::Init();
	//頂点設定
	SetVtx(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CShadow::Uninit()
{
	CObject3D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CShadow::Update()
{
	CObject3D::Update();
	//頂点設定
	SetVtx(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}

//=============================================
//描画
//=============================================
void My::CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

	//zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//減算合成処理
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject3D::Draw();

	//通常の合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================
//生成
//=============================================
My::CShadow* My::CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CShadow* pShadow = new CShadow;

	// nullならnullを返す
	if (pShadow == nullptr) { return nullptr; }

	//テクスチャ取得
	CTexture* pTexture = GET_TEXTURE;

	pShadow->SetPos(pos); //pos設定
	pShadow->SetSize(size); //サイズ設定
	pShadow->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME))); //テクスチャ設定
	pShadow->SetType(OBJECT_TYPE_SHADOW); //タイプ設定
	pShadow->Init(); //影の初期化処理

	return pShadow;
}
