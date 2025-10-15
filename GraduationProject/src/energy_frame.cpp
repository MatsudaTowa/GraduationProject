//==================================================================================================================
//
// エナジーUI枠 「energy_frame.cpp」
// Author : 笹生一颯
//
//==================================================================================================================
#include "energy_frame.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\UI\\Energy.png";
}

//=================================================================
// コンストラクタ
//=================================================================
My::CEnergy_frame::CEnergy_frame(int nPriority) : CObject2D(nPriority)
{
}

//=================================================================
// デストラクタ
//=================================================================
My::CEnergy_frame::~CEnergy_frame()
{

}

//=================================================================
// 初期化
//=================================================================
HRESULT My::CEnergy_frame::Init()
{

	CObject2D::Init();

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	//テクスチャ登録
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));//テクスチャ設定

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();


	return S_OK;
}

//=================================================================
// 終了
//=================================================================
void My::CEnergy_frame::Uninit()
{
	CObject2D::Uninit();
}

//=================================================================
// 更新
//=================================================================
void My::CEnergy_frame::Update()
{
	CObject2D::Update();
}

//=================================================================
// 描画
//=================================================================
void My::CEnergy_frame::Draw()
{
	CObject2D::Draw();
}
//=================================================================
// 生成
//=================================================================
My::CEnergy_frame* My::CEnergy_frame::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CEnergy_frame* pEnergyFrame = new CEnergy_frame(11);
	pEnergyFrame->SetPos(pos);
	pEnergyFrame->SetSize(size);
	pEnergyFrame->SetType(CObject::OBJECT_TYPE::OBJECT_TYPE_PLAYER);
	pEnergyFrame->Init();
	return pEnergyFrame;
}