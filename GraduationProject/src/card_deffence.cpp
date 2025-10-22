#include "card_deffence.h"

My::CCardDeffence::~CCardDeffence()
{
}

HRESULT My::CCardDeffence::Init()
{
	CCard::Init();
	SetCardType(CCard::CARDTYPE_::TYPE_DEFFENCE);
	return S_OK;
}

void My::CCardDeffence::Uninit()
{
	CCard::Uninit();
}

void My::CCardDeffence::Update()
{
	CCard::Update();
}

void My::CCardDeffence::Draw()
{
	CCard::Draw();
}