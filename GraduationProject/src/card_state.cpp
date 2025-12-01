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
#include "duel_manager.h"

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

	//CActiveSceneManager::GetInstance()->ChangeState(new CCardCast);

	// 今だけわかりやすく位置を変える
	//D3DXVECTOR3 pos = cpy->GetPos();
	//pos.z += 20.0f;

	//cpy->SetPos(pos);

	//リストの取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	//キャラクターの周回
	for (auto& iter : List)
	{
		if (cpy->GetUserArea() != iter->GetArea()) continue;

		// ステータスを取得
		CActiveSceneCharacter::Status status = iter->GetStatus();

		// コスト分エナジーを減らす
		status.energy -= cpy->GetBaseStatus().nCost;

		// エナジーを設定
		iter->SetStatus(status);

		break;
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

	//cpy->ChangeState(CCardState::CARD_STATE::CARD_STAY, duel);

	//cpy->ChangeState(CCardState::CARD_STATE::CARD_STAY);
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
	if (cpy->GetTarget() == cpy->GetUserArea())
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
	m_pNumber(nullptr),			//数字表示用
	m_nCount(),					//カウント
	m_nDrawNum(FIRST_COUNT)		//表示する番号
{
	
}

//=======================================================================================
// 初期化
//=======================================================================================
void My::CCardStateStay::Init(CCard* cpy, CDuelCharacter* /*duel*/)
{
	// カウントを初期化
	m_Staycount = 0;

	//位置の指定
	SetCardPos(cpy);

	cpy->Stay();

	//ディフェンスカードはカウントダウンを始めない
	if (cpy->GetBaseStatus().maintype == CCard::TYPE_DEFFENCE) return;

	//数字の設定
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), cpy->GetPos()); //スクリーン座標に変換
	m_pNumber = m_pNumber->Create(screen_pos, COUNT_SIZE, 0);
	m_pNumber->SetNumber(m_nDrawNum * 0.1f, (m_nDrawNum + 1) * 0.1f, COLOR_WHITE);

	//リストの取得
	std::list<CActiveSceneCharacter*> List = CActiveSceneManager::GetInstance()->GetCharacterList();

	//キャラクターの周回
	for (auto& iter : List)
	{
		if (cpy->GetUserArea() != iter->GetArea()) continue;

		//// ターゲットアローをマネージャーに登録
		//iter->GetTargetArrowManeger()->Regist(CTargetArrow::Create(cpy->GetUserArea(), cpy->GetTarget()));

		

		break;
	}

	CActiveSceneManager::GetInstance()->GetTargetArrowManager()->Regist(CTargetArrow::Create(cpy->GetUserArea(), cpy->GetTarget()));
}

//=======================================================================================
// カードの位置を設定
//=======================================================================================
void My::CCardStateStay::SetCardPos(CCard* cpy)
{
	//ディフェンスカードはカウントダウンを始めない
	if (cpy->GetBaseStatus().maintype == CCard::TYPE_DEFFENCE) return;

	cpy->SetPos({ 0.0f, 0.0f, 0.0f });
	
	//エリアによって位置を変える
	switch (cpy->GetUserArea())
	{
	case My::CInputMouse::DOWN:	//下
		cpy->SetPos({ 0.0f, -100.0f, -150.0f });
		break;

	case My::CInputMouse::RIGHT://右
		cpy->SetPos({ 200.0f, -100.0f, 50.0f });
		break;

	case My::CInputMouse::LEFT:	//左
		cpy->SetPos({ 200.0f, -100.0f, 50.0f });
		break;

	case My::CInputMouse::UP:	//上
		cpy->SetPos({ 0.0f, -100.0f, 250.0f });
		break;
	}
}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateStay::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;

	//倍率
	float mag = 5.0f;
	cpy->SetSize({ mag * 1.2f,mag,mag });

	//ディフェンスカードはカウントダウンを始めない
	if (cpy->GetBaseStatus().maintype == CCard::TYPE_DEFFENCE) return;

	if (m_Staycount >= STAY_TIME)
	{// カウントが設定された時間を超えたら

		//オブジェクトの破棄
		if (m_pNumber != nullptr)
		{
			m_pNumber->Uninit();
			m_pNumber = nullptr;
		}

		// トリガー状態にする
		cpy->ChangeState(CCardState::CARD_STATE::CARD_TRIGGER, duel);
		
		return;
	}

	// カウントを進める
	m_Staycount++;

	//カウントダウン処理
	CountDown();
}

//=======================================================================================
// カウントダウン処理
//=======================================================================================
void My::CCardStateStay::CountDown()
{
	++m_nCount;

	//数字の表記
	if (m_nCount > 60)
	{
		//数値のリセット
		m_nCount = 0;

		//描画する数値を減らす
		--m_nDrawNum;

		//数字の設定
		m_pNumber->SetNumber(m_nDrawNum * 0.1f, (m_nDrawNum + 1) * 0.1f, COLOR_WHITE);
	}
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

	//キャラクターの周回
	for (auto& iter : List)
	{
		if (cpy->GetUserArea() != iter->GetArea()) continue;

		//トリガーしたか
		if (CActiveSceneManager::GetInstance()->GetAreaManager()->CardTrigger(cpy->GetTarget()))
		{
			//トリガー処理
			cpy->GetCardStrategy()->Strategy(duel, cpy);
		}

		//// トリガーされた際に消去する(TODO : 使用者によって変更)
		//iter->GetTargetArrowManeger()->Remove();

		break;
	}

	CActiveSceneManager::GetInstance()->GetTargetArrowManager()->Remove();

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

	//墓地状態にする
	//cpy->ChangeState(CCardState::CARD_STATE::CARD_CEMETERY, duel);
}

//=======================================================================================
// 更新
//=======================================================================================
void My::CCardStateTrigger::Update(CCard* cpy, CDuelCharacter* duel)
{
	if (cpy == nullptr)
		return;
}