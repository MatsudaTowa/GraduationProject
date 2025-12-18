//===========================================================================================================================================================
// 
// アタックカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================
#include "card_attack.h"
#include "zone_manager.h"
#include "card_deffence.h"
#include "duel_player_manager.h"
#include "useful_card.h"
#include "card_strategy.h"
#include "raknet_server.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardAttack::CCardAttack():
m_AttackType(),
m_nAttackValue(0),
m_DefCardVector()
{
	m_DefCardVector.clear();
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCardAttack::~CCardAttack()
{
	m_DefCardVector.clear();
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCardAttack::Init()
{
	CCard::SetCardType(CCard_Client::CardType::ATTACK);
	CCard::Init();

	//トリガー時のストラテジーを代入
	AddPostCalculateStrategy(new CAttack);
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
//固有情報の読み込み
//===========================================================================================================
void My::CCardAttack::LoadUniqueInfo(CCard_Client::Param param)
{
	//攻撃ステータスの読み込み
	m_AttackType = (AttackType)param.Attacktype;	//攻撃の種類
	m_nAttackValue = param.nDamage;					//攻撃力
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAttack::Cast(CDuel_Player* duel)
{
	//対象のプレイヤーのみ周回
	for (CDuel_Player* targetplayer : GetTargetPlayer(GetTargetIdVector()))
	{
		//待機状態のカードを確認
		if (!targetplayer->GetZoneManager()->GetWaitZone()->GetList().empty())
		{
			CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(targetplayer->GetZoneManager()->GetWaitZone()->GetList().front());	//先頭のカードの確認
			if (pCard == nullptr) break;

			//守備カードに自身のエリアを追加し、ステイ状態にする
			pCard->AddTargetIdVector(GetUserId());

			//守備対象のターゲットの作成
			CCardDeffence::DiffenceTarget Target = { 0, 0 };

			//使用者の番号を保持
			Target.nAttackCardUserId = GetUserId();

			//パラメータを代入
			Target.nTargetCard = duel->GetZoneManager()->GetCastPreviewZone()->GetList().size() - 1;
			pCard->SetDiffenceTarget(Target);

			//守備カードのポインタを保存
			m_DefCardVector.push_back(pCard);
		}
	}
}

//===========================================================================================================
//ステイ処理
//===========================================================================================================
void My::CCardAttack::Stay()
{
	
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardAttack::Trigger()
{
	////リスト周回
	//for (auto& itr : GetTargetPlayer(GetTargetIdVector()))
	//{
	//	if (itr == nullptr) { continue; }

	//	//守備カードの周回
	//	for (auto& iter : m_DefCardVector)
	//	{
	//		if (iter->GetStateNum() != CCardState::CARD_STAY) continue;

	//		//守備の値だけダメージを減らす
	//		nDamage -= iter->GetDefenceValue();

	//		//ターゲットの守備カードの状態を変更
	//		iter->ChangeState(CCardState::CARD_TRIGGER, DuelState);
	//	}

	//	//ダメージがあるなら与える
	//	if (nDamage > 0)
	//	{
	//		itr->ReceiveDamage(nDamage);
	//	}
	//}

	////カードのクリア
	//m_DefCardVector.clear();

	//周回
	for (auto& iter : GetTargetPlayer(GetTargetIdVector()))
	{
		if (iter == nullptr) { continue; }

		//if (iter->GetArea() != GetTarget()) { continue; }

		// ゾーンマネージャーの取得
		CZoneManager* pZoneManager = nullptr;
		pZoneManager = dynamic_cast<CZoneManager*>(iter->GetZoneManager());

		//攻撃の合計値
		int nTotalAttackValue = 0.0f;

		/*for (auto& pCard : pZoneManager->GetCastPreviewZone()->GetList())
		{
			CCardAttack* pAttackCard = dynamic_cast<CCardAttack*>(pCard);

			if (pAttackCard == nullptr)
				continue;

			nTotalAttackValue += pAttackCard->GetAttackValue();
		}*/

		nTotalAttackValue += m_nAttackValue;

		//ダメージの計算
		int nDamage = nTotalAttackValue;	//与えるダメージ

		////TODO : デュエル状態を参照できる場所が必要
		//CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(iter->GetState());	//キャスト
		//if (DuelState == nullptr) continue;											//中身の確認

		//守備カードの周回
		for (auto& pDefCard : m_DefCardVector)
		{
			if (pDefCard->GetStateNum() != CCardState::CARD_STAY) continue;

			//守備の値だけダメージを減らす
			nDamage -= pDefCard->GetDefenceValue();

			//ターゲットの守備カードの状態を変更
			pDefCard->ChangeState(CCardState::CARD_TRIGGER, iter);
		}

		//ダメージがあるなら与える
		if (nDamage > 0)
		{
			iter->ReceiveDamage(nDamage);
		}
	}

	//クライアントにトリガー情報を送信
	CRakNet_Server::GetInstance()->SendTriggerCard(this);

	//カードのクリア
	m_DefCardVector.clear();
}

//===========================================================================================================
//トリガー時に送るデータ
//===========================================================================================================
void My::CCardAttack::SendTriggerData(RakNet::BitStream* bsout)
{
	return;

	//カードの書き出し
	bsout->Write(m_DefCardVector.size());	//キャストされている守備カードの数

	//守備カードの書き出し
	for (auto iter : m_DefCardVector)
	{
		bsout->Write(iter->GetUserId());				//使用者番号
		bsout->Write(iter->GetBaseStatus().nCardID);	//カード番号
		bsout->Write(iter->GetSameTypeId());			//同種類番号
	}
}
