//===========================================================================================================================================================
// 
// card_info_cost.cpp [card_info_cost.cpp]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#include "card_info_cost.h"
#include <format>

namespace
{
	D3DXVECTOR3 OFFSET = { -170.0f,-120.0f,0.0f };
}
//===========================================================================================================================================================
// コンストラクタ
//===========================================================================================================================================================
My::CCardInfoCost::CCardInfoCost(int nPriority):CCardInfoUseFont(nPriority)
{
}

//===========================================================================================================================================================
// 初期化
//===========================================================================================================================================================
HRESULT My::CCardInfoCost::Init()
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
		CCard* card = GetCard();
		if (card == nullptr) { return S_OK; }
		std::wstring wtxt = std::format(L"{}", card->GetBaseStatus().nCost);
		D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換
		D3DXVECTOR2 size = GetSize(); //スクリーン座標に変換
		const wchar_t* txt = wtxt.c_str();
		offset_pos.x += OFFSET.x;
		offset_pos.y += OFFSET.y;
		pFontmanager->RegistAdjustFontSize(txt, offset_pos, { 40.0f,size.y }, 60.0f, 58.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos(OFFSET);
	}

	CCardInfoUseFont::Init();
	return S_OK;
}

//===========================================================================================================================================================
// 終了
//===========================================================================================================================================================
void My::CCardInfoCost::Uninit()
{
	CCardInfoUseFont::Uninit();
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::CCardInfoCost::Update()
{
	CCardInfoUseFont::Update();
}

void My::CCardInfoCost::SetUI()
{
	CFontManager* pFontmanager = GetFontManager(0);

	if (pFontmanager != nullptr)
	{	
		CCard* card = GetCard();
		if (card == nullptr) { return; }
		std::wstring wtxt = std::format(L"{}", card->GetBaseStatus().nCost);
		D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換
		D3DXVECTOR2 size = GetSize(); //スクリーン座標に変換
		const wchar_t* txt = wtxt.c_str();
		offset_pos.x += OFFSET.x;
		offset_pos.y += OFFSET.y;

		pFontmanager->SetTextAdjustFontSize(txt, offset_pos, { 40.0f,size.y }, 60.0f, 58.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos(OFFSET);
	}
}
