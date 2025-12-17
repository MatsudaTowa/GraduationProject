//================================
//
// カードのcpp[card.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "card.h"
#include "duel_player.h"

//=====================================
//コンストラクタ
//=====================================
My::CCard::CCard() : 
	m_Param(),							//カードのパラメータ
	m_CardType(CCard_Client::NONE),		//カードの種類
	m_PreCalculateStrategy(),			//計算前効果のストラテジー
	m_PostCalculateStrategy(),			//計算後効果のストラテジー
	m_TargetIdVector(),					//ターゲット番号
	m_nUserId(-1),						//使用者番号
	m_CurrentZone(DECK),				//現在のゾーン
	m_OldZone(DECK),					//過去のゾーン
	m_BaseParam(),						//ベースのパラメータ
	m_pState(nullptr),					//状態のポインタ
	m_StateNum(),						//列挙の状態
	m_nSameTypeId(0)					//同じ種類のカード番号
{
	m_TargetIdVector.clear();
}

//=====================================
//デストラクタ
//=====================================
My::CCard::~CCard()
{

}

//=====================================
//攻撃パラメータの取得
//=====================================
CCard_Client::AttackParam My::CCard::GetAttackParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::AttackParam AttackParam;

	//代入
	AttackParam.BaseParam.nPackID = param.nPackID;		//パックID
	AttackParam.BaseParam.nCardID = param.nCardID;		//カードID
	AttackParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	AttackParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	AttackParam.BaseParam.Name = param.Name;			//カード名
	AttackParam.BaseParam.nCost = param.nCost;			//コスト
	AttackParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	AttackParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	AttackParam.Attacktype = param.Attacktype;			//攻撃種類
	AttackParam.nDamage = param.nDamage;				//ダメージ

	return AttackParam;
}

//=====================================
//防御パラメータの取得
//=====================================
CCard_Client::DefenseParam My::CCard::GetDefenseParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::DefenseParam DefenseParam;

	//代入
	DefenseParam.BaseParam.nPackID = param.nPackID;		//パックID
	DefenseParam.BaseParam.nCardID = param.nCardID;		//カードID
	DefenseParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	DefenseParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	DefenseParam.BaseParam.Name = param.Name;			//カード名
	DefenseParam.BaseParam.nCost = param.nCost;			//コスト
	DefenseParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	DefenseParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	DefenseParam.Defensetype = param.Defensetype;		//防御種類
	DefenseParam.nCounter = param.nCounter;				//カウンターダメージ
	DefenseParam.nGuard = param.nGuard;					//防御

	return DefenseParam;
}

//=====================================
//アシストパラメータの取得
//=====================================
CCard_Client::AssistParam My::CCard::GetAssistParam(CCard_Client::Param param)
{
	//攻撃パラメータ
	CCard_Client::AssistParam AssistParam;

	//代入
	AssistParam.BaseParam.nPackID = param.nPackID;		//パックID
	AssistParam.BaseParam.nCardID = param.nCardID;		//カードID
	AssistParam.BaseParam.ImagePath = param.ImagePath;	//テクスチャパス
	AssistParam.BaseParam.Maintype = param.Maintype;	//メイン種類
	AssistParam.BaseParam.Name = param.Name;			//カード名
	AssistParam.BaseParam.nCost = param.nCost;			//コスト
	AssistParam.BaseParam.Raritytype = param.Raritytype;//レアリティ
	AssistParam.BaseParam.Ruby = param.Ruby;			//フリガナ
	AssistParam.Bufftype = param.Bufftype;				//バフ種類
	AssistParam.Healtype = param.Healtype;				//回復種類
	AssistParam.nHeal = param.nHeal;					//回復

	return AssistParam;
}

