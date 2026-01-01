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
	D3DXVECTOR3 OFFSET = { -180.0f,-60.0f,0.0f };
}
My::CCardInfoStatus::CCardInfoStatus(int nPriority):CCardInfoUseFont(nPriority)
{
}

HRESULT My::CCardInfoStatus::Init()
{
	CCardInfoUseFont::Init();

	return S_OK;
}

void My::CCardInfoStatus::Uninit()
{
	CCardInfoUseFont::Uninit();
}

void My::CCardInfoStatus::Update()
{

	for (int i = 0; i < GetFontManagerSize(); ++i)
	{
		CFontManager* fm = GetFontManager(i);
		for (auto& text : fm->GetList())
		{
			if (text == nullptr) { continue; }
			text->SetisDraw(GetisDraw());
		}
	}
}

void My::CCardInfoStatus::SetUI()
{
	CCard* card = GetCard();
	if (card == nullptr) { return; }

	std::vector<std::wstring> statusText;

	if (typeid(*card) == typeid(CCardAttack))
	{
		CCardAttack* attack_card = dynamic_cast<CCardAttack*>(card);
		statusText.push_back(std::format(L"{}", attack_card->GetAttackValue()));
	}
	else if (typeid(*card) == typeid(CCardDeffence))
	{
		CCardDeffence* deffence_card = dynamic_cast<CCardDeffence*>(card);
		statusText.push_back(std::format(L"{}", deffence_card->GetDefenceValue()));
		if (deffence_card->GetCounterValue() > 0)
		{
			statusText.push_back(std::format(L"{}", deffence_card->GetCounterValue()));
		}
	}

	EnsureFontManagerCount(statusText.size(), CFontManager::CENTER);

	for (int i = 0; i < GetFontManagerSize(); ++i)
	{
		CFontManager* fm = GetFontManager(i);

		//文字列を一度リセット
		fm->Release();
		if (i >= statusText.size())
		{
			// 使わない → 描画OFF
			for (auto& text : fm->GetList())
			{
				if (text) 
				{ 
					text->SetisDraw(false); 
				}
			}
			continue;
		}

		// 使う分だけ更新
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR2 size = GetSize();

		pos.x += OFFSET.x + (60.0f * i);
		pos.y += OFFSET.y;

		fm->RegistAdjustFontSize(
			statusText[i].c_str(),
			pos,
			{ 40.0f, size.y },
			40.0f,
			45.0f,
			0, 5,
			COLOR_WHITE,
			false
		);
	}
}
