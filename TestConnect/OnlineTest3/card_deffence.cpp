//===========================================================================================================================================================
// 
// ディフェンスカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================

#include "card_deffence.h"
#include "card_strategy.h"
#include "zone_manager.h"
#include "card_attack.h"
#include "duel_player_manager.h"
#include "duel_manager.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardDeffence::CCardDeffence():
m_DefenceType(),
m_nDefenceValue(0),
m_nCounterValue(0),
m_isStay(false),
m_TargetInfo()
{
	m_TargetInfo.clear();
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCardDeffence::~CCardDeffence()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCardDeffence::Init()
{
	//SetCardType(CCard::CARDTYPE_::TYPE_DEFFENCE);
	CCard::SetCardType(CCard_Client::CardType::DEFENSE);
	CCard::Init();

	//トリガー時のストラテジーを代入
	AddPostCalculateStrategy(new CDiffence);
	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCardDeffence::Uninit()
{
	CCard::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCardDeffence::Update()
{
	CCard::Update();
}

//===========================================================================================================
//固有情報の読み込み
//===========================================================================================================
void My::CCardDeffence::LoadUniqueInfo(CCard_Client::Param param)
{
	//攻撃ステータスの読み込み
	m_DefenceType = (DefenseType)param.Defensetype;	//防御の種類
	m_nCounterValue = param.nCounter;				//カウンター値
	m_nDefenceValue = param.nGuard;					//防御値
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardDeffence::Cast(CDuel_Player* duel)
{
	
}

//===========================================================================================================
//ステイ処理
//===========================================================================================================
void My::CCardDeffence::Stay()
{
	////対象のカード
	//CCard* pCard = nullptr;

	////ターゲット周回
	//for (auto& iter : m_TargetInfo)
	//{
	//	for (auto& Character : My::CActiveSceneManager::GetInstance()->GetCharacterList())
	//	{
	//		if (iter.nAttackCardUserId != Character->GetPlayerIdx()) continue;

	//		CDuelCharacter* DuelState = nullptr;

	//		//対戦状態にキャスト
	//		DuelState = dynamic_cast<CDuelCharacter*>(Character->GetState());

	//		if (DuelState == nullptr) continue;	//キャスト成功したかの確認

	//		int nCount = 0;

	//		//手札のカードを周回し、受信したカードを探す
	//		for (auto& Card : DuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
	//		{
	//			if (nCount != iter.nTargetCard)
	//			{
	//				++nCount;
	//				continue;
	//			}

	//			pCard = Card;
	//			break;
	//		}
	//	}
	//}
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardDeffence::Trigger()
{
	////ステイ後に起動
	//std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	////リスト周回
	//for (auto& itr : List)
	//{
	//	if (itr == nullptr) { continue; }

	//	if (itr->GetArea() != GetTarget()) { continue; }

	//	//ダメージがあるなら与える
	//	if (m_nCounterValue > 0)
	//	{
	//		itr->ReceiveDamage(m_nCounterValue);
	//	}
	//}
}

//===========================================================================================================
//キャストカードの読み込み処理
//===========================================================================================================
bool My::CCardDeffence::LoadCastInfo(RakNet::BitStream* bsin, CastDestination destination)
{
	//列挙に応じて処理を送信内容を変更
	switch (destination)
	{
	case CastDestination::AREA:	//エリアの場合

		//キャスト状態に変更
		SetStartCastTime(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime());
		ChangeState(My::CCardState::CARD_CAST, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId()));
		return true;
		break;

	case CastDestination::CARD:	//カードの場合

	{
		//変数宣言
		int nUserId = 0;	//使用者番号
		int nCardId = 0;	//カード番号
		int nSameId = 0;	//同種番号
		My::CDuel_Player* pPlayer = nullptr;

		//読み込み
		bsin->Read(nUserId);
		bsin->Read(nCardId);
		bsin->Read(nSameId);

		//番号が一致するプレイヤーの取得
		pPlayer = My::CDuel_Player_Manager::GetInstance()->GetDuelPlayer(nUserId);

		if (!pPlayer) return false;	//中身の確認

		//対象を代入用の攻撃カードポインタ
		My::CCardAttack* pAttackCard = nullptr;

		//相手のキャストゾーンのカードを確認
		for (My::CCard* pCard : pPlayer->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//受信した対象のカードがあるかを確認
			if (pCard->GetBaseStatus().Maintype != CCard_Client::ATTACK) continue;	//攻撃カードか
			if (pCard->GetBaseStatus().nCardID != nCardId) continue;				//カード番号
			if (pCard->GetSameTypeId() != nSameId) continue;						//同種番号

			//攻撃カードにキャストして
			pAttackCard = dynamic_cast<My::CCardAttack*>(pCard);
			break;
		}

		//攻撃カードの中身を確認
		if (!pAttackCard) return false;

		//攻撃カードに自身を登録
		pAttackCard->AddDefCard(this);

		//ターゲット情報の代入
		DiffenceTarget Target = { pAttackCard->GetUserId(), pAttackCard->GetBaseStatus().nCardID, pAttackCard->GetSameTypeId() };
		m_TargetInfo.push_back(Target);

		return true;
	}

		break;
	default:

		return false;
		break;
	}

	return false;
}

//===========================================================================================================
//キャスト情報の書き出し
//===========================================================================================================
void My::CCardDeffence::SendCastInfo(RakNet::BitStream* bsout)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合

		break;

	case CastDestination::CARD:	//カードの場合

		//重ね先のカード情報を送信
		bsout->Write(m_TargetInfo[0].nAttackCardUserId);	//対象カードの使用者番号
		bsout->Write(m_TargetInfo[0].nTargetCardId);		//カード番号
		bsout->Write(m_TargetInfo[0].nTargetCardSameId);	//同種番号

		break;

	default:
		break;
	}
}