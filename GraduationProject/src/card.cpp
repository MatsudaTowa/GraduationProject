//===========================================================================================================================================================
// 
// ƒJ[ƒh‚Ìˆ— [card.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "card.h"

My::CCard::CCard(int nPriority):CObjectX(nPriority)
{
}

My::CCard::~CCard()
{
}

HRESULT My::CCard::Init()
{
	CModel*pModel= CManager::GetInstance()->GetModel();
	int Idx = 0;
	Idx = pModel->Regist("data\\model\\card_traial_000.x");
	CModel::MODEL_INFO mi = pModel->GetModelInfo(Idx);
	BindXFile(mi.pBuffMat, mi.dwNumMat, mi.pMesh);

	float size = 1.0f;

	SetScale({ size,size,size });

	//SetColor(COLOR_RED);
	return E_NOTIMPL;
}

void My::CCard::Draw()
{
	CObjectX::Draw();
}

My::CCard* My::CCard::Create()
{
	CCard* pCard = new CCard(5);

	CCamera*pcamera= CManager::GetInstance()->GetCamera(2);

	pCard->SetPos(pcamera->GetPosV());
	pCard->Init();

	return pCard;
}


