//===========================================================================================================================================================
// 
// 手札の処理 [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card.h"
#include "game.h"

My::CHand::CHand() :
	m_SelectNum(0),
	m_TotalNum(0),
	m_IsPassStart(false)
{
	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}
}

My::CHand::~CHand()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
void My::CHand::Init()
{
	m_SelectNum = 0;
	m_TotalNum = 0;
	m_IsPassStart = false;

	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}
	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(CGame::GAME_CAMERA);
	m_CenterPos = { pCamera->GetPosV().x, pCamera->GetPosV().y - 100.0f, pCamera->GetPosV().z + 30.0f };
}

//===========================================================================================================
// 開始時処理
//===========================================================================================================
void My::CHand::Start()
{
	// 一度通ったら通らない
	if (m_IsPassStart)
		return;

	HandDraw(START_HANDS);
	m_IsPassStart = true;
}

//===========================================================================================================
// 選択
//===========================================================================================================
void My::CHand::Select()
{
}

//===========================================================================================================
// 手札を引く
//===========================================================================================================
void My::CHand::HandDraw(int drawnum)
{
	/*
	* @brief 手札が持てるカードの枚数を超えたらドローをしない
	* TODO : ここの処理は超えたときにMAXの枚数にするか、引けないようにするか検討
	*/
	if (m_TotalNum >= MAX_HANDSCARD)
		return;	

	// TODO : 今後の実装山札を引く、山札の総数を減らす
	
	for (int i = 0; i < drawnum; i++)
	{
		m_pCard[m_TotalNum] = CCard::Create();
		SetHandCardPos();
		m_TotalNum++;	// 手札の総数を増やす
	}
}

My::CHand* My::CHand::Create()
{
	CHand* pHand = new CHand();

	pHand->Init();

	return pHand;
}

void My::CHand::SetHandCardPos()
{
	float posInterbal = 20.0f;


	D3DXVECTOR3 firstpos;	// 一番左側の位置(手札の最初の位置)
	float xpos;
	xpos = m_CenterPos.x - (m_TotalNum* posInterbal);
	int a = 0;

	if (m_TotalNum == 0)
	{
		a = 0;
	}
	else
	{
		a = 1;
	}

	for (int i = 0; i <= m_TotalNum; i++)
	{
		firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };

		if (i != 0)
		{
			m_pCard[i]->SetPos({ m_pCard[i - 1]->GetPos().x+posInterbal, m_pCard[i - 1]->GetPos().y, m_pCard[i - 1]->GetPos().z });
		}
		else
		{
			m_pCard[0]->SetPos({ firstpos.x + (a * (posInterbal * -0.5f)), firstpos.y, firstpos.z });
		}
		
		xpos += posInterbal;
	}
}
