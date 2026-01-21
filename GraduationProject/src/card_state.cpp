//===========================================================================================================================================================
// 
// カードのステート [card_state.cpp]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#include "card_state.h"
#include "card.h"
#include "active_scene_manager.h"
#include "target_arrow.h"
#include "zone_manager.h"
#include "duel_manager.h"
#include "raknet.h"
#include "object2D_trianglefan.h"

//===========================================================================================================
// 
// カードステート基底クラス
// 
//===========================================================================================================

//=======================================================================================
// 終了処理
//=======================================================================================
void My::CCardState::Uninit()
{
	//delete m_pCardCpy;
	//m_pCardCpy = nullptr;
}

//=======================================================================================
// 更新処理
//=======================================================================================
void My::CCardState::Copy(CCard* /*cpy*/)
{
	//m_pCardCpy = cpy;
}


//===========================================================================================================
// 
// ニュートラルステート
// 
//===========================================================================================================

//=======================================================================================
// 初期化処理
//=======================================================================================
void My::CCardStateNeutral::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	D3DXVECTOR3 pos = cpy->GetPos();
	D3DXVECTOR3 npos = cpy->GetNeutralPos();

	cpy->SetPos(npos);
}

//=======================================================================================
// 更新処理
//=======================================================================================
void My::CCardStateNeutral::Update(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	cpy->SetSize({ 1.0f * 1.2f, 1.0f, 1.0f });
}


//===========================================================================================================
// 
// ピックアップステート
// 
//===========================================================================================================

//=======================================================================================
// 初期化処理
//=======================================================================================
void My::CCardStatePickup::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	// カードを見やすくする
	D3DXVECTOR3 pos = cpy->GetPos();
	//pos.y += 20.0f;
	pos.z += 10.0f;
	cpy->SetPos(pos);
}

//=======================================================================================
// 更新処理
//=======================================================================================
void My::CCardStatePickup::Update(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	cpy->SetSize({ 1.5f * 1.2f, 1.5f, 1.5f });

	//cpy->SetSize({ 1.2f * 1.2f,1.2f, 1.2f });
}

//===========================================================================================================
// 
// 選択ステート
// 
//===========================================================================================================

//=======================================================================================
// 初期化処理
//=======================================================================================
void My::CCardStateSelect::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	// カードを見やすくする
	D3DXVECTOR3 pos = cpy->GetPos();
	//pos.y += 20.0f;
	pos.z += 10.0f;
	cpy->SetPos(pos);
}

//=======================================================================================
// 更新処理
//=======================================================================================
void My::CCardStateSelect::Update(CCard* cpy, CDuelCharacter* /*duel*/)
{
	if (cpy == nullptr)
		return;

	cpy->SetSize({ 1.5f * 1.2f, 1.5f, 1.5f });

	//倍率
	float mag = 40.0f;	//元々30
	cpy->SetSize({ mag * 1.2f,mag,mag });

	//cpy->SetSize({ 1.2f * 1.2f,1.2f, 1.2f });
}


//===========================================================================================================
// 
// キャストステート(カードを使用)
// 
//===========================================================================================================

//=======================================================================================
// 初期化
//=======================================================================================
void My::CCardStateCast::Init(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;

	//倍率
	float mag = 1.0f;
	cpy->SetSize({ mag * 1.2f,mag,mag });

	//オフライン時の処理
	if (!CRakNet::GetInstance()->GetOnline())
	{
		//リストの取得
		std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

		CActiveSceneCharacter* pCharacter = My::CActiveSceneManager::GetInstance()->GetCharacter(cpy->GetUserId());

		// ステータスを取得
		CActiveSceneCharacter::Status status = pCharacter->GetStatus();

		// コスト分エナジーを減らす
		status.energy -= cpy->GetBaseStatus().nCost;

		// エナジーを設定
		pCharacter->SetStatus(status);

		// ゾーンマネージャーの取得
		CZoneManager* pZoneManager = dynamic_cast<CDuelCharacter*>(pCharacter->GetState())->GetZoneManager();
		// オーバーラップカードの格納変数
		COverlapCard* pOverlapCard = nullptr;

		// 登録
		for (auto& Target : cpy->GetTargetPlayerList())
		{
			pOverlapCard = pZoneManager->GetCastPreviewZone()->GetOverlapManager()->Create(duel, cpy);

			// 重ねたカードが存在していたら
			if (pOverlapCard)
			{
				pOverlapCard->Regist(cpy);
			}

			// ターゲットエリアが違うならスルー
			if (pCharacter->GetArea() != Target->GetArea()) { continue; }
		}
	}

	//カードのキャスト処理
	cpy->Cast(duel);
}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateCast::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;

	//倍率
	float mag = 10.0f;
	cpy->SetSize({ mag * 1.2f,mag,mag });

	//状態を変更
	ChangeToState(cpy, duel);
}

