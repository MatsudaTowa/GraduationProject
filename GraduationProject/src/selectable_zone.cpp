#include "selectable_zone.h"

My::CSelectableZone::CSelectableZone() :m_pRange(nullptr)
{
}

My::CSelectableZone::~CSelectableZone()
{
	if (m_pRange != nullptr)
	{
		m_pRange->SetisDelete(true);
		m_pRange = nullptr;
	}
}

void My::CSelectableZone::CreateRange(D3DXVECTOR3 pos, D3DXVECTOR2 size, D3DXCOLOR col, bool isDraw)
{
	if (m_pRange == nullptr)
	{
		m_pRange = CSelectionRange::Create(pos, size);
		m_pRange->SetColor(col);
		m_pRange->SetisDraw(isDraw);
	}
}

void My::CSelectableZone::Update()
{
}
