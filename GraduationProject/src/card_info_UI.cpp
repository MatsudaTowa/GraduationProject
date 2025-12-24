//=============================================
//
// カードの情報テキスト「card_info_UI.h」
// Author松田永久
//
//=============================================
#include "card_info_UI.h"
#include "card_info_base.h"
namespace
{
	const D3DXVECTOR3 BASE_POS = { 210.0f,160.0f,0.0f };
	const D3DXVECTOR3 OFFSET_POS[My::CCardInfoBase::MAX] =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	const D3DXVECTOR2 POLY_SIZE[My::CCardInfoBase::MAX] =
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
My::CCardInfoUI::CCardInfoUI():m_pCardInfo()
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
	for (int i = 0; i < CCardInfoBase::MAX - 1; ++i)
	{
		if (m_pCardInfo[i] == nullptr)
		{
			m_pCardInfo[i] = CCardInfoBase::Create(BASE_POS, POLY_SIZE[i], (CCardInfoBase::UI_TYPE)i);
		}
	}

	SetisDraw(false);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCardInfoUI::Uninit()
{
	for (int i = 0; i < CCardInfoBase::MAX - 1; ++i)
	{
		if (m_pCardInfo[i] == nullptr) { continue; }
		
		m_pCardInfo[i]->Uninit();
		m_pCardInfo[i] = nullptr;
		
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
	for (int i = 0; i < CCardInfoBase::MAX - 1; ++i)
	{
		if (m_pCardInfo[i] == nullptr) { continue; }

		if (m_pCardInfo[i]->GetCard() == card) { continue; }

		m_pCardInfo[i]->SetCard(card);
		m_pCardInfo[i]->SetUI();
	}
}
