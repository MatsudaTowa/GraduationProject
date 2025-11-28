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
	const std::string LIFE_100 = "data\\TEXTURE\\UI\\HP_100~75.png";
	const std::string LIFE_75 = "data\\TEXTURE\\UI\\HP_75~50.png";
	const std::string LIFE_50 = "data\\TEXTURE\\UI\\HP_50~25.png";
	const std::string LIFE_25 = "data\\TEXTURE\\UI\\HP_25.png";
	const std::string DEATH = "data\\TEXTURE\\UI\\HP_0.png";
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
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(LIFE_100)));//テクスチャ設定

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

	//割合に応じた色変化
	if (L_Raito > 0.75)
	{//水色
			//テクスチャ登録
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(LIFE_100)));//テクスチャ設定
	}

	else if (L_Raito <= 0.75 && L_Raito >= HALF)
	{//緑色
			//テクスチャ登録
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(LIFE_75)));//テクスチャ設定
	}

	else if (L_Raito <= HALF && L_Raito >= 0.25)
	{//黄色
			//テクスチャ登録
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(LIFE_50)));//テクスチャ設定
	}

	else if (L_Raito <= 0.25 && L_Raito > FLOAT_ZERO)
	{//赤色
			//テクスチャ登録
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(LIFE_25)));//テクスチャ設定
	}

	else if (L_Raito == FLOAT_ZERO)
	{
		CTexture* pTexture = CManager::GetInstance()->GetTexture();
		CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(DEATH)));//テクスチャ設定
	}

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