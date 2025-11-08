#include "card_assist.h"

My::CCardAssist::CCardAssist(int nPriority):CCard(nPriority),
m_AssistType(),
m_BuffType(),
m_DebuffType(),
m_HealType()
{
}

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

//===========================================================================================================
// カード情報読み込み
//===========================================================================================================
void My::CCardAssist::LoadCardData()
{
	//TODO:各ステータス読み込みしてから親クラスの読み込み
	CCard::LoadCardData();
}
