//=============================================
//
//ゲームマネージャー[activemaneger.cpp]
//Auther MatsudaTowa
//
//=============================================
#include "active_scene_manager.h"
#include "raknet.h"
#include "card_manager.h"
#include "zone_manager.h"
#include "card_attack.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneManager::CActiveSceneManager():
m_pState(nullptr),			//ゲームのステート
m_pEnemyManager(),			//エネミーマネージャー
m_pAreaManager(),			//エリアマネージャー
m_pPlayer(nullptr),			//プレイヤーのポインタ
m_pField(nullptr),			//床のポインタ
m_isFinish(false)
{
}

//=============================================
// デストラクタ
//=============================================
My::CActiveSceneManager::~CActiveSceneManager()
{
}

//=============================================
// 終了
//=============================================
void My::CActiveSceneManager::Uninit()
{
	if (m_pAreaManager != nullptr)
	{
		m_pAreaManager->Uninit();
		delete m_pAreaManager;
		m_pAreaManager = nullptr;
	}
	//エネミーマネージャーの破棄
	if (m_pEnemyManager != nullptr)
	{
		//終了処理後に開放
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}
	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}
}

//=============================================
//ステート変更
//=============================================
void My::CActiveSceneManager::ChangeState(CActiveSceneState* state)
{
	CActiveSceneState* current_state = CActiveSceneManager::GetInstance()->GetState();
	//今のステートを消し引数のステートに切り替える
	if (current_state != nullptr)
	{
		if (typeid(*current_state) == typeid(*state))
		{//現在と同じだったら関数を抜ける
			return;
		}
		delete current_state;

		current_state = state;
		CActiveSceneManager::GetInstance()->SetState(current_state);
	}
}

//=============================================
//キャラクターのリストを返す
//=============================================
std::list<My::CActiveSceneCharacter*> My::CActiveSceneManager::GetCharacterList()
{
	//返す用のリスト
	std::list<My::CActiveSceneCharacter*> List;
	List.clear();

	//プレイヤーの追加
	List.push_back(m_pPlayer);

	//敵の追加
	for (auto& iter : m_pEnemyManager->GetList())
	{
		List.push_back(iter);
	}

	//返す
	return List;
}

//=============================================
//対戦中の通信処理
//=============================================
void My::CActiveSceneManager::DuelConnect()
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//自身のステータスを送る
	CRakNet::GetInstance()->SendMyStatus();

	//全体との更新を合わせる
	while (!CRakNet::GetInstance()->GetIsUpdate())
	{
		//通信処理
		CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
	}

	//カード情報の読み込み
	if (IsCardCast())
	{
		CardCast();
	}

	//カード情報の読み込み
	if (IsDefCardCast())
	{
		DefCardCast();
	}
}

//=============================================
//キャストカードの確認
//=============================================
bool My::CActiveSceneManager::IsCardCast()
{
	if (!CRakNet::GetInstance()->GetOnline()) return false;	//オンラインじゃない
	if (m_CastCardVector.empty()) return false;				//使われたカードが存在しない

	return true;
}

//=============================================
//キャストカードの確認
//=============================================
bool My::CActiveSceneManager::IsDefCardCast()
{
	if (!CRakNet::GetInstance()->GetOnline()) return false;	//オンラインじゃない
	if (m_CastDiffenceCardVector.empty()) return false;		//使われたカードが存在しない

	return true;
}

//=============================================
//カードのキャスト処理
//=============================================
void My::CActiveSceneManager::CardCast()
{
	//キャストカードの処理
	for (auto iter : m_CastCardVector)
	{
		//自分が使用したカードは飛ばす
		if (CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() == iter.nUsePlayer)
		{
			continue;
		}

		//使用するカードと使用者の状態
		CCard* pCard = nullptr;
		CDuelCharacter* DuelState = nullptr;

		//TODOこの下に読み込みこんだカードの処理を追加予定
		for (auto& Character : CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			//通すかの確認
			if (iter.nUsePlayer != Character->GetPlayerIdx()) continue;				//使用者の番号と一致するか
			//if (typeid(CDuelCharacter*) == typeid(*Character->GetState())) continue;//状態の確認

			//対戦状態にキャスト
			DuelState = dynamic_cast<CDuelCharacter*>(Character->GetState());

			if (DuelState == nullptr) continue;	//キャスト成功したかの確認

			//手札のカードを周回し、受信したカードを探す
			for (auto& Card : DuelState->GetZoneManager()->GetDeck()->GetList())
			{
				if (Card->GetBaseStatus().nCardID != iter.nCardID) continue;

				pCard = Card;
				break;
			}
			break;
		}

		//使用者のエリアを代入
		for (auto Character : CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			//使用者を見つけてエリアを代入
			if (iter.nUsePlayer != Character->GetPlayerIdx()) continue;
			pCard->SetUserArea(Character->GetArea());
			break;
		}

		//対象の数だけ周回
		for (auto nTarget : iter.Target)
		{
			//対象者のエリアを代入
			for (auto Character : CActiveSceneManager::GetInstance()->GetCharacterList())
			{
				//使用者を見つけてエリアを代入
				if (nTarget != Character->GetPlayerIdx()) continue;
				pCard->SetTarget(Character->GetArea());
				break;
			}
		}

		//カードの状態をステイに変更(TODO:守備カードはステイじゃない)
		pCard->ChangeState(CCardState::CARD_CAST, DuelState);
	}

	//カード情報のクリア
	m_CastCardVector.clear();
}

