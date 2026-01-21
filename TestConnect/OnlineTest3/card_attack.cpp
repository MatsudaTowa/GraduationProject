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
#include "duel_manager.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardAttack::CCardAttack():
m_AttackType(),
m_nAttackValue(0),
m_DefCardVector(),
m_StackedCardsList(),
m_isTopCastCard(true),
m_pStackCard(nullptr),
m_DamageInfo()
{
	m_DamageInfo.clear();
	m_DefCardVector.clear();
	m_StackedCardsList.clear();
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCardAttack::~CCardAttack()
{
	m_DamageInfo.clear();
	m_DefCardVector.clear();
	m_StackedCardsList.clear();
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
			Target.nTargetCardId = pCard->GetBaseStatus().nCardID;
			Target.nTargetCardSameId = pCard->GetSameTypeId();
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
	//ダメージ対象の数だけ周回
	for (auto& Damage : m_DamageInfo)
	{
		//対象を取得
		CDuel_Player* pPlayer = CDuel_Player_Manager::GetInstance()->GetDuelPlayer(Damage.first);

		//守備カードの周回
		for (auto& pDefCard : m_DefCardVector)
		{
			if (pDefCard->GetUserId() != Damage.first) continue;

			//守備の値だけダメージを減らす
			Damage.second -= pDefCard->GetDefenceValue();

			//ターゲットの守備カードの状態を変更
			pDefCard->ChangeState(CCardState::CARD_TRIGGER, pPlayer);
		}

		//体力が0以下なら飛ばす
		if (pPlayer->GetStatus().life <= 0)
		{
			continue;
		}

		//ダメージがあるなら与える
		if (Damage.second > 0)
		{
			pPlayer->ReceiveDamage(Damage.second);
		}
	}

	//死亡したプレイヤーの確認
	CheckDeathPlayer();

	//重ねているカードを墓地状態にする
	for (auto& StackCard : m_StackedCardsList)
	{
		//状態とゾーンの変更
		StackCard->ChangeState(CCardState::CARD_CEMETERY, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(StackCard->GetUserId()));
		CDuel_Player_Manager::GetInstance()->GetDuelPlayer(StackCard->GetUserId())->GetZoneManager()->MoveZone(this, CastToZone(GetCurrentZone(), CDuel_Player_Manager::GetInstance()->GetDuelPlayer(StackCard->GetUserId())), CDuel_Player_Manager::GetInstance()->GetDuelPlayer(StackCard->GetUserId())->GetZoneManager()->GetCemetery(), true);
		StackCard->SetCurrentZone(CCard::CEMETERY);
	}

	//重ねカードのクリア
	m_StackedCardsList.clear();

	//クライアントにトリガー情報を送信
	CRakNet_Server::GetInstance()->SendTriggerCard(this);

	//カードのクリア
	m_DefCardVector.clear();

	//ゲームの終了を送る
	CRakNet_Server::GetInstance()->SendGameSet();
}

//===========================================================================================================
//トリガー時に送るデータ
//===========================================================================================================
void My::CCardAttack::SendTriggerData(RakNet::BitStream* bsout)
{
	//カードの書き出し
	//bsout->Write(m_StackedCardsList.size());	//重ねているカードの数

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

//===========================================================================================================
//キャストカードの読み込み処理
//===========================================================================================================
bool My::CCardAttack::LoadCastInfo(RakNet::BitStream* bsin, CastDestination destination)
{
	//列挙に応じて処理を送信内容を変更
	switch (destination)
	{
	case CastDestination::AREA:	//エリアの場合

	{
		int nTargetNum = 0;
		bsin->Read(nTargetNum);

		//周回
		for (int i = 0; i < nTargetNum; i++)
		{
			int nId = 0;
			bsin->Read(nId);
			AddTargetIdVector(nId);
		}

		//一番上のカードとして設定
		m_isTopCastCard = true;

		//キャスト状態に変更
		SetStartCastTime(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime());
		ChangeState(My::CCardState::CARD_CAST, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId()));

		////ターゲットの書き出し
		//bsout->Write((int)GetTargetPlayerList().size());

		////ターゲットの番号
		//for (auto iter : GetTargetPlayerList())
		//{
		//	bsout->Write(iter->GetPlayerIdx());
		//}

		return true;
	}

		break;

	case CastDestination::CARD:	//カードの場合

	{
		//変数宣言
		int nTargetCardId = 0;
		int nTagetSameId = 0;

		//読み込み
		bsin->Read(nTargetCardId);
		bsin->Read(nTagetSameId);

		//プレイヤーのキャストゾーンを確認
		My::CDuel_Player* pPlayer = My::CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId());	//使用者の番号を取得
		My::CCardAttack* StackAttackCard = nullptr;															//重ね先のカード

		//キャストカードの確認
		for (My::CCard* pCard : pPlayer->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//一致の確認
			if (pCard->GetBaseStatus().nCardID != nTargetCardId) continue;			//カード番号
			if (pCard->GetSameTypeId() != nTagetSameId) continue;					//同種番号
			if (pCard->GetBaseStatus().Maintype != CCard_Client::ATTACK) continue;	//攻撃カードか

			StackAttackCard = dynamic_cast<My::CCardAttack*>(pCard);	//攻撃カードにキャスト
			break;
		}

		if (!StackAttackCard) return false;	//中身の確認

		//重ねたフラグ
		m_isTopCastCard = false;					//一番上のフラグを下す
		StackAttackCard->AddStackCards(this);		//重ね先のカードの重ねカードに自身を追加
		m_pStackCard = StackAttackCard;				//重ね先の登録
		StackAttackCard->AddDamage(m_nAttackValue);	//攻撃力を加算

		//キャスト状態に変更(NOTE : 重ねているカードはキャストしなくても良いかも)
		SetStartCastTime(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime());
		ChangeState(My::CCardState::CARD_CAST, CDuel_Player_Manager::GetInstance()->GetDuelPlayer(GetUserId()));
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
//ターゲットの追加
//===========================================================================================================
void My::CCardAttack::AddTargetIdVector(int id)
{
	//ダメージ情報の書き出し
	m_DamageInfo[id] = m_nAttackValue;

	My::CCard::AddTargetIdVector(id);	//対象の追加
}

//===========================================================================================================
//ダメージの追加
//===========================================================================================================
void My::CCardAttack::AddDamage(int damage)
{
	//現在のダメージを追加
	for (auto& Damage : m_DamageInfo)
	{
		Damage.second += damage;
	}
}

//===========================================================================================================
//キャスト情報の書き出し
//===========================================================================================================
void My::CCardAttack::SendCastInfo(RakNet::BitStream& bsout)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合

		//ターゲットの数を書き出し(クライアント側でサイズが消失しているため一時的に二つに)
		//bsout.Write((int)GetTargetIdVector().size());
		bsout.Write((int)GetTargetIdVector().size());

		//ターゲットの番号を書き出し
		for (int Id : GetTargetIdVector())
		{
			bsout.Write(Id);
		}

		break;

	case CastDestination::CARD:	//カードの場合

		//重ね先のカード情報を送信
	{
		CCardAttack* pAttackCard = GetStackCard();
		//bsout.Write((int)pAttackCard->GetBaseStatus().nCardID);	//カード番号(謎にここの読み込みが消えているのでもう一つ送るバグです)
		bsout.Write((int)pAttackCard->GetBaseStatus().nCardID);	//カード番号
		bsout.Write((int)pAttackCard->GetSameTypeId());			//同種番号
	}
		
		break;

	default:
		break;
	}
}

//===========================================================================================================
//死亡したプレイヤーの確認
//===========================================================================================================
bool My::CCardAttack::CheckDeathPlayer()
{
	//人数格納用変数
	std::vector<int> PlayerIndexVector;	//現在死んだプレイヤー番号の保存用
	int nDiedPlayer = 0;				//すでに死んでいたプレイヤー数のカウント用

	//プレイヤーを周回して死んだ人数をカウント
	for (auto iter : CDuel_Player_Manager::GetInstance()->GetList())
	{
		//現在の攻撃で死んだプレイヤーの数を数える
		if (iter->GetStatus().life > 0) continue;	//体力が０ではないなら飛ばす

		//すでに死んでいるプレイヤーならカウントして飛ばす
		if (iter->GetIsDeath())
		{
			nDiedPlayer++;
			continue;
		}

		//死んだプレイヤーの番号を保存
		PlayerIndexVector.push_back(iter->GetIndex());	//番号を保存
		iter->SetIsDeath(true);							//死亡フラグを立てる
	}

	//死亡したプレイヤーの順位を算出
	for (int Index : PlayerIndexVector)
	{
		CDuel_Manager::GetInstance()->GetRankMap()[Index] = 3 - nDiedPlayer;
	}

	return true;	//NOTE : 返り値は必要ない可能性
}