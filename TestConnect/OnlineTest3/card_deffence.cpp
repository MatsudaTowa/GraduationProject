//===========================================================================================================================================================
// 
// ディフェンスカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================

#include "card_deffence.h"
#include "card_strategy.h"
//#include "active_scene_manager.h"
//#include "raknet.h"
#include "zone_manager.h"
#include "card_attack.h"

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