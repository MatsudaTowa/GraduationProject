//============================================================================================================
//
// 体力UI枠 「life_frame.cpp」
// Author : 笹生一颯
//
//============================================================================================================

//=================================================================
// ヘッダーファイルインクルード
//=================================================================
#include "life_frame.h"
#include "life_gauge.h"
#include "life_changer.h"
namespace
{
	const std::string TEX_PATH = "data\\TEXTURE\\UI\\life_ui.png";
}

//=================================================================
// コンストラクタ
//=================================================================
My::CLife_frame::CLife_frame(int nPriority) : CObject2D(nPriority)
{
}

//=================================================================
// デストラクタ
//=================================================================
My::CLife_frame::~CLife_frame()
{

}

//=================================================================
// 初期化
//=================================================================
HRESULT My::CLife_frame::Init()
{

	CObject2D::Init();

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	//テクスチャ登録
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(TEX_PATH)));//テクスチャ設定

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();


	return S_OK;
}

//=================================================================
// 終了
//=================================================================
void My::CLife_frame::Uninit()
{
	//インスタンス取得
	CLife_changer* pLife = CLife_changer::GetInstance();

	//ライフな奴らの更新
	pLife->Uninit();
	CObject2D::Uninit();
}

//=================================================================
// 更新
//=================================================================
void My::CLife_frame::Update()
{
	//インスタンス取得
	CLife_changer* pLife = CLife_changer::GetInstance();

	//ライフな奴らの更新
	pLife->Update();

	//体力に応じたテクスチャチェンジ
	float L_Raito = pLife->GetLifeRaito();
}

//=================================================================
// 描画
//=================================================================
void My::CLife_frame::Draw()
{
	CObject2D::Draw();
}
//=================================================================
// 生成
//=================================================================
My::CLife_frame* My::CLife_frame::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CLife_frame* pLifeFrame = new CLife_frame(11);
	pLifeFrame->SetPos(pos);
	pLifeFrame->SetSize(size);
	pLifeFrame->SetType(CObject::OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	pLifeFrame->Init();
	return pLifeFrame;
}