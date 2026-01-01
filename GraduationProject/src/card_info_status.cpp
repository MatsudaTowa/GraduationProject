//===========================================================================================================================================================
// 
// card_info_status.cpp [card_info_status.cpp]
// Author : マツダトワ
// 
//===========================================================================================================================================================
#include "card_info_status.h"
#include <format>
#include "card_attack.h"
#include "card_assist.h"
#include "card_deffence.h"
namespace
{
	D3DXVECTOR3 OFFSET = { -170.0f,-60.0f,0.0f };
}
My::CCardInfoStatus::CCardInfoStatus(int nPriority):CCardInfoUseFont(nPriority)
{
}

HRESULT My::CCardInfoStatus::Init()
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
		std::wstring wtxt;

		if (card == nullptr) { return S_OK; }
		if (typeid(*card) == typeid(CCardAttack))
		{
			CCardAttack* attack_card = dynamic_cast<CCardAttack*>(card);
			wtxt = std::format(L"{}", attack_card->GetAttackValue());
		}
		else if (typeid(*card) == typeid(CCardDeffence))
		{
			CCardDeffence* deffence_card = dynamic_cast<CCardDeffence*>(card);
			wtxt = std::format(L"{}", deffence_card->GetDefenceValue());
		}
		else if (typeid(*card) == typeid(CCardDeffence))
		{
			return S_OK;
		}
		D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換
		D3DXVECTOR2 size = GetSize(); //スクリーン座標に変換
		const wchar_t* txt = wtxt.c_str();
		offset_pos.x += OFFSET.x;
		offset_pos.y += OFFSET.y;
		pFontmanager->RegistAdjustFontSize(txt, offset_pos, { 40.0f,size.y }, 40.0f, 45.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos(OFFSET);
	}

	CCardInfoUseFont::Init();

	return S_OK;
}

void My::CCardInfoStatus::Uninit()
{
	CCardInfoUseFont::Uninit();
}

void My::CCardInfoStatus::Update()
{
	CCardInfoUseFont::Update();
}

void My::CCardInfoStatus::SetUI()
{
	CFontManager* pFontmanager = GetFontManager(0);

	if (pFontmanager != nullptr)
	{
		CCard* card = GetCard();
		std::wstring wtxt;

		if (card == nullptr) { return; }
		if (typeid(*card) == typeid(CCardAttack))
		{
			CCardAttack* attack_card = dynamic_cast<CCardAttack*>(card);
			wtxt = std::format(L"{}", attack_card->GetAttackValue());
		}
		else if (typeid(*card) == typeid(CCardDeffence))
		{
			CCardDeffence* deffence_card = dynamic_cast<CCardDeffence*>(card);
			wtxt = std::format(L"{}", deffence_card->GetDefenceValue());
		}
		else if (typeid(*card) == typeid(CCardDeffence))
		{
			return;
		}
		D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換
		D3DXVECTOR2 size = GetSize(); //スクリーン座標に変換
		const wchar_t* txt = wtxt.c_str();
		offset_pos.x += OFFSET.x;
		offset_pos.y += OFFSET.y;

		pFontmanager->SetTextAdjustFontSize(txt, offset_pos, { 40.0f,size.y }, 40.0f, 48.0f, 0, 5, COLOR_WHITE, false);
		SetOffSetPos(OFFSET);
	}
}
