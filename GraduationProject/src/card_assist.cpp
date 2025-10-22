#include "card_assist.h"

My::CCardAssist::~CCardAssist()
{
}

HRESULT My::CCardAssist::Init()
{
	CCard::Init();
	SetCardType(CCard::CARDTYPE_::TYPE_ASSIST);
	return S_OK;
}

void My::CCardAssist::Uninit()
{
	CCard::Uninit();
}

void My::CCardAssist::Update()
{
	CCard::Update();
}

void My::CCardAssist::Draw()
{
	CCard::Draw();
}
