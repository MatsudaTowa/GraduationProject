//===========================================================================================================================================================
// 
// 手札の処理 [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card.h"
#include "card_state.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_assist.h"
#include "game.h"
#include "active_manager.h"
#include "active_scene_state.h"

My::CHand::CHand() :
	m_SelectNum(-1),
	m_TotalNum(0),
	m_IsPassStart(false),
	m_IsPickUp(false),
	m_HandState(NEUTRAL),
	m_pHandCard(),
	m_pStayCard(),
	m_pTriggerCard()
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
	m_CenterPos = { pCamera->GetPosV().x, pCamera->GetPosV().y - 150.0f, pCamera->GetPosV().z + 23.0f };
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

	// カードキャスト
	Cast();

	// カード除去
	DeleteCard();
}

//===========================================================================================================
// 選択
//===========================================================================================================
void My::CHand::Select()
{
	// キャスト状態だったらセレクトさせない
	if (m_HandState == CAST)
		return;

	// 何も選択されていない場合
	if (!m_IsPickUp)
	{
		for (int i = 0; i < m_TotalNum; i++)
		{// すべてのカードを判定

			if (!m_pCard[i])
				continue;

			// マウスでカード選択
			m_IsPickUp = m_pCard[i]->CardSelectToMouse();

			if (m_IsPickUp)
			{// どれかのカードが選択されたら
				m_SelectNum = i;	// 今の配列番号を一時格納しておく
				//CGameManager::GetInstance()->ChangeState(new CCardCast);
				break;
			}
		}
	}
	else
	{
		if (!m_pCard[m_SelectNum])
			return;

		// 選択番号のカードが非選択状態になったら
		if (m_pCard[m_SelectNum]->GetStateNum() == CCardState::CARD_NEUTRAL)
		{
			//CGameManager::GetInstance()->ChangeState(new CDuel);
			m_IsPickUp = false;	// 選択されていない状態にする
		}
		else
		{
			// 選択番号のカードのみ判定する
			m_IsPickUp = m_pCard[m_SelectNum]->CardSelectToMouse();
		}
	}
}

//===========================================================================================================
// カードのキャスト
//===========================================================================================================
void My::CHand::Cast()
{
	// 選択カードがnullptr か セレクトナンバーが 0 未満だったら
	if (!m_pCard[m_SelectNum] || m_SelectNum < 0)
		return;

	// キャスト状態かどうか判断
	bool IsCast = m_pCard[m_SelectNum]->CardCastToMouse();

	// キャスト状態によって手札のタイプを変える
	if (IsCast)
		m_HandState = CAST;
	else
		m_HandState = NEUTRAL;
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

	if (!m_pCard[m_SelectNum]||m_SelectNum <= -1)
		return;

	// 選択中のカードのステートを変える
	if (m_pCard[m_SelectNum] != nullptr)
	{
		m_pCard[m_SelectNum]->ChangeState(CCardState::CARD_STATE::CARD_PICKUP);
	}
}

//===========================================================================================================
// カード消去
//===========================================================================================================
void My::CHand::DeleteCard()
{
	// カード番号(-1 で初期化)
	int num = -1;

	// トリガーされたカードを調べる
	for (int i = 0; i < m_TotalNum; i++)
	{
		if (m_pCard[i]->GetStateNum() == CCardState::CARD_STATE::CARD_TRIGGER)
		{
			num = i;
		}
	}

	// -1以下だったら通さない
	if (num < 0)
		return;

	// トリガーカード除去
	m_pCard[num]->Uninit();

	// 次の番号のカードの情報を前に移していく
	for (int ii = num; ii < m_TotalNum; ii++)
	{
		m_pCard[ii] = m_pCard[ii + 1];
	}

	// 最大の配列カードに変なゴミが残らないようにヌルにする
	m_pCard[m_TotalNum-1] = nullptr;

	// 手札総数を減らす
	m_TotalNum--;

	// ステイ中のカードがあるかどうか
	for (int i = 0; i < m_TotalNum; i++)
	{
		if (m_pCard[i]->GetStateNum() == CCardState::CARD_STATE::CARD_STAY)
		{// あったら手札の整理をしない
			return;
		}
	}

	// 手札の位置をセットする
	SetHandCardPos();
	
}

//===========================================================================================================
// カード設定
//===========================================================================================================
void My::CHand::SetCard(CCard::CARDTYPE_ type)
{
	switch (type)
	{
	case CCard::CARDTYPE_::TYPE_ATTACK:
		m_pCard[m_TotalNum] = CCardAttack::Create(type);
		break;

	case CCard::CARDTYPE_::TYPE_DEFFENCE:
		m_pCard[m_TotalNum] = CCardDeffence::Create(type);
		break;

	case CCard::CARDTYPE_::TYPE_ASSIST:
		m_pCard[m_TotalNum] = CCardAssist::Create(type);
		break;
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
		// TODO : 一旦ここでランダムに抽選する
		int rundom;
		rundom = Rundom(CCard::CARDTYPE_::TYPE_ATTACK, CCard::CARDTYPE_::TYPE_MAX);

		SetCard((CCard::CARDTYPE_)rundom);
		m_TotalNum++;	// 手札の総数を増やす

		// 手札の位置整理
		SetHandCardPos();
	}
}

//===========================================================================================================
// 生成
//===========================================================================================================
My::CHand* My::CHand::Create()
{
	CHand* pHand = new CHand();
	//CGameManager::GetInstance()->SetHand(pHand);

	pHand->Init();

	return pHand;
}

//===========================================================================================================
// 手札のカードの座標設定
//===========================================================================================================
void My::CHand::SetHandCardPos()
{
	D3DXVECTOR3 firstpos;	// 一番左側の位置(手札の最初の位置)
	float posInterbal = 25.0f - (20 * m_TotalNum * 0.07f);	// 手札に表示されている時のカードの間隔
	float xpos;	// 一枚目のカードのx座標

	// x座標の設定 = センター - (現在の合計枚数 * カードの間隔の半分)
	xpos = (m_CenterPos.x) - ((m_TotalNum-1)* posInterbal*0.5f);

	for (int i = 0; i < m_TotalNum; i++)
	{
		// カードの座標の設定
		if (i != 0)
		{
			// 一枚目以外は前の手札の位置を参照して "Interbal" 分横にずらす
			m_pCard[i]->SetPos({ m_pCard[i - 1]->GetPos().x+posInterbal, m_pCard[i - 1]->GetPos().y, m_pCard[i - 1]->GetPos().z });
		}
		else
		{
			// 一枚目のカードの座標(基準となる)
			firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };
			m_pCard[0]->SetPos(firstpos);
		}

		if (m_pCard[i]->GetStateNum() != CCardState::CARD_STAY)
		{
			// 元の位置を設定しておく
			m_pCard[i]->SetNeutralPos(m_pCard[i]->GetPos());
			// 一度ニュートラルにリセットする
			m_pCard[i]->ChangeState(CCardState::CARD_STATE::CARD_NEUTRAL);
		}
		
		// 間隔を開ける
		xpos += posInterbal*0.5f;
	}

	// 選択番号も一度リセット
	m_SelectNum = 0;
	
}
