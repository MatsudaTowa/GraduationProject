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
#include "total_damage_UI.h"

My::CCardAttack::CCardAttack(int nPriority):CCard(nPriority),
m_AttackType(),
m_nAttackValue(INT_ZERO),
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
	SetCardType(CCard::CARDTYPE_::TYPE_ATTACK);
	CCard::Init();

	//トリガー時のストラテジーを代入
	//SetCardStrategy(new CAttack);

	std::vector<CCardStrategy_Base*> strategy_vector = GetPostCalculateVector();
	strategy_vector.push_back(new CAttack);

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
	if (GetTotalDamageUI() != nullptr)
	{
		// ダメージUIの位置更新
		D3DXVECTOR3 pos = GetPos();
		pos.y += 100.0f;
		GetTotalDamageUI()->SetPos(ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), pos));
	}
	
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
	//キャストしたか
	bool isCast = false;

	// カメラ取得
	CCamera* pCamera = GET_CAMERA(0);
	// マウス取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	// マウス座標
	D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };

	//敵が対象かの確認
	CActiveScenePlayer* pPlayer = CActiveSceneManager::GetInstance()->GetPlayer();

	if (pPlayer == nullptr) return true;

	//対象の状態を取得
	//if (typeid(CDuelCharacter) != typeid(*itr->GetState())) continue;			//中身の一致を確認
	CDuelCharacter* State = dynamic_cast<CDuelCharacter*>(pPlayer->GetState());	//キャスト
	if (State == nullptr) return true;											//中身があるか確認

	//TODO::現在は先に出した攻撃カードを参照するが、将来的には相手のステイ中のカードから選択する
	if (!State->GetZoneManager()->GetCastPreviewZone()->GetList().empty())
	{
		//キャストゾーンのカード周回
		for (auto& iter : State->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//攻撃のカードが存在するかを確認
			if (iter->GetCardType() != CCard::CARDTYPE_::TYPE_ATTACK)
			{
				continue;
			}

			// カード座標
			D3DXVECTOR3 pos = iter->GetPos();

			// カードの座標をスクリーン座標変換した座標を格納する変数
			D3DXVECTOR3 screenpos;

			// カード座標をスクリーン座標変換する
			screenpos = ConvertToScreenPos(pCamera, pos);

			// マウスとカードの位置の差
			D3DXVECTOR2 resultpos;
			resultpos.x = mousepos.x - screenpos.x;
			resultpos.y = mousepos.y - screenpos.y;

			// 矩形判定
			if (resultpos.x <= /*50.0f * */GetSize().x && resultpos.x >= -/*50.0f * */GetSize().x &&
				resultpos.y <= /*100.0f * */GetSize().y && resultpos.y >= -/*100.0f * */GetSize().y)
			{
				//攻撃カードにキャストして自身を追加する
				CCardAttack* pAttackCard = dynamic_cast<CCardAttack*>(iter);
				if (pAttackCard == nullptr)
				{
					continue;
				}

				if (!pAttackCard->GetTopCastCard()) continue;

				//キャストの確認
				isCast = true;

				if (!CRakNet::GetInstance()->GetOnline())
				{
					pAttackCard->AddStackCards(this);	//自身を追加
				}

				m_pStackCard = pAttackCard;			//重ね先の登録

				//キャスト先の代入
				SetCastDestination(CARD);	//カード

				return true;
			}
		}
	}

	//キャスト先の代入
	SetCastDestination(AREA);	//エリア

	//キャストしたかを返す
	return true;
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardAttack::Cast(CDuelCharacter* duel)
{
	//ターゲットの周回
	for (CActiveSceneCharacter* TargetCharacter : GetTargetPlayerList())
	{
		//デュエル状態にキャスト
		CDuelCharacter* DuelState = dynamic_cast<CDuelCharacter*>(TargetCharacter->GetState());	//キャスト
		if (DuelState == nullptr) break;														//中身の確認

		//待機状態のカードを確認
		if (!DuelState->GetZoneManager()->GetWaitZone()->GetList().empty())
		{
			CCardDeffence* pCard = dynamic_cast<CCardDeffence*>(DuelState->GetZoneManager()->GetWaitZone()->GetList().front());	//先頭のカードの確認
			if (pCard == nullptr) break;

			//守備カードに自身のエリアを追加し、ステイ状態にする
			//pCard->SetTarget(GetUserArea());
			pCard->RegistTargetList(My::CActiveSceneManager::GetInstance()->GetCharacter(GetUserId()));

			//守備対象のターゲットの作成
			CCardDeffence::DiffenceTarget Target = { 0, 0, 0 };

			//キャラクターの周回
			for (auto character : My::CActiveSceneManager::GetInstance()->GetCharacterList())
			{
				//同じエリアのみ通す
				if (character->GetArea() != GetUserArea()) continue;

				Target.nAttackCardUserId = character->GetPlayerIdx();	//使用者の番号を伝える

				break;
			}

			//パラメータを代入
			Target.nTargetCardId = GetBaseStatus().nCardID;
			Target.nTargetCardSameId = GetSameTypeId();
			Target.nAttackCardUserId = GetUserId();
			pCard->SetDiffenceTarget(Target);

			pCard->ChangeState(CCardState::CARD_STAY, DuelState);
			pCard->SetisDraw(true);

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
	//1番上のカードでは無いなら画面外に飛ばす
	if (!m_isTopCastCard)
	{
		SetPos({ -1000.0f, 0.0f, 0.0f });
	}

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
	if (CRakNet::GetInstance()->GetOnline()) return;

	//ステイ後に起動
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();
	
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

	//重なっているカードを墓地に送る
	for (auto& iter : m_StackedCardsList)
	{
		CDuelCharacter* duel = dynamic_cast<CDuelCharacter*>(My::CActiveSceneManager::GetInstance()->GetCharacter(iter->GetUserId())->GetState());
		duel->GetZoneManager()->MoveZone(iter, iter->CastToZone(iter->GetCurrentZone(), duel), duel->GetZoneManager()->GetCemetery(), true);

		iter->SetCurrentZone(CCard::CEMETERY);
	}

	//カードのクリア
	m_DefCardVector.clear();
}

//===========================================================================================================
//トリガー受信時の処理
//===========================================================================================================
void My::CCardAttack::ReceiveTrigger(RakNet::BitStream* bsin)
{
	//ダメージを与えたか受信
	bool isDamage = false;
	bsin->Read(isDamage);

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
		iter->SetCurrentZone(CCard::CEMETERY);
	}

	//カードのクリア
	m_DefCardVector.clear();

	for (auto& iter : m_StackedCardsList)
	{
		//使用者を見つけゾーンの位置を変更
		CActiveSceneCharacter* pUsedPlayer = CActiveSceneManager::GetInstance()->GetCharacter(iter->GetUserId());
		My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pUsedPlayer->GetState());

		//キャスト失敗時にアサート
		if (!pDuelState) assert(false);

		//状態とゾーンの変更
		iter->ChangeState(CCardState::CARD_CEMETERY, pDuelState);
		iter->SetCurrentZone(CCard::CEMETERY);
	}

	// ダメージUIの終了
	GetTotalDamageUI()->Uninit();

	//カードクリア
	m_StackedCardsList.clear();

	//親の処理
	My::CCard::ReceiveTrigger(bsin);
}

//===========================================================================================================
//カード情報の送信
//===========================================================================================================
void My::CCardAttack::SendCardInfo(RakNet::BitStream* bsout)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合

		//ターゲットの書き出し
		bsout->Write((int)GetTargetPlayerList().size());

		//ターゲットの番号
		for (auto iter : GetTargetPlayerList())
		{
			bsout->Write(iter->GetPlayerIdx());
		}

		break;

	case CastDestination::CARD:	//カードの場合

		//重ねたカードの書き出し
		bsout->Write(m_pStackCard->GetBaseStatus().nCardID);	//カードID
		bsout->Write(m_pStackCard->GetSameTypeId());			//同種類番号

		//読み込みがあるため中身をnullにする
		m_pStackCard = nullptr;

		break;
	default:
		break;
	}
}

//===========================================================================================================
//ターゲットのリストの追加
//===========================================================================================================
void My::CCardAttack::AddTargetPlayerList(CActiveSceneCharacter* character)
{
	//ダメージ情報に追加
	m_DamageInfo[character->GetPlayerIdx()] = m_nAttackValue;

	//親の呼び出し
	My::CCard::AddTargetPlayerList(character);
}

//===========================================================================================================
//カード情報読み込み
//===========================================================================================================
void My::CCardAttack::LoadCardInfo(RakNet::BitStream* bsin)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合

	{
		//変数宣言
		int nTargetNum = 0;

		//送信時にpushbackしているため送信時にプッシュバックを行わずに送信する処理が必要
		m_DamageInfo.clear();
		m_StackedCardsList.clear();

		//ターゲットの数を読み込み
		bsin->Read(nTargetNum);

		//ターゲットの番号を読み込み
		for (int i = 0; i < nTargetNum; i++)
		{
			int Id = 0;
			bsin->Read(Id);
			AddTargetPlayerList(My::CActiveSceneManager::GetInstance()->GetCharacter(Id));
		}

		//一番上のフラグを立てる
		m_isTopCastCard = true;

		if (GetTotalDamageUI() == nullptr)
		{
			//ダメージUIの生成
			SetTotalDamageUI(CTotalDamageUI::Create(ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()), m_nAttackValue));
		}
		
	}

	break;

	case CastDestination::CARD:	//カードの場合

	{
		//読み込むデータ
		int nCardId = 0;		//カードID
		int nSameTypeId = 0;	//同種番号

		//読み込み
		bsin->Read(nCardId);
		bsin->Read(nSameTypeId);

		//キャラクターのポインタ
		My::CActiveSceneCharacter* pCharacter = My::CActiveSceneManager::GetInstance()->GetCharacter(GetUserId());
		My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pCharacter->GetState());
		My::CCardAttack* StackAttackCard = nullptr;															//重ね先のカード

		//キャストカードの確認
		for (My::CCard* pCard : pDuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//一致の確認
			if (pCard->GetBaseStatus().nCardID != nCardId) continue;				//カード番号
			if (pCard->GetSameTypeId() != nSameTypeId) continue;					//同種番号
			if (pCard->GetBaseStatus().maintype != CCard_Client::ATTACK) continue;	//攻撃カードか

			StackAttackCard = dynamic_cast<My::CCardAttack*>(pCard);	//攻撃カードにキャスト
			break;
		}

		if (!StackAttackCard) return;	//中身の確認

		//重ねたフラグ
		m_isTopCastCard = false;				//一番上のフラグを下す
		StackAttackCard->AddStackCards(this);	//重ね先のカードの重ねカードに自身を追加
		m_pStackCard = StackAttackCard;			//重ね先の登録

		//ステイ時間の初期化
		CCardStateStay* StayState = dynamic_cast<CCardStateStay*>(GetState());
		if (StayState) { StayState->ResetStayTime(); }

		//キャスト状態に変更(NOTE : 重ねているカードはキャストしなくても良いかも)
		//ChangeState(My::CCardState::CARD_CAST, pDuelState);

		// 攻撃カードが重なったときの合計攻撃力表示更新
		if (StackAttackCard->GetTotalDamageUI() != nullptr)
		{
			StackAttackCard->GetTotalDamageUI()->SetTexPos({ (StackAttackCard->GetAttackValue() + m_nAttackValue) * 0.1f,1.0f });
		}
		
		break;
	}

	default:
		break;
	}

	//引数の番号のプレイヤーを取得
	My::CActiveSceneCharacter* Character = My::CActiveSceneManager::GetInstance()->GetCharacter(GetUserId());

	//対戦状態にキャスト
	My::CDuelCharacter* pState = dynamic_cast<My::CDuelCharacter*>(Character->GetState());

	//手札のカードを手札に移動
	ChangeState(My::CCardState::CARD_CAST, pState);
}

//===========================================================================================================
//カード情報読み込み
//===========================================================================================================
bool My::CCardAttack::IsSetUpStay()
{
	return m_isTopCastCard;
}
