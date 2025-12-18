//=============================================
//
// カードの情報テキスト「card_info_UI.h」
// Author松田永久
//
//=============================================
#include "card_info_UI.h"

namespace
{
	const D3DXVECTOR3 BASE_POS = { 210.0f,160.0f,0.0f };
	const D3DXVECTOR3 OFFSET_POS[My::CCardInfoUI::MAX] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	const D3DXVECTOR2 POLY_SIZE[My::CCardInfoUI::MAX] =
	{
		{200.0f,150.0f},
		{50.0f,50.0f},
		{50.0f,50.0f},
		{100.0f,50.0f},
		{100.0f,150.0f},
		{100.0f,120.0f},
	};
}

//=============================================
// コンストラクタ
//=============================================
My::CCardInfoUI::CCardInfoUI():m_pBG(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCardInfoUI::~CCardInfoUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCardInfoUI::Init()
{
	if (m_pBG == nullptr)
	{
		m_pBG = CCardInfoBG::Create(BASE_POS, POLY_SIZE[0]);
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCardInfoUI::Uninit()
{
	if (m_pBG != nullptr)
	{
		m_pBG->SetisDelete(true);
		m_pBG = nullptr;
	}

	delete this;
}

//=============================================
// 更新
//=============================================
void My::CCardInfoUI::Update()
{
}

//=============================================
// UI設定
//=============================================
void My::CCardInfoUI::SetCurrentUI(CCard* card)
{
}