//=======================================================================================
// 状態を変更
//=======================================================================================
void My::CCardStateCast::ChangeToState(CCard* cpy, CDuelCharacter* duel)
{
	//守備カードじゃないならステイ状態
	if (cpy->GetBaseStatus().maintype != CCard::CARDTYPE_::TYPE_DEFFENCE)
	{
		cpy->ChangeState(CCardState::CARD_STATE::CARD_STAY, duel);
		return;
	}

	//ターゲットが自分なら守備待機状態へ
	for (auto& iter : cpy->GetTargetPlayerList())
	{
		if (iter->GetArea() == My::CActiveSceneManager::GetInstance()->GetPlayer()->GetArea())
		{
			cpy->ChangeState(CCardState::CARD_STATE::CARD_WAIT, duel);
			return;
		}
	}

	//キャスト先がエリアなら待機状態
	if (cpy->GetCastDestination() == My::CCard::CastDestination::AREA)
	{
		cpy->ChangeState(CCardState::CARD_STATE::CARD_WAIT, duel);
		return;
	}

	//ターゲットが敵ならステイ状態へ
	cpy->ChangeState(CCardState::CARD_STATE::CARD_STAY, duel);
}

//===========================================================================================================
// 
// ステイステート(カード効果発動待機)
// 
//===========================================================================================================

//無名空間
namespace
{
	const int FIRST_COUNT{ 3 };					//最初の数字
	const D3DXVECTOR2 COUNT_SIZE{50.0f, 50.0f};	//カウントのサイズ
}

//=======================================================================================
//コンストラクタ
//=======================================================================================
My::CCardStateStay::CCardStateStay() : 
	m_fStaycount(0.0f),			//ステイ時間
	m_pFan(nullptr)
{
	
}

//=======================================================================================
//デストラクタ
//=======================================================================================
My::CCardStateStay::~CCardStateStay()
{
	
}

//=======================================================================================
// 初期化
//=======================================================================================
void My::CCardStateStay::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	// カウントを初期化
	m_fStaycount = 0.0f;

	//位置の指定
	SetCardPos(cpy);

	cpy->Stay();

	//カウントを始めないか
	if (!cpy->IsCreatStayCount()) return;

	// ターゲットアローをマネージャーに登録
	for (auto& iter : cpy->GetTargetPlayerList())
	{
		CActiveSceneManager::GetInstance()->GetTargetArrowManager()->Regist(CTargetArrow::Create(cpy->GetUserArea(), iter->GetArea(), cpy), cpy);
	}

	m_pFan = new CObject2D_TriangleFan(20);
	m_pFan->SetPos(ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), cpy->GetPos()));
	m_pFan->Init();

}

//=======================================================================================
// カードの位置を設定
//=======================================================================================
void My::CCardStateStay::SetCardPos(CCard* cpy)
{
	//ディフェンスカードはカウントダウンを始めない
	if (cpy->GetBaseStatus().maintype == CCard::TYPE_DEFFENCE) return;


	// TODO : 
	// ユーザーエリアとターゲットエリアの中間地点に
	// カードの位置を設定する
	cpy->GetUserArea();

}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateStay::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;

	//倍率
	float mag = 5.0f; static float a = 0;
	cpy->SetSize({ mag * 1.2f,mag,mag });

	//ディフェンスカードはカウントダウンを始めない
	if (!cpy->IsCreatStayCount()) return;
	
	//オフライン時の処理
	if (!CRakNet::GetInstance()->GetOnline())
	{
		if (m_fStaycount >= 3.0f)
		{// カウントが設定された時間を超えたら

			// トリガー状態にする
			cpy->ChangeState(CCardState::CARD_STATE::CARD_TRIGGER, duel);

			return;
		}
	}

	// カウントを進める
	if (CRakNet::GetInstance()->GetOnline())
	{
		std::chrono::duration<float>(My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime()).count();
		m_fStaycount += My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime()/* * 0.001f*/;
	}
	else
	{
		m_fStaycount += 0.016f;
	}

	m_pFan->SetStayTime(m_fStaycount);
}

