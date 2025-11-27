//=============================================
//
//カードのタイプアイコン[card_type_icon]
//Author Matsuda Towa
//
//=============================================
#include "card_type_icon.h"

namespace
{
	const float ICON_SIZE = 12.0f;

	const std::string TEX_NAME[My::CCard::CARDTYPE_::TYPE_MAX]
	{
		"",
		"data\\TEXTURE\\icon\\attack.png",
		"data\\TEXTURE\\icon\\shield.png",
		"data\\TEXTURE\\icon\\assist.png",
	};
}

//=============================================
// コンストラクタ
//=============================================
My::CCardTypeIcon::CCardTypeIcon(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCardTypeIcon::~CCardTypeIcon()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCardTypeIcon::Init()
{
	SetColor(COLOR_WHITE);
	SetSize({ ICON_SIZE,ICON_SIZE });

	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCardTypeIcon::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCardTypeIcon::Update()
{
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CCardTypeIcon::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CCardTypeIcon* My::CCardTypeIcon::Create(D3DXVECTOR3 pos, CCard::CARDTYPE_ type)
{
	CCardTypeIcon* pIcon = new CCardTypeIcon;
	if (pIcon == nullptr) { return nullptr; }

	pIcon->SetPos(pos);

	//テクスチャ登録
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	pIcon->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME[type])));//テクスチャ設定

	pIcon->Init();

	return pIcon;
}