//===========================================================================================================
// ステートを変更する
//===========================================================================================================
void My::CCard::ChangeState(CCardState::CARD_STATE state, CDuel_Player* duel)
{
	if (m_pState != nullptr)
	{
		// 同じステートかチェンジできない場合ここを通らない
		if (m_StateNum == state)
			return;

		// 削除
		delete m_pState;
		m_pState = nullptr;

		switch (state)
		{
		case CCardState::CARD_NEUTRAL:
			m_pState = new CCardStateNeutral();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::HAND, duel), duel->GetZoneManager()->GetHandZone(), true);
			break;

		case CCardState::CARD_PICKUP:
			m_pState = new CCardStatePickup();
			break;

		case CCardState::CARD_SELECT:
			m_pState = new CCardStateSelect();
			break;

		case CCardState::CARD_CAST:
			m_pState = new CCardStateCast();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::CAST, duel), duel->GetZoneManager()->GetCastPreviewZone(), true);
			break;

		case CCardState::CARD_STAY:
			m_pState = new CCardStateStay();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::CAST, duel), duel->GetZoneManager()->GetCastPreviewZone(), true);
			break;

		case CCardState::CARD_WAIT:
			m_pState = new CCardStateWait();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::WAIT, duel), duel->GetZoneManager()->GetWaitZone(), true);
			break;

		case CCardState::CARD_TRIGGER:
			m_pState = new CCardStateTrigger();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::CEMETERY, duel), duel->GetZoneManager()->GetCemetery(), true);
			break;

		case CCardState::CARD_CEMETERY:
			m_pState = new CCardStateCemetery();
			//duel->GetZoneManager()->MoveZone(this, CastToEnumZone(m_CurrentZone, duel), duel->GetZoneManager()->GetCemetery(), true);
			break;

		default:
			assert(1);
			break;
		}

		// ステート番号も保存しておく
		m_StateNum = state;

		// 初期化
		m_pState->Init(this, duel);
		//m_pState->Init();
	}
}

//===========================================================================================================
// 列挙からゾーンのポインタを返す
//===========================================================================================================
My::CZone* My::CCard::CastToEnumZone(ZONE zone, CDuel_Player* duel)
{
	//返す用の変数
	CZone* pZone = nullptr;

	//昔のゾーンを保存
	if (zone != m_CurrentZone)
	{
		m_OldZone = m_CurrentZone;
	}
	else
	{
		return nullptr;
	}

	//引数の列挙をポインタに変換
	pZone = CastToZone(m_OldZone, duel);

	//現在のゾーンの更新
	m_CurrentZone = zone;

	return pZone;
}

//===========================================================================================================
//列挙に対応したゾーンのポインタを返す
//===========================================================================================================
My::CZone* My::CCard::CastToZone(ZONE zone, CDuel_Player* duel)
{
	//返す用の変数
	CZone* pZone = nullptr;

	//ゾーン列挙
	switch (zone)
	{
	case ZONE::DECK:	//デッキ
		pZone = duel->GetZoneManager()->GetDeck();
		m_CurrentZone = ZONE::DECK;	//列挙の登録
		break;

	case ZONE::HAND:	//手札
		pZone = duel->GetZoneManager()->GetHandZone();
		m_CurrentZone = ZONE::HAND;	//列挙の登録
		break;

	case ZONE::CAST:	//キャストプレビュー
		pZone = duel->GetZoneManager()->GetCastPreviewZone();
		m_CurrentZone = ZONE::CAST;	//列挙の登録
		break;

	case ZONE::WAIT:	//待機
		pZone = duel->GetZoneManager()->GetWaitZone();
		m_CurrentZone = ZONE::WAIT;	//列挙の登録
		break;

	case ZONE::FIELD:	//フィールド
		pZone = duel->GetZoneManager()->GetFieldZone();
		m_CurrentZone = ZONE::FIELD;	//列挙の登録
		break;

	case ZONE::CEMETERY:	//墓地
		pZone = duel->GetZoneManager()->GetCemetery();
		m_CurrentZone = ZONE::CEMETERY;	//列挙の登録
		break;
	}

	return pZone;
}
