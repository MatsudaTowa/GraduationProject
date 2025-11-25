//===========================================================================================================================================================
// 
// 手札の処理 [hand.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "hand.h"
#include "card_state.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_assist.h"
#include "active_scene.h"
#include "active_scene_manager.h"
#include "active_scene_state.h"
#include "zone_manager.h"
#include <algorithm>

My::CHand::CHand() :
	m_SelectNum(-1),
	m_TotalNum(0),
	m_IsPassStart(false),
	m_IsPickUp(false),
	m_HandState(NEUTRAL)/*,
	m_pHandCard(),
	m_pStayCard(),
	m_pTriggerCard()*/
{
	m_pHandList.clear();

	/*for (int i = 0; i < MAX_HANDSCARD; i++)
	{
		m_pCard[i] = nullptr;
	}*/
}

My::CHand::~CHand()
{
	m_pHandList.clear();
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

	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	m_CenterPos = { pCamera->GetPosV().x, pCamera->GetPosV().y - 150.0f, pCamera->GetPosV().z + 23.0f };
}

//===========================================================================================================
// 開始時処理
//===========================================================================================================
void My::CHand::Start(CPlayerDuelState* state)
{
	// 一度通ったら通らない
	if (m_IsPassStart)
		return;

	// スタート時のカードの枚数分ドローする
	HandDraw(START_HANDS, state);

	// スタートを通ったら、この試合ではスタートを通らない
	m_IsPassStart = true;
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CHand::Update(CPlayerDuelState* state, CActiveSceneCharacter* player)
{
	// キーボード取得
	CInputKeyboard* pkeyboad = CManager::GetInstance()->GetKeyboard();

	// ステータス取得
	CActiveSceneCharacter::Status status = CActiveSceneManager::GetInstance()->GetPlayer()->GetStatus();

	// 手札ドロー
	if (pkeyboad->GetTrigger(DIK_SPACE))
	{
		if (status.energy > 0 && m_TotalNum < MAX_HANDSCARD)
		{// 消費できるエナジーがなかったらドローできない

			HandDraw(1, state);
			--status.energy;
			CActiveSceneManager::GetInstance()->GetPlayer()->SetStatus(status);
		}
	}

	for (auto& iter : m_pHandList)
	{
		iter->Update(state);
	}

	// 手札選択
	Select(state);

	// カードキャスト
	Cast(state, player);

	// カード除去
	DeleteCard(state);
}

//===========================================================================================================
// 選択
//===========================================================================================================
void My::CHand::Select(CDuelCharacter* character)
{
	// キャスト状態だったらセレクトさせない
	if (m_HandState == SELECT)
	{
		return;
	}

	// 何も選択されていない場合
	if (!m_IsPickUp)
	{
		int nCount = 0;
		for (auto& iter : m_pHandList)
		{
			// マウスでカード選択
			m_IsPickUp = iter->CardSelectToMouse(character);

			if (m_IsPickUp)
			{// どれかのカードが選択されたら
				m_SelectNum = nCount;	// 今の配列番号を一時格納しておく
				break;
			}
			else
			{
				nCount++;
			}
		}
	}
	else
	{
		int nCount = 0;

		for (auto& iter : m_pHandList)
		{
			if (nCount != m_SelectNum)
			{
				nCount++;
				continue;
			}

			// 選択番号のカードが非選択状態になったら
			if (iter->GetStateNum() == CCardState::CARD_NEUTRAL)
			{
				m_IsPickUp = false;	// 選択されていない状態にする
			}
			else
			{
				// 選択番号のカードのみ判定する
				m_IsPickUp = iter->CardSelectToMouse(character);
			}
			break;
		}
	}
}

//===========================================================================================================
// カードのキャスト
//===========================================================================================================
void My::CHand::Cast(CDuelCharacter* character, CActiveSceneCharacter* player)
{
	if (m_SelectNum < 0) return;

	int nCount = 0;

	for (auto& iter : m_pHandList)
	{
		if (nCount != m_SelectNum)
		{
			nCount++;
			continue;
		}

		// 返り値でキャスト状態かどうか判断
		bool IsCast = iter->CardCastToMouse(character, player);

		// キャスト状態によって手札の状態を変える
		if (IsCast)
			m_HandState = SELECT;
		else
			m_HandState = NEUTRAL;
		break;
	}
}

My::CCard* My::CHand::SearchHandList(int num)
{
	auto itr = m_pHandList.begin();

	// リスト分回す
	for (unsigned int i = 0; i < m_pHandList.size(); i++)
	{
		if (i == num){
			return *itr;
		}
		itr++;
	}

	return nullptr;
}

//===========================================================================================================
// 手札のカードの状態変更
//===========================================================================================================
void My::CHand::SelectStateSet(CDuelCharacter* character)
{
	int nCount = 0;
	CCard* pCard = nullptr;

	// すべてのカードを選ばれていない状態にする
	for (auto& iter : m_pHandList)
	{
		///選択中のカードなら代入
		if (m_SelectNum == nCount)
		{
			pCard = iter;
		}

		iter->ChangeState(CCardState::CARD_STATE::CARD_NEUTRAL, character);
		nCount++;
	}

	if (!pCard || m_SelectNum <= -1)
		return;

	// 選択中のカードのステートを変える
	if (pCard != nullptr)
	{
		pCard->ChangeState(CCardState::CARD_STATE::CARD_PICKUP, character);
	}
}

//===========================================================================================================
// カード消去
//===========================================================================================================
void My::CHand::DeleteCard(CDuelCharacter* character)
{
	bool isDecrease = false;

	for (auto& iter : m_pHandList)
	{
		//トリガーカードを消去
		if (iter->GetStateNum() == CCardState::CARD_STATE::CARD_TRIGGER ||
			iter->GetStateNum() == CCardState::CARD_STATE::CARD_WAIT ||
			iter->GetStateNum() == CCardState::CARD_STATE::CARD_STAY ||
			iter->GetStateNum() == CCardState::CARD_STATE::CARD_CAST)
		{
			//削除処理とフラグを立てる
			//iter->Uninit();
			m_pHandList.remove(iter);
			isDecrease = true;
			break;
		}
	}

	//カードが減ってないなら抜ける
	if (!isDecrease) return;

	//手札総数を減らす
	m_TotalNum--;

	for (auto& iter : m_pHandList)
	{
		if (iter->GetStateNum() == CCardState::CARD_STATE::CARD_STAY)
		{//ステイ中のカードがあったら手札の整理をしない
			//return;
		}
	}

	// 手札の位置をセットする
	SetHandCardPos(character);
}

//===========================================================================================================
// カード設定
//===========================================================================================================
void My::CHand::SetCard(CCard::CARDTYPE_ type)
{
	// 一時的に保管する
	CCard* pCard = nullptr;

	switch (type)
	{
	case CCard::CARDTYPE_::TYPE_ATTACK:
		pCard = CCardAttack::Create(type);
		break;

	case CCard::CARDTYPE_::TYPE_DEFFENCE:
		pCard = CCardDeffence::Create(type);
		break;

	case CCard::CARDTYPE_::TYPE_ASSIST:
		pCard = CCardAssist::Create(type);
		break;
	}

	// リストテスト
	m_pHandList.push_back(pCard);
}

//===========================================================================================================
// 手札を引く
//===========================================================================================================
void My::CHand::HandDraw(int drawnum, CPlayerDuelState* state)
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
		//トップのカードを取得
		CZoneManager* pZoneManager = state->GetZoneManager();
		CCard* pDrawCard = pZoneManager->GetDeck()->GetTopCard();
		
		// TODO : 一旦ここでランダムに抽選する
		//int rundom;
		//rundom = static_cast<int>(Rundom(CCard::CARDTYPE_::TYPE_ATTACK, CCard::CARDTYPE_::TYPE_MAX));

		//手札に加える処理
		m_pHandList.push_back(pDrawCard);
		pZoneManager->MoveZone(pDrawCard, pZoneManager->GetDeck(), pZoneManager->GetHandZone(), true);

		pDrawCard->SetCurrentZone(CCard::HAND);
		//SetCard((CCard::CARDTYPE_)rundom);
		m_TotalNum++;	// 手札の総数を増やす

		// 手札の位置整理
		SetHandCardPos(state);
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
void My::CHand::SetHandCardPos(CDuelCharacter* character)
{
	D3DXVECTOR3 firstpos;	// 一番左側の位置(手札の最初の位置)
	float posInterbal = 25.0f - (20 * m_TotalNum * 0.07f);	// 手札に表示されている時のカードの間隔
	float xpos;	// 一枚目のカードのx座標
	int nCount = 0;	//周回数
	CCard* pCard = nullptr;	//カードのポインタ

	// x座標の設定 = センター - (現在の合計枚数 * カードの間隔の半分)
	xpos = (m_CenterPos.x) - ((m_TotalNum-1)* posInterbal*0.5f);

	for (auto& iter : m_pHandList)
	{
		if (nCount == 0)
		{
			firstpos = { xpos,m_CenterPos.y,m_CenterPos.z };
			iter->SetPos(firstpos);
		}
		else
		{
			// 一枚目以外は前の手札の位置を参照して "Interbal" 分横にずらす
			iter->SetPos({ pCard->GetPos().x + posInterbal, pCard->GetPos().y, pCard->GetPos().z });
		}

		if (iter->GetStateNum() != CCardState::CARD_STAY)
		{
			// 元の位置を設定しておく
			iter->SetNeutralPos(iter->GetPos());
			// 一度ニュートラルにリセットする
			iter->ChangeState(CCardState::CARD_STATE::CARD_NEUTRAL, character);
		}

		//前回のカードのポインタを代入
		pCard = iter;

		nCount++;

		// 間隔を開ける
		xpos += posInterbal * 0.5f;
	}

	// 選択番号も一度リセット
	m_SelectNum = 0;
	
}
