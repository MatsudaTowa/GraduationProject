//===========================================================================================================================================================
// 
// アタックカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================
#include "card_attack.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "zone_manager.h"
#include "card_deffence.h"

My::CCardAttack::CCardAttack(int nPriority):CCard(nPriority),
m_AttackType(),
m_nAttackValue(INT_ZERO),
m_DefCardVector()
{
	m_DefCardVector.clear();
}

My::CCardAttack::~CCardAttack()
{
	m_DefCardVector.clear();
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCardAttack::Init()
{
	SetCardType(CCard::CARDTYPE_::TYPE_ATTACK);
	CCard::Init();

	//トリガー時のストラテジーを代入
	SetCardStrategy(new CAttack);
	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCardAttack::Uninit()
{
	CCard::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCardAttack::Update()
{
	CCard::Update();
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CCardAttack::Draw()
{
	CCard::Draw();
}

//===========================================================================================================
// カード情報読み込み
//===========================================================================================================
void My::CCardAttack::LoadCardData()
{
	//TODO:各ステータス読み込みしてから親クラスの読み込み
	CCard::LoadCardData();
}

//===========================================================================================================
//固有情報の読み込み
//===========================================================================================================
void My::CCardAttack::LoadUniqueInfo(CCard_Client::Param param)
{
	//攻撃ステータスの読み込み
	m_AttackType = (AttackType)param.Attacktype;	//攻撃の種類
	m_nAttackValue = param.nDamage;					//攻撃力
}

//===========================================================================================================
//キャストをしたかの確認
//===========================================================================================================
bool My::CCardAttack::IsCast(CDuelCharacter* duel)
{
	//キャラクターリスト
	for (auto& iter : CActiveSceneManager::GetInstance()->GetCharacterList())
	{
		//対象のエリアを持つプレイヤーを探す
		if (GetTarget() != iter->GetArea()) continue;

		return true;
	}

	return false;
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAttack::Cast(CDuelCharacter* duel)
{
	//キャラクターの取得
	for (auto& iter : CActiveSceneManager::GetInstance()->GetCharacterList())
	{
		//対象のエリアを持つプレイヤーを探す
		if (GetTarget() != iter->GetArea()) continue;
		
		//デュエル状態にキャスト
		//if (typeid(CDuelCharacter) != typeid(*iter->GetState())) break;					//状態の確認
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(iter->GetState());	//キャスト
		if (DuelState == nullptr) break;												//中身の確認

		//待機状態のカードを確認
		if (!DuelState->GetZoneManager()->GetWaitZone()->GetList().empty())
		{
			CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(DuelState->GetZoneManager()->GetWaitZone()->GetList().front());	//先頭のカードの確認
			if (pCard == nullptr) break;

			//守備カードに自身のエリアを追加し、ステイ状態にする
			pCard->SetTarget(GetUserArea());
			pCard->ChangeState(CCardState::CARD_STAY, DuelState);

			//守備カードのポインタを保存
			m_DefCardVector.push_back(pCard);
		}

		break;
	}
	
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardAttack::Trigger()
{
	//ステイ後に起動
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	
	//リスト周回
	for (auto& itr : List)
	{
		if (itr == nullptr) { continue; }

		if (itr->GetArea() != GetTarget()) { continue; }

		//ダメージの計算
		int nDamage = m_nAttackValue;	//与えるダメージ

		//TODO : デュエル状態を参照できる場所が必要
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(itr->GetState());	//キャスト
		if (DuelState == nullptr) continue;											//中身の確認

		////キャスト状態のカードを確認
		//if (!DuelState->GetZoneManager()->GetCastPreviewZone()->GetList().empty())
		//{
		//	//キャストカードの周回
		//	for (auto& iter : DuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
		//	{
		//		//守備カードかの確認
		//		if (iter->GetCardType() != CARDTYPE_::TYPE_DEFFENCE) continue;

		//		//TODO : ディフェンスカードを参照できるマネージャーが必要
		//		CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(iter);
		//		if (pCard == nullptr) continue;	//キャストに失敗したら飛ばす

		//		//守備の値だけダメージを減らす
		//		nDamage -= pCard->GetDefenceValue();

		//		//ターゲットの守備カードの状態を変更
		//		pCard->ChangeState(CCardState::CARD_TRIGGER, DuelState);
		//	}
		//}

		//守備カードの周回
		for (auto& iter : m_DefCardVector)
		{
			if (iter->GetStateNum() != CCardState::CARD_STAY) continue;

			//守備の値だけダメージを減らす
			nDamage -= iter->GetDefenceValue();

			//ターゲットの守備カードの状態を変更
			iter->ChangeState(CCardState::CARD_TRIGGER, DuelState);
		}

		//ダメージがあるなら与える
		if (nDamage > 0)
		{
			itr->ReceiveDamage(nDamage);
		}
	}

	//カードのクリア
	m_DefCardVector.clear();
}