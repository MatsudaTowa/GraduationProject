//=============================================
//
//何Pかの表示UI[player_number_UI.cpp]
//Author Matsuda Towa
//
//=============================================
#include "player_number_UI.h"

const std::string My::CPlayerNumberUI::TEX_NAME[My::CPlayerNumberUI::NUM_PLAYER] =
{
	"data\\TEXTURE\\player_1P.png",
	"data\\TEXTURE\\player_2P.png",
	"data\\TEXTURE\\player_3P.png",
	"data\\TEXTURE\\player_4P.png"
};

//=============================================
// コンストラクタ
//=============================================
My::CPlayerNumberUI::CPlayerNumberUI(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerNumberUI::~CPlayerNumberUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CPlayerNumberUI::Init()
{
	SetSize({ POLY_SIZE,POLY_SIZE });
	SetColor(COLOR_WHITE);

	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CPlayerNumberUI::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CPlayerNumberUI::Update()
{
	CObject2D::Update();
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CPlayerNumberUI::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CPlayerNumberUI* My::CPlayerNumberUI::Create(D3DXVECTOR3 pos, int player_id)
{
	CPlayerNumberUI* pUI = new CPlayerNumberUI;
	if (pUI == nullptr) { return nullptr; }
	pUI->SetPos(pos);
	pUI->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(&CPlayerNumberUI::TEX_NAME[player_id])));
	pUI->Init();
	return pUI;
}