//===========================================================================================================
// 
// 守備ステイステート(守備カード効果発動待機)
// 
//===========================================================================================================

//=======================================================================================
// 初期化
//=======================================================================================
void My::CCardStateWait::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	//試しに画面外に飛ばす
	cpy->SetPos({ -1000.0f, 0.0f, 0.0f });
}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateWait::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;
}

//===========================================================================================================
// 
// トリガーステート(カード効果発動)
// 
//===========================================================================================================

//=======================================================================================
// 初期化
//=======================================================================================
void My::CCardStateTrigger::Init(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;

	//リストの取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	////キャラクターの周回
	//for (auto& iter : List)
	//{
	//	//カードの使用者と異なるなら次へ
	//	if (cpy->GetUserArea() != iter->GetArea()) continue;

	//	//トリガーしたか
	//	if (CActiveSceneManager::GetInstance()->GetAreaManager()->CardTrigger(cpy->GetTarget()))
	//	{
	//		//トリガー処理
	//		//cpy->GetCardStrategy()->Strategy(duel, cpy);

	//		//TODO:一旦計算後のストラテジーベクターだけ処理
	//		std::vector<CCardStrategy_Base*> strategy_vector = cpy->GetPostCalculateVector();

	//		for (auto& itr : strategy_vector)
	//		{
	//			if (itr == nullptr) { continue; }
	//			itr->Strategy(duel, cpy);
	//		}
	//	}

	//	break;
	//}

	//周回
	//for (auto& iter : cpy->GetTargetPlayerList())
	//{
	//	//トリガーしたか
	//	if (CActiveSceneManager::GetInstance()->GetAreaManager()->CardTrigger(iter->GetArea()))
	//	{
	//		//トリガー処理
	//		//cpy->GetCardStrategy()->Strategy(duel, cpy);

	//		//TODO:一旦計算後のストラテジーベクターだけ処理
	//		std::vector<CCardStrategy_Base*> strategy_vector = cpy->GetPostCalculateVector();

	//		for (auto& itr : strategy_vector)
	//		{
	//			if (itr == nullptr) { continue; }
	//			itr->Strategy(duel, cpy);
	//		}
	//	}

	//	//break;
	//}

	//TODO:一旦計算後のストラテジーベクターだけ処理
	std::vector<CCardStrategy_Base*> strategy_vector = cpy->GetPostCalculateVector();

	for (auto& itr : strategy_vector)
	{
		if (itr == nullptr) { continue; }
		itr->Strategy(duel, cpy);
	}

	CActiveSceneManager::GetInstance()->GetTargetArrowManager()->Remove();
	CActiveSceneManager::GetInstance()->GetTargetArrowManager()->CardRemove(cpy);

	//設定
	cpy->SetSize({ 0.0f * 1.2f, 0.0f, 0.0f });

	//リストの取得
	//std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	//ディフェンスカードはカウントダウンを始めない
	if (cpy->GetBaseStatus().maintype == CCard::TYPE_DEFFENCE) return;

	//キャラクターの周回
	for (auto& iter : List)
	{
		if (cpy->GetUserArea() != iter->GetArea()) continue;

		//// TODO : Init から Update に移設(この処理は重いのか？)
		//// トリガーされた際に消去する(TODO : 使用者によって変更)
		//iter->GetTargetArrowManeger()->Remove();

		break;
	}

	duel->GetZoneManager()->MoveZone(cpy, cpy->CastToZone(cpy->GetCurrentZone(), duel), duel->GetZoneManager()->GetCemetery(), true);

	cpy->SetCurrentZone(CCard::CEMETERY);
}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateTrigger::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;
}
//===========================================================================================================
// 
// セメタリーステート(カード効果発動)
// 
//===========================================================================================================
void My::CCardStateCemetery::Init(CCard* cpy, CDuelCharacter* duel)
{
	//対象のリストを削除
	cpy->RemoveAllTargetList();
}