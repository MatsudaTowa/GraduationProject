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
	//SetCardStrategy(new CAttack);

	std::vector<CCardStrategy_Base*> strategy_vector = GetPostCalculateVector();
	strategy_vector.push_back(new CAttack);
	strategy_vector.push_back(new CFlipMyDeck);
	strategy_vector.push_back(new CSendToMyCemetary);

	SetpostCalculateStrategyVector(strategy_vector);
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
bool My::CCardAttack::IsCast(CDuelCharacter*, CInputMouse::AREA)
{
	return true;

	for (auto Target : GetTargetPlayerList())
	{
		//キャラクターリスト
		for (auto& iter : CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			//対象のエリアを持つプレイヤーを探す
			if (Target->GetArea() != iter->GetArea()) continue;

			return true;
		}
	}

	return false;
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAttack::Cast(CDuelCharacter* duel)
{
	////キャラクターリストの取得
	//std::list<My::CActiveSceneCharacter*> CharacterList = CActiveSceneManager::GetInstance()->GetCharacterList();

	////キャラクターの取得
	//for (auto& iter : CharacterList)
	//{
	//	//対象のエリアを持つプレイヤーを探す
	//	if (GetTarget() != iter->GetArea()) continue;
	//	
	//	//デュエル状態にキャスト
	//	//if (typeid(CDuelCharacter) != typeid(*iter->GetState())) break;					//状態の確認
	//	CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(iter->GetState());	//キャスト
	//	if (DuelState == nullptr) break;												//中身の確認

	//	//待機状態のカードを確認
	//	if (!DuelState->GetZoneManager()->GetWaitZone()->GetList().empty())
	//	{
	//		CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(DuelState->GetZoneManager()->GetWaitZone()->GetList().front());	//先頭のカードの確認
	//		if (pCard == nullptr) break;

	//		//守備カードに自身のエリアを追加し、ステイ状態にする
	//		pCard->SetTarget(GetUserArea());

	//		//守備対象のターゲットの作成
	//		CCardDeffence::DiffenceTarget Target = {0, 0};

	//		//キャラクターの周回
	//		for (auto character : My::CActiveSceneManager::GetInstance()->GetCharacterList())
	//		{
	//			//同じエリアのみ通す
	//			if (character->GetArea() != GetUserArea()) continue;

	//			Target.nAttackCardUserId = character->GetPlayerIdx();	//使用者の番号を伝える

	//			break;
	//		}

	//		//パラメータを代入
	//		Target.nTargetCard = duel->GetZoneManager()->GetCastPreviewZone()->GetList().size() - 1;
	//		pCard->SetDiffenceTarget(Target);

	//		pCard->ChangeState(CCardState::CARD_STAY, DuelState);

	//		//守備カードのポインタを保存
	//		m_DefCardVector.push_back(pCard);
	//	}

	//	break;
	//}
	
	//ターゲットの周回
	for (auto& Target : GetTargetPlayerList())
	{
		//デュエル状態にキャスト
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(Target->GetState());	//キャスト
		if (DuelState == nullptr) break;												//中身の確認

		//待機状態のカードを確認
		if (!DuelState->GetZoneManager()->GetWaitZone()->GetList().empty())
		{
			CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(DuelState->GetZoneManager()->GetWaitZone()->GetList().front());	//先頭のカードの確認
			if (pCard == nullptr) break;

			//守備カードに自身のエリアを追加し、ステイ状態にする
			//pCard->SetTarget(GetUserArea());
			pCard->RegistTargetList(My::CActiveSceneManager::GetInstance()->GetCharacter(GetUserId()));

			//守備対象のターゲットの作成
			CCardDeffence::DiffenceTarget Target = { 0, 0 };

			//キャラクターの周回
			for (auto character : My::CActiveSceneManager::GetInstance()->GetCharacterList())
			{
				//同じエリアのみ通す
				if (character->GetArea() != GetUserArea()) continue;

				Target.nAttackCardUserId = character->GetPlayerIdx();	//使用者の番号を伝える

				break;
			}

			//パラメータを代入
			Target.nTargetCard = duel->GetZoneManager()->GetCastPreviewZone()->GetList().size() - 1;
			pCard->SetDiffenceTarget(Target);

			pCard->ChangeState(CCardState::CARD_STAY, DuelState);

			//守備カードのポインタを保存
			m_DefCardVector.push_back(pCard);
		}

		break;
	}
}

//===========================================================================================================
//ステイ処理
//===========================================================================================================
void My::CCardAttack::Stay()
{
	//守備カードの位置更新
	for (CCardDeffence* pCard : m_DefCardVector)
	{
		pCard->SetPos({ GetPos().x + CCardDeffence::STEY_SPACE, GetPos().y, GetPos().z });
	}
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardAttack::Trigger()
{
	//ステイ後に起動
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	
	////リスト周回
	//for (auto& itr : List)
	//{
	//	if (itr == nullptr) { continue; }

	//	if (itr->GetArea() != GetTarget()) { continue; }

	//	// ゾーンマネージャーの取得
	//	CZoneManager* pZoneManager = nullptr;
	//	pZoneManager = dynamic_cast<CDuelCharacter*>(itr->GetState())->GetZoneManager();

	//	float totalattackvalue = 0.0f;

	//	for (auto& iter : pZoneManager->GetCastPreviewZone()->GetList())
	//	{
	//		CCardAttack* pAttackCard = dynamic_cast<CCardAttack*>(iter);

	//		if (pAttackCard == nullptr)
	//			continue;

	//		totalattackvalue += pAttackCard->GetAttackValue();
	//	}

	//	totalattackvalue += m_nAttackValue;

	//	//ダメージの計算
	//	int nDamage = totalattackvalue;	//与えるダメージ

	//	//TODO : デュエル状態を参照できる場所が必要
	//	CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(itr->GetState());	//キャスト
	//	if (DuelState == nullptr) continue;											//中身の確認

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
	for (auto& iter : GetTargetPlayerList())
	{
		if (iter == nullptr) { continue; }

		//if (iter->GetArea() != GetTarget()) { continue; }

		// ゾーンマネージャーの取得
		CZoneManager* pZoneManager = nullptr;
		pZoneManager = dynamic_cast<CDuelCharacter*>(iter->GetState())->GetZoneManager();

		// 重なったカードの合計攻撃値
		float totalattackvalue = 0.0f;

		/**
		* TODO
		* 現段階の下記コードではまるでOverlapCardListから
		* カードを取り出して攻撃力加算をしているように見えるが
		* 全然そんなことはできていなく、
		* OverlapCardlistの要素が消されているため加算できていない
		* これは私が直すのか、あるいは島津さんが通信処理と一緒に直してくれるのか...
		*/
		for (auto& pOverlapCards : pZoneManager->GetCastPreviewZone()->GetOverlapManager()->GetOverlapCardList())
		{
			if (pOverlapCards->GetTarget() != iter->GetArea())
				continue;

			// 重なったカードの動的な配列からカードを抜き出す
			for (auto& Card : pOverlapCards->GetOverlapCards())
			{
				// カードを攻撃カードにキャスト
				CCardAttack* pAttackCard = dynamic_cast<CCardAttack*>(Card);

				if (pAttackCard == nullptr)
					continue;

				// カードから攻撃力を取得して合計攻撃力に加算する
				totalattackvalue += pAttackCard->GetAttackValue();

				// カードを削除
				pOverlapCards->ReMove(pAttackCard);
			}
		}

		// Overlap の vector を削除
		pZoneManager->GetCastPreviewZone()->GetOverlapManager()->ReMove();

		// 最後にこのカードの攻撃力を加算
		totalattackvalue += m_nAttackValue;

		//ダメージの計算
		int nDamage = totalattackvalue;	//与えるダメージ

		//TODO : デュエル状態を参照できる場所が必要
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(iter->GetState());	//キャスト
		if (DuelState == nullptr) continue;											//中身の確認

		//守備カードの周回
		for (auto& pDefCard : m_DefCardVector)
		{
			if (pDefCard->GetStateNum() != CCardState::CARD_STAY) continue;

			//守備の値だけダメージを減らす
			nDamage -= pDefCard->GetDefenceValue();

			//ターゲットの守備カードの状態を変更
			pDefCard->ChangeState(CCardState::CARD_TRIGGER, DuelState);
		}

		//ダメージがあるなら与える
		if (nDamage > 0)
		{
			iter->ReceiveDamage(nDamage);
		}
	}

	//カードのクリア
	m_DefCardVector.clear();
}

//===========================================================================================================
//トリガー受信時の処理
//===========================================================================================================
void My::CCardAttack::ReceiveTrigger()
{
	//守備カード周回
	for (auto& iter : m_DefCardVector)
	{
		//使用者を見つけゾーンの位置を変更
		CActiveSceneCharacter* pUsedPlayer = CActiveSceneManager::GetInstance()->GetCharacter(iter->GetUserId());
		My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pUsedPlayer->GetState());

		//キャスト失敗時にアサート
		if (!pDuelState) assert(false);

		//状態とゾーンの変更
		iter->ChangeState(CCardState::CARD_CEMETERY, pDuelState);
		pDuelState->GetZoneManager()->MoveZone(this, CastToZone(GetCurrentZone(), pDuelState), pDuelState->GetZoneManager()->GetCemetery(), true);
		iter->SetCurrentZone(CCard::CEMETERY);
	}

	//カードのクリア
	m_DefCardVector.clear();

	//親の処理
	My::CCard::ReceiveTrigger();
}
