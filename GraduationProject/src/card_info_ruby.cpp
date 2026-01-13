#include "card_info_ruby.h"

namespace
{
	D3DXVECTOR3 OFFSET = { -145.0f,-140.0f,0.0f };
	float FONT_SIZE = 10.0f;
}
My::CCardInfoRuby::CCardInfoRuby(int nPriority):CCardInfoUseFont(nPriority)
{
}

HRESULT My::CCardInfoRuby::Init()
{
	CFontManager* pFontmanager = GetFontManager(0);
	if (pFontmanager == nullptr)
	{
		pFontmanager = new CFontManager(CFontManager::CENTER);
		pFontmanager->Init();
		PushFontManager(pFontmanager);
	}
	if (pFontmanager != nullptr)
	{
		D3DXVECTOR3 offset_pos = GetPos();
		D3DXVECTOR2 size = GetSize();
		CCard* card = GetCard();
		if (card == nullptr) { return S_OK; }
		std::string card_name = card->GetBaseStatus().ruby;
		std::wstring wtxt;
		int len = MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, nullptr, 0);
		wtxt.resize(len - 1);
		MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, &wtxt[0], len);
		D3DXVECTOR3 offset = { OFFSET.x ,OFFSET.y,OFFSET.z };
		offset_pos.x += offset.x;
		offset_pos.y += offset.y;

		pFontmanager->RegistAdjustFontSize(wtxt.c_str(), offset_pos, { 200.0,50.0f }, FONT_SIZE, 13.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos({ offset });
	}
	CCardInfoUseFont::Init();
	return S_OK;
}

void My::CCardInfoRuby::Uninit()
{
	CCardInfoUseFont::Uninit();
}

void My::CCardInfoRuby::Update()
{
	CCardInfoUseFont::Update();
}

void My::CCardInfoRuby::SetUI()
{
	CFontManager* pFontmanager = GetFontManager(0);

	if (pFontmanager != nullptr)
	{
		D3DXVECTOR3 offset_pos = GetPos();
		D3DXVECTOR2 size = GetSize();
		CCard* card = GetCard();
		if (card == nullptr) { return; }
		std::string card_name = card->GetBaseStatus().ruby;
		std::wstring wtxt;
		int len = MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, nullptr, 0);
		wtxt.resize(len - 1);
		MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, &wtxt[0], len);
		D3DXVECTOR3 offset = { OFFSET.x ,OFFSET.y,OFFSET.z };
		offset_pos.x += offset.x;
		offset_pos.y += offset.y;

		pFontmanager->SetTextAdjustFontSize(wtxt.c_str(), offset_pos, { 200.0,50.0f }, FONT_SIZE, 13.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos({ offset });
	}
}
