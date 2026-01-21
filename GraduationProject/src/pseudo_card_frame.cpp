//===========================================================================================================================================================
// 
// pseund_card_frame.cppのヘッダー [pseund_card_frame.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "pseudo_card_frame.h"

My::CPsendCardFrame::CPsendCardFrame(int nPri):CCardFrame(nPri)
{
}

My::CPsendCardFrame::~CPsendCardFrame()
{
}

HRESULT My::CPsendCardFrame::Init()
{
	// テクスチャ設定
	CTexture* pTex = GET_TEXTURE;

	CCardFrame::FRAMETYPE type = GetType();

	if (type == CCardFrame::FRAMETYPE_ILLUST)
	{
		std::string tex = "data\\TEXTURE\\cardframe\\illust.png";
		BindTexture(pTex->GetAddress(pTex->Regist(tex)));
	}
	else
	{
		BindTexture(pTex->GetAddress(pTex->Regist(m_FrameInfo[GetType()].pass)));
	}

	{
		SetColor({0.6f,0.6f,0.6f,1.0f});
	}


	// サイズ設定
	SetSize({ CARD_WIDTH,CARD_HEIGHT });

	// カラー設定

	//頂点設定
	SetVtx();
	return S_OK;
}

void My::CPsendCardFrame::Uninit()
{
	CCardFrame::Uninit();
}

void My::CPsendCardFrame::Update()
{
	//頂点設定
	SetVtx();
}

void My::CPsendCardFrame::Draw()
{
	CCardFrame::Draw();
}

My::CPsendCardFrame* My::CPsendCardFrame::Create(FRAMETYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPsendCardFrame* pObj = new CPsendCardFrame;
	if (pObj == nullptr) { return nullptr; }
	pObj->SetType(type);
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->Init();
	return pObj;
}
