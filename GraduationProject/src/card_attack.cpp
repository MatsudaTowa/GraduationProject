#include "card_attack.h"

My::CCardAttack::~CCardAttack()
{
}

HRESULT My::CCardAttack::Init()
{
	CCard::Init();
	SetCardType(CCard::CARDTYPE_::TYPE_ATTACK);
	return S_OK;
}

void My::CCardAttack::Uninit()
{
	CCard::Uninit();
}

void My::CCardAttack::Update()
{
	CCard::Update();
}

void My::CCardAttack::Draw()
{
	CCard::Draw();
}
