//=============================================
//
//デッキUI処理「deck_num_UI.cpp」
// Author松田永久
//
//=============================================
#include "deck_num_UI.h"

My::CDeckNumUI::CDeckNumUI():CZoneNumUI(),
m_pBG(nullptr)
{
}

My::CDeckNumUI::~CDeckNumUI()
{
}

HRESULT My::CDeckNumUI::Init()
{
	CZoneNumUI::Init();
	if (m_pBG == nullptr)
	{
		m_pBG = CDeckNumUI_BG::Create(GetNumPos());
	}
	return S_OK;
}

void My::CDeckNumUI::Uninit()
{
	if (m_pBG != nullptr)
	{
		m_pBG->Uninit();
		m_pBG = nullptr;
	}
	CZoneNumUI::Uninit();
}
