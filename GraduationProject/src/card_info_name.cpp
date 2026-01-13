//===========================================================================================================================================================
// 
// card_info_name.cpp [card_info_name.cpp]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#include "card_info_name.h"

namespace
{
	D3DXVECTOR3 OFFSET = { -130.0f,-110.0f,0.0f };
	float FONT_SIZE = 23.5f;
}

//===========================================================================================================================================================
//コンストラクタ
//===========================================================================================================================================================
My::CCardInfoName::CCardInfoName(int nPriority):CCardInfoUseFont(nPriority)
{
}

//===========================================================================================================================================================
// 初期化
//===========================================================================================================================================================
HRESULT My::CCardInfoName::Init()
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
		std::string card_name = card->GetBaseStatus().name;
		std::wstring wtxt;
		int len = MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, nullptr, 0);
		wtxt.resize(len - 1);
		MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, &wtxt[0], len);
		D3DXVECTOR3 offset = { OFFSET.x ,OFFSET.y,OFFSET.z };
		offset_pos.x += offset.x;
		offset_pos.y += offset.y;

		pFontmanager->RegistAdjustFontSize(wtxt.c_str(), offset_pos, { 200.0,50.0f }, FONT_SIZE, 12.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos({ offset });
	}
	CCardInfoUseFont::Init();
	return S_OK;
}

//===========================================================================================================================================================
// 終了
//===========================================================================================================================================================
void My::CCardInfoName::Uninit()
{
	CCardInfoUseFont::Uninit();
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::CCardInfoName::Update()
{

	CCardInfoUseFont::Update();
}

void My::CCardInfoName::SetUI()
{
	CFontManager* pFontmanager = GetFontManager(0);

	if (pFontmanager != nullptr)
	{
		D3DXVECTOR3 offset_pos = GetPos();
		D3DXVECTOR2 size = GetSize();
		CCard* card = GetCard();
		if (card == nullptr) { return; }
		std::string card_name = card->GetBaseStatus().name;
		std::wstring wtxt;
		int len = MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, nullptr, 0);
		wtxt.resize(len - 1);
		MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, &wtxt[0], len);
		D3DXVECTOR3 offset = { OFFSET.x ,OFFSET.y,OFFSET.z };
		offset_pos.x += offset.x;
		offset_pos.y += offset.y;

		pFontmanager->SetTextAdjustFontSize(wtxt.c_str(), offset_pos, {200.0,50.0f }, FONT_SIZE, 35.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos({ offset });
	}
}