//=============================================
//守備カードのキャスト処理
//=============================================
void My::CActiveSceneManager::DefCardCast()
{
	//キャストカードの処理
	for (auto iter : m_CastDiffenceCardVector)
	{
		//自分が使用したカードは飛ばす
		if (CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() == iter.nUsePlayer)
		{
			continue;
		}

		//使用するカードと使用者の状態
		CCard* pCard = nullptr;
		CDuelCharacter* DuelState = nullptr;

		//TODOこの下に読み込みこんだカードの処理を追加予定
		for (auto& Character : CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			//通すかの確認
			if (iter.nUsePlayer != Character->GetPlayerIdx()) continue;				//使用者の番号と一致するか
			//if (typeid(CDuelCharacter*) == typeid(*Character->GetState())) continue;//状態の確認

			//対戦状態にキャスト
			DuelState = dynamic_cast<CDuelCharacter*>(Character->GetState());

			if (DuelState == nullptr) continue;	//キャスト成功したかの確認

			//手札のカードを周回し、受信したカードを探す
			for (auto& Card : DuelState->GetZoneManager()->GetDeck()->GetList())
			{
				if (Card->GetBaseStatus().nCardID != iter.nCardID) continue;

				pCard = Card;
				break;
			}
			break;
		}

		//使用者のエリアを代入
		for (auto Character : CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			//使用者を見つけてエリアを代入
			if (iter.nUsePlayer != Character->GetPlayerIdx()) continue;
			pCard->SetUserArea(Character->GetArea());
			break;
		}

		//対象の数だけ周回
		for (auto nTarget : iter.DiffenceTarget)
		{
			//対象者のエリアを代入
			for (auto Character : CActiveSceneManager::GetInstance()->GetCharacterList())
			{
				//対象者を見つけてエリアを代入
				if (nTarget.nAttackCardUserId != Character->GetPlayerIdx()) continue;
				pCard->SetTarget(Character->GetArea());

				//相手の対戦状態にキャストし、キャストプレビューゾーンを確認
				CDuelCharacter* EnemyDuelState = nullptr;

				//対戦状態にキャスト
				EnemyDuelState = dynamic_cast<CDuelCharacter*>(Character->GetState());
				if (EnemyDuelState == nullptr) continue;	//キャスト成功したかの確認

				//守備をされたカードを探す
				int nCount = 0;
				for (auto& AttackCard : EnemyDuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
				{
					//異なるカードなら飛ばす
					if (nTarget.nTargetCard != nCount)
					{
						++nCount;
						continue;
					}

					//対象カードを攻撃カードにキャスト
					CCardAttack* pAttackCard = nullptr;
					pAttackCard = dynamic_cast<CCardAttack*>(AttackCard);
					if (pAttackCard == nullptr) break;

					//使用カードを守備カードにキャスト
					CCardDeffence* pDeffenceCard = nullptr;
					pDeffenceCard = dynamic_cast<CCardDeffence*>(pCard);
					if (pDeffenceCard == nullptr) break;

					//守備カードを追加
					pAttackCard->AddDefCard(pDeffenceCard);

					//TODO守備カードに対象の情報を入れる
				}

				break;
			}
		}

		//対象が自分ならキャスト状態にする
		if (pCard->GetTarget() == pCard->GetUserArea())
		{
			pCard->ChangeState(CCardState::CARD_WAIT, DuelState);
		}
		else
		{
			pCard->ChangeState(CCardState::CARD_STAY, DuelState);
		}

	}

	//カード情報のクリア
	m_CastCardVector.clear();
	m_CastDiffenceCardVector.clear();
}