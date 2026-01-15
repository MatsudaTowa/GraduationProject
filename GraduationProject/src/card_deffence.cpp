//===========================================================================================================================================================
// 
// 守備カードの処理 [card_deffence.cpp]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
//ヘッダーのインクルード
#include "card_deffence.h"
#include "card_strategy.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "zone_manager.h"
#include "card_attack.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardDeffence::CCardDeffence(int nPriority):CCard(nPriority),
m_DefenceType(),
m_nDefenceValue(INT_ZERO),
m_nCounterValue(INT_ZERO),
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
	SetCardType(CCard::CARDTYPE_::TYPE_DEFFENCE);
	CCard::Init();

	//トリガー時のストラテジーを代入
	//SetCardStrategy(new CDiffence);

	std::vector<CCardStrategy_Base*> strategy_vector = GetPostCalculateVector();
	strategy_vector.push_back(new CDiffence);
	SetpostCalculateStrategyVector(strategy_vector);
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
// 描画処理
//===========================================================================================================
void My::CCardDeffence::Draw()
{
	CCard::Draw();
}

//===========================================================================================================
// カード情報読み込み
//===========================================================================================================
void My::CCardDeffence::LoadCardData()
{
	//TODO:各ステータス読み込みしてから親クラスの読み込み
	CCard::LoadCardData();
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
//キャストをしたかの確認
//===========================================================================================================
bool My::CCardDeffence::IsCast(CDuelCharacter* duel, CInputMouse::AREA area)
{
	//プレイヤーが対象かの確認
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player->GetArea() == area)
	{
		//自身の対象に追加
		DiffenceTarget Target;
		Target.nAttackCardUserId = player->GetPlayerIdx();
		Target.nTargetCardId = 0;
		Target.nTargetCardSameId = 0;
		m_TargetInfo.push_back(Target);
		RegistTargetList(player);

		//キャスト先の代入
		SetCastDestination(AREA);	//エリア

		return true;
	}

	//キャストしたか
	bool isCast = false;

	// カメラ取得
	CCamera* pCamera = GET_CAMERA(0);
	// マウス取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	// マウス座標
	D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };

	//敵が対象かの確認
	std::list<CEnemy*> enemy_list = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();
	for (auto& itr : enemy_list)
	{
		if (itr == nullptr) { continue; }
		
		//対象の状態を取得
		//if (typeid(CDuelCharacter) != typeid(*itr->GetState())) continue;		//中身の一致を確認
		CDuelCharacter* State = dynamic_cast<CDuelCharacter*>(itr->GetState());	//キャスト
		if (State == nullptr) continue;											//中身があるか確認

		//TODO::現在は先に出した攻撃カードを参照するが、将来的には相手のステイ中のカードから選択する
		if (State->GetZoneManager()->GetCastPreviewZone()->GetList().empty()) continue;

		//リストの周回カウント
		int nListCount = 0;

		//キャストゾーンのカード周回
		for (auto& iter : State->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//攻撃のカードが存在するかを確認
			if (iter->GetCardType() != CCard::CARDTYPE_::TYPE_ATTACK)
			{
				++nListCount;
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
			if (resultpos.x <= 50.0f * GetSize().x && resultpos.x >= -50.0f * GetSize().x &&
				resultpos.y <= 100.0f * GetSize().y && resultpos.y >= -100.0f * GetSize().y)
			{
				RegistTargetList(itr);
				isCast = true;

				//攻撃カードにキャストして自身を追加する
				CCardAttack* pAttackCard = dynamic_cast<CCardAttack*>(iter);
				if (pAttackCard == nullptr)
				{
					++nListCount;
					continue;
				}
				
				//ベクターに追加
				pAttackCard->AddDefCard(this);

				//ステイフラグ
				m_isStay = true;

				//自身の対象に追加
				DiffenceTarget Target;
				Target.nAttackCardUserId = itr->GetPlayerIdx();
				Target.nTargetCardId = iter->GetBaseStatus().nCardID;
				Target.nTargetCardSameId = iter->GetSameTypeId();
				m_TargetInfo.push_back(Target);

				//キャスト先の代入
				SetCastDestination(CARD);	//カード
			}

			++nListCount;
		}
	}

	//キャストしたかを返す
	return isCast;
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardDeffence::Cast(CDuelCharacter* duel)
{
	
}

//===========================================================================================================
//ステイ処理
//===========================================================================================================
void My::CCardDeffence::Stay()
{
	//対象のカード
	CCard* pCard = nullptr;

	//ターゲット周回
	for (auto& iter : m_TargetInfo)
	{
		for (auto& Character : My::CActiveSceneManager::GetInstance()->GetCharacterList())
		{
			if (iter.nAttackCardUserId != Character->GetPlayerIdx()) continue;

			CDuelCharacter* DuelState = nullptr;

			//対戦状態にキャスト
			DuelState = dynamic_cast<CDuelCharacter*>(Character->GetState());

			if (DuelState == nullptr) continue;	//キャスト成功したかの確認

			int nCount = 0;

			//手札のカードを周回し、受信したカードを探す
			for (auto& Card : DuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
			{
				if (iter.nTargetCardId != Card->GetBaseStatus().nCardID) continue;
				if (iter.nTargetCardSameId != Card->GetSameTypeId()) continue;

				/*if (nCount != iter.nTargetCard)
				{
					++nCount;
					continue;
				}*/

				pCard = Card;
				break;
			}

			if (!pCard) return;

			//カード
			D3DXVECTOR3 pos = pCard->GetPos();
			SetPos({ pos.x + STEY_SPACE, pos.y, pos.z});
			
		}
	}
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

	//ターゲットの周回
	for (auto& iter : GetTargetPlayerList())
	{
		//ダメージがあるなら与える
		if (m_nCounterValue > 0)
		{
			iter->ReceiveDamage(m_nCounterValue);
		}
	}
}

//===========================================================================================================
// カードをマウスでキャスト
//===========================================================================================================
bool My::CCardDeffence::CardCastToMouse(CDuelCharacter* duel, CActiveSceneCharacter* player)
{
	// 選択状態とキャスト状態以外は通さない
	if (GetStateNum() != CCardState::CARD_PICKUP &&
		GetStateNum() != CCardState::CARD_CAST &&
		GetStateNum() != CCardState::CARD_SELECT)
	{
		return false;
	}

	// カメラ取得
	//CCamera* pCamera = GET_CAMERA(0);
	// マウス取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;
	// カード座標
	D3DXVECTOR3 pos = GetPos();

	// カード座標をスクリーン座標変換する
	D3DXVECTOR3 screenpos;
	//screenpos = ConvertToScreenPos(pCamera, pos);
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//クライアントサイズを取得
	RECT rc;
	GetClientRect(GET_HWND, &rc);
	float width = (float)(rc.right - rc.left);
	float height = (float)(rc.bottom - rc.top);

	D3DXMATRIX View, Proj;
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);	// ビュー行列の取得
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);	// 投影行列の取得

	D3DXVECTOR3 resultpos = { 0,0,0 };

	//リストのクリア
	m_TargetInfo.clear();

	if (pMouse->GetPress(0))
	{
		// ロビーじゃなかったら抜ける
		if (typeid(*duel) == typeid(CPlayerDuelState))
		{
			CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(duel);
			//duel_state->GetHand()->GetSelectionRange()->SetisDraw(false);
		}
		// キャストステートにする
		ChangeState(CCardState::CARD_SELECT, duel);
		CActiveSceneManager::GetInstance()->ChangeState(new CCardCast);

		screenpos = pMouse->GetMousePos();
		CalcScreenToWorld(&resultpos, screenpos.x, screenpos.y, 1.0f, static_cast<int>(width), static_cast<int>(height), &View, &Proj);
		resultpos.y += 20.0f;
		SetPos(resultpos);

		return true;
	}
	else if (pMouse->GetRelease(0))
	{
		// ロビーじゃなかったら抜ける
		if (typeid(*duel) == typeid(CPlayerDuelState))
		{
			CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(duel);
			//キャンセルエリアなら解除
			if (GET_COLISION->Check2DPolygonColision(GET_INPUT_MOUSE->GetMousePos(), { 3.0f,3.0f }, { duel_state->GetHand()->GetSelectionRange()->GetPos() }, duel_state->GetHand()->GetSelectionRange()->GetSize()))
			{
				//通常状態にする
				ChangeState(CCardState::CARD_NEUTRAL, duel);
				CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
				return false;
			}
		}
		//対象のエリア
		CInputMouse::AREA Area;
		Area = pMouse->GetArea();

		//エナジーが不足しているなら抜ける
		if (player->GetEnergy() < GetBaseStatus().nCost)
		{
			// 通常状態にする
			ChangeState(CCardState::CARD_NEUTRAL, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
			return false;
		}

		//使用者のエリアの取得
		SetUserArea(player->GetArea());

		if (IsCast(duel, Area))
		{
			//ステイフラグの確認
			//if (m_isStay)
			//{
			//	//ステイステートにする
			//	ChangeState(CCardState::CARD_STAY, duel);
			//}
			//else
			//{
			//	// キャスト遷移
			//	ChangeState(CCardState::CARD_CAST, duel);
			//}
			
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);

			//オンライン時なら送信
			if (CRakNet::GetInstance()->GetOnline())
			{//TODO : カードの対象が複数になったら処理の変更の必要があり
				//std::list<CActiveSceneCharacter*> list = CActiveSceneManager::GetInstance()->GetCharacterList();

				////対象を見つけて送信
				//for (auto& itr : list)
				//{
				//	if (itr == nullptr) { continue; }
				//	if (itr->GetArea() != Area) { continue; }
				//	if (m_TargetInfo.empty()) { continue; }

				//	CRakNet::GetInstance()->SendCastDefCard(GetBaseStatus().nCardID, CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx(), m_TargetInfo);

				//	break;
				//}

				//キャストのリクエストを送信
				CRakNet::GetInstance()->RequestCastCard(this);
			}
			else
			{
				// キャスト遷移
				ChangeState(CCardState::CARD_CAST, duel);
			}

			return false;
		}
		else
		{
			//通常状態にする
			ChangeState(CCardState::CARD_NEUTRAL, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
			return true;
		}
	}

	return false;

}

//===========================================================================================================
//カード情報の送信
//===========================================================================================================
void My::CCardDeffence::SendCardInfo(RakNet::BitStream* bsout)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合

		break;

	case CastDestination::CARD:	//カードの場合

		//重ねたカードの書き出し
		bsout->Write(m_TargetInfo[0].nAttackCardUserId);	//使用者番号
		bsout->Write(m_TargetInfo[0].nTargetCardId);		//カードID
		bsout->Write(m_TargetInfo[0].nTargetCardSameId);	//同種類番号

		break;
	default:
		break;
	}
}

//===========================================================================================================
//カード情報読み込み
//===========================================================================================================
void My::CCardDeffence::LoadCardInfo(RakNet::BitStream* bsin)
{
	//列挙に応じて処理を送信内容を変更
	switch (GetCastDestination())
	{
	case CastDestination::AREA:	//エリアの場合
	{
		My::CActiveSceneCharacter* pCharacter = My::CActiveSceneManager::GetInstance()->GetCharacter(GetUserId());
		My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pCharacter->GetState());
		//ChangeState(My::CCardState::CARD_CAST, pDuelState);
	}
		
		break;

	case CastDestination::CARD:	//カードの場合

		////重ね先のカード情報を送信
		//bsin->Read(m_TargetInfo[0].nAttackCardUserId);	//対象カードの使用者番号
		//bsin->Read(m_TargetInfo[0].nTargetCardId);		//カード番号
		//bsin->Read(m_TargetInfo[0].nTargetCardSameId);	//同種番号

	{
		//変数宣言
		int nUserId = 0;	//使用者番号
		int nCardId = 0;	//カード番号
		int nSameId = 0;	//同種番号
		My::CActiveSceneCharacter* pCharacter = nullptr;

		//読み込み
		bsin->Read(nUserId);
		bsin->Read(nCardId);
		bsin->Read(nSameId);

		//番号が一致するプレイヤーの取得
		pCharacter = My::CActiveSceneManager::GetInstance()->GetCharacter(nUserId);

		if (!pCharacter) return;	//中身の確認

		//対象を代入用の攻撃カードポインタ
		My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pCharacter->GetState());
		My::CCardAttack* pAttackCard = nullptr;

		//相手のキャストゾーンのカードを確認
		for (My::CCard* pCard : pDuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//受信した対象のカードがあるかを確認
			if (pCard->GetBaseStatus().maintype != CCard_Client::ATTACK) continue;	//攻撃カードか
			if (pCard->GetBaseStatus().nCardID != nCardId) continue;				//カード番号
			if (pCard->GetSameTypeId() != nSameId) continue;						//同種番号

			//攻撃カードにキャストして
			pAttackCard = dynamic_cast<My::CCardAttack*>(pCard);
			break;
		}

		//攻撃カードの中身を確認
		if (!pAttackCard) return;

		//攻撃カードに自身を登録
		pAttackCard->AddDefCard(this);

		//ターゲット情報の代入
		DiffenceTarget Target = { pAttackCard->GetUserId(), pAttackCard->GetBaseStatus().nCardID, pAttackCard->GetSameTypeId() };
		m_TargetInfo.push_back(Target);
		ChangeState(My::CCardState::CARD_STAY, pDuelState);
	}

		break;

	default:
		break;
	}
}