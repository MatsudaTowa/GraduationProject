//===========================================================================================================================================================
// 
// 手札の処理 [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card.h"
#include "card_state.h"
#include "game.h"

My::CHand::CHand() :
	m_SelectNum(-1),
	m_TotalNum(0),
	m_IsPassStart(false),
	m_FrontSelectNum(-1)
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
	// メンバ変数初期化
	m_SelectNum = -1;
	m_TotalNum = 0;
	m_IsPassStart = false;

	for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}

	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
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

	// スタート時のカードの枚数分ドローする
	HandDraw(START_HANDS);

	// スタートを通ったら、この試合ではスタートを通らない
	m_IsPassStart = true;
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CHand::Update()
{
	// キーボード取得
	CInputKeyboard* pkeyboad = CManager::GetInstance()->GetKeyboard();

	// 手札ドロー
	if (pkeyboad->GetTrigger(DIK_SPACE))
	{
		HandDraw(1);
	}

	// 手札選択
	Select();

	if (pkeyboad->GetTrigger(DIK_J))
	{
		if (m_SelectNum >= 0)
			m_pCard[m_SelectNum]->ChangeState(CCardState::CARD_STATE::CARD_CAST);
	}
}

//===========================================================================================================
// 選択
//===========================================================================================================
void My::CHand::Select()
{
	// キーボード取得
	// TODO : マウスDE操作
	CInputKeyboard* pKeyboad = CManager::GetInstance()->GetKeyboard();

	// 選択番号が変わっているか否か
	bool IsChangeSelect = false;

	// キーボードで選択
	if (pKeyboad->GetTrigger(DIK_RIGHTARROW))
	{// 右選択
		IsChangeSelect = true;
		m_SelectNum++;
		if (m_SelectNum >= m_TotalNum)
		{
			m_SelectNum = 0;
		}
	}
	if (pKeyboad->GetTrigger(DIK_LEFTARROW))
	{// 左選択
		IsChangeSelect = true;
		m_SelectNum--;
		if (m_SelectNum < 0)
		{
			m_SelectNum = m_TotalNum - 1;
		}
	}

	// 番号が変わっていない場合チェンジステートをしない
	if (!IsChangeSelect)
		return;

	SelectStateSet();
}

//===========================================================================================================
// 手札のカードの状態変更
//===========================================================================================================
void My::CHand::SelectStateSet()
{
	// すべてのカードを選ばれていない状態にする
	for (int i = 0; i < m_TotalNum; i++)
	{// 今持っている枚数分
		if (m_pCard[i] != nullptr)
		{
			m_pCard[i]->ChangeState(CCardState::CARD_STATE::CARD_NEUTRAL);
		}
	}

	if (m_SelectNum <= -1)
		return;

	// 選択中のカードのステートを変える
	if (m_pCard[m_SelectNum] != nullptr)
	{
		m_pCard[m_SelectNum]->ChangeState(CCardState::CARD_STATE::CARD_PICKUP);
	}
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

//===========================================================================================================
// 生成
//===========================================================================================================
My::CHand* My::CHand::Create()
{
	CHand* pHand = new CHand();

	pHand->Init();

	return pHand;
}

//===========================================================================================================
// 手札のカードの座標設定
//===========================================================================================================
void My::CHand::SetHandCardPos()
{
	D3DXVECTOR3 firstpos;	// 一番左側の位置(手札の最初の位置)
	float posInterbal = 20.0f - (20 * m_TotalNum * 0.07f);	// 手札に表示されている時のカードの間隔
	float xpos;	// 一枚目のカードのx座標

	// x座標の設定 = センター - (現在の合計枚数 * カードの間隔の半分)
	xpos = m_CenterPos.x - (m_TotalNum* posInterbal*0.5f);

	for (int i = 0; i <= m_TotalNum; i++)
	{
		// カードの座標の設定
		if (i != 0)
		{// 一枚目以外は前の手札の位置を参照して "Interbal" 分横にずらす
			m_pCard[i]->SetPos({ m_pCard[i - 1]->GetPos().x+posInterbal, m_pCard[i - 1]->GetPos().y, m_pCard[i - 1]->GetPos().z });
		}
		else
		{
			// 一枚目のカードの座標(基準となる)
			firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };
			m_pCard[0]->SetPos(firstpos);
		}
		
		// 間隔を開ける
		xpos += posInterbal*0.5f;
	}
	
}
