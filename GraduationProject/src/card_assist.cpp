#include "card_assist.h"

My::CCardAssist::CCardAssist(int nPriority):CCard(nPriority),
m_AssistType()
{
}

My::CCardAssist::~CCardAssist()
{
}

HRESULT My::CCardAssist::Init()
{
	SetCardType(CCard::CARDTYPE_::TYPE_ASSIST);
	CCard::Init();
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

//===========================================================================================================
//固有情報の読み込み
//===========================================================================================================
void My::CCardAssist::LoadUniqueInfo(CCard_Client::Param param)
{
	//攻撃ステータスの読み込み
	m_AssistType = (AssistType)param.Attacktype;	//攻撃の種類
}
