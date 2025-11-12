//================================
//
//card_manager.cppに必要な宣言[card_manager.cpp]
//Author:松田永久
// 
//================================
#include "card_manager.h"
#include "card_assist.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_manager.h"
#include "card_assist_buff.h"
#include "card_assist_debuff.h"

//================================
// コンストラクタ
//================================
My::CCardManager::CCardManager()
{
	m_CardVector.clear();
}

//================================
// デストラクタ
//================================
My::CCardManager::~CCardManager()
{
	m_CardVector.clear();
}

//================================
// 初期化
//================================
HRESULT My::CCardManager::Init()
{
	return S_OK;
}

//================================
// 終了
//================================
void My::CCardManager::Uninit()
{
}

//================================
// カードのリストに登録
//================================
void My::CCardManager::RegistCardList(CCard_Client::Param card)
{
	//ターゲットリストの情報を登録
	m_CardVector.push_back(card);
}

//================================
// カードのリストから削除
//================================
void My::CCardManager::RemoveCardList(CCard_Client::Param card)
{
	//サイズが0なら抜ける
	if (m_CardVector.size() == 0)
	{
		return;
	}
	//カードの情報を削除
	//m_CardList.remove(card);
}

//================================
// リストのリセット
//================================
void My::CCardManager::ResetCardList()
{
	m_CardVector.clear();
}

//================================
//カードの生成
//================================
void My::CCardManager::CreateCard(int id)
{
	if (m_CardVector.size() < id) return;

	//基本ステータスの代入
	My::CCard::BaseStatus Status;
	Status.imagePath = m_CardVector[id - 1].ImagePath;						//画像パス
	Status.maintype = (My::CCard::CARDTYPE_)m_CardVector[id - 1].Maintype;	//メインタイプ
	Status.name = m_CardVector[id - 1].Name;								//名前
	Status.nCardID = m_CardVector[id - 1].nCardID;							//カードID
	Status.nCost = m_CardVector[id - 1].nCost;								//コスト
	Status.nPackID = m_CardVector[id - 1].nPackID;							//パックID
	Status.raritytype = (My::CCard::RARITY)m_CardVector[id - 1].Raritytype;	//レアリティ
	Status.ruby = m_CardVector[id - 1].Ruby;								//フリガナ

	switch (m_CardVector[id - 1].Maintype)
	{
	case CCard_Client::CardType::ATTACK:
	{
		My::CCardAttack* pAttack = new My::CCardAttack(5);

		//代入
		pAttack->SetAttackValue(m_CardVector[id - 1].nDamage);
		pAttack->SetAttackType((CCardAttack::AttackType)m_CardVector[id - 1].Attacktype);
		pAttack->SetBaseStatus(Status);

		//初期化
		pAttack->Init();
	}
		break;

	case CCard_Client::CardType::DEFENSE:
		{
			My::CCardDeffence* pDeffence = new My::CCardDeffence(5);

			//代入
			pDeffence->SetDefenceType((CCardDeffence::DefenseType)m_CardVector[id - 1].Defensetype);
			pDeffence->SetDefenceValue(m_CardVector[id - 1].nGuard);
			pDeffence->SetCounterValue(m_CardVector[id - 1].nCounter);
			pDeffence->SetBaseStatus(Status);

			//初期化
			pDeffence->Init();
		}

		break;

	case CCard_Client::CardType::ASSIST:
		{
			switch (m_CardVector[id - 1].Assisttype)
			{
			case CCard_Client::AssistType::BUFF:
				{
					My::CCardAssist_Buff* pBuff = new My::CCardAssist_Buff(5);
					pBuff->SetAssistType(My::CCardAssist_Buff::AssistType::BUFF);
					pBuff->SetBuffType(CCardAssist_Buff::BuffType::HEAL);
					pBuff->SetBaseStatus(Status);

					//初期化
					pBuff->Init();
				}
				
				break;

			case CCard_Client::AssistType::DEBUFF:
				{
					My::CCardAssist_Debuff* pDebuff = new My::CCardAssist_Debuff(5);
					pDebuff->SetAssistType(My::CCardAssist_Buff::AssistType::DEBUFF);
					pDebuff->SetDebuffType(My::CCardAssist_Debuff::DebuffType::DEATH);
					pDebuff->SetBaseStatus(Status);

					//初期化
					pDebuff->Init();
				}
				
				break;

			case CCard_Client::AssistType::OBSTRUCT:

				//TODO追加次第実装

				break;
			}
		}
		break;

	default:
		break;
	}
}
