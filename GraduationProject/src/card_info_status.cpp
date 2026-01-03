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
	D3DXCOLOR ICON_COL[My::CCardTypeIcon::TYPE_MAX]
	{
		{},
		COLOR_RED,
		COLOR_BLUE,
		COLOR_PURPLE,
		COLOR_ORANGE,
	};
}
My::CCardInfoStatus::CCardInfoStatus(int nPriority):CCardInfoUseFont(nPriority)
{
	m_IconVector.clear();
}

HRESULT My::CCardInfoStatus::Init()
{
	CCardInfoUseFont::Init();

	return S_OK;
}

void My::CCardInfoStatus::Uninit()
{
	for (auto& itr : m_IconVector)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_IconVector.clear();
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
	for (auto& icon : m_IconVector)
	{
		if (icon == nullptr) { continue; }
		icon->SetisDraw(GetisDraw());
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

		SetIcon(statusText, CCardTypeIcon::TYPE_ATTACK);
	}
	else if (typeid(*card) == typeid(CCardDeffence))
	{
		CCardDeffence* deffence_card = dynamic_cast<CCardDeffence*>(card);
		statusText.push_back(std::format(L"{}", deffence_card->GetDefenceValue()));
		SetIcon(statusText, CCardTypeIcon::TYPE_DEFFENCE);

		if (deffence_card->GetCounterValue() > 0)
		{
			statusText.push_back(std::format(L"{}", deffence_card->GetCounterValue()));
			SetIcon(statusText, CCardTypeIcon::TYPE_COUNTER);

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
			if (m_IconVector[i] != nullptr)
			{
				m_IconVector[i]->SetisDraw(false);
				//表示するものをリセット
				m_IconVector[i]->SetColor(COLOR_NONE);
			}
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

		if (m_IconVector[i] != nullptr)
		{
			m_IconVector[i]->SetPos(pos);
		}

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

void My::CCardInfoStatus::SetIcon(std::vector<std::wstring>& statusText, int icon_type)
{
	if (m_IconVector.size() <= statusText.size() - 1)
	{
		CCardTypeIcon* icon = CCardTypeIcon::Create(VEC3_RESET_ZERO, icon_type);
		icon->SetSize({ 30.0f,30.0f });
		icon->SetColor(ICON_COL[icon_type]);
		m_IconVector.push_back(icon);
	}
	else if (m_IconVector[statusText.size() - 1] != nullptr)
	{
		m_IconVector[statusText.size() - 1]->SetTypeTexture(icon_type);
		m_IconVector[statusText.size() - 1]->SetColor(ICON_COL[icon_type]);

	}
}
