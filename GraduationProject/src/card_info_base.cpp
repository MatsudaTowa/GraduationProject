#include "card_info_base.h"
#include "card_info_BG.h"
#include "card_info_cost.h"
#include "card_info_name.h"

My::CCardInfoBase::CCardInfoBase(int nPriority):CObject2D(nPriority)
{
}

My::CCardInfoBase::~CCardInfoBase()
{
}

HRESULT My::CCardInfoBase::Init()
{
	SetVtx();
	return S_OK;
}

void My::CCardInfoBase::Uninit()
{
	CObject2D::Uninit();
}

void My::CCardInfoBase::Update()
{
	SetVtx();
}

void My::CCardInfoBase::Draw()
{
	CObject2D::Draw();
}

My::CCardInfoBase* My::CCardInfoBase::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, UI_TYPE type)
{
	CCardInfoBase* pInfo = nullptr;
	switch (type)
	{
	case My::CCardInfoBase::BG:
		pInfo = new CCardInfoBG;
		break;
	case My::CCardInfoBase::COST:
		pInfo = new CCardInfoCost;
		break;
	case My::CCardInfoBase::STATES:
		return pInfo;
		break;
	//case My::CCardInfoBase::NAME:
	//	pInfo = new CCardInfoName;
	//	break;
	case My::CCardInfoBase::ILLUST:
		return pInfo;
		break;
	case My::CCardInfoBase::TEXT:
		return pInfo;
		break;
	default:
		return pInfo;
		break;
	}
	pInfo->SetPos(pos);
	pInfo->SetSize(size);
	pInfo->Init();
	return pInfo;
}
