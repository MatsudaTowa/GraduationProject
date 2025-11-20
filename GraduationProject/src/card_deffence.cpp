#include "card_deffence.h"
#include "card_strategy.h"
#include "active_scene_manager.h"
#include "raknet.h"

My::CCardDeffence::CCardDeffence(int nPriority):CCard(nPriority),
m_DefenceType(),
m_nDefenceValue(INT_ZERO),
m_nCounterValue(INT_ZERO)
{
}

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
	SetCardStrategy(new CDiffence);
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
	m_DefenceType = (DefenseType)param.Attacktype;	//防御の種類
	m_nCounterValue = param.nCounter;				//カウンター値
	m_nDefenceValue = param.nGuard;					//防御値
}

//===========================================================================================================
//キャスト処理
//===========================================================================================================
void My::CCardDeffence::Cast(CDuelCharacter* duel)
{
	//待機ゾーンに追加
	//マウスの取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	//ターゲットの生成
	My::CInputMouse::AREA Target;

	//ステータスの取得
	BaseStatus Status = GetBaseStatus();

	//ステイゾーンに追加
	Target = pMouse->GetArea();

	// ステイ遷移
	ChangeState(CCardState::CARD_STAY, duel);
	CActiveSceneManager::GetInstance()->ChangeState(new CDuel);

	//プレイヤーの取得
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player->GetArea() == Target)
	{
		RegistTargetList(player);
	}

	std::list<CEnemy*> enemy_list = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();
	for (auto& itr : enemy_list)
	{
		if (itr == nullptr) { continue; }
		if (itr->GetArea() != Target) { continue; }

		//デュエル状態か確認
		if (typeid(CDuelCharacter) != typeid(*itr->GetState())) return;

		//状態を取得
		CDuelCharacter* state = dynamic_cast<CDuelCharacter*>(itr->GetState());

		if (state == nullptr) return;	//中身が無いなら抜ける

		RegistTargetList(itr);

		//オンライン時なら送信
		if (CRakNet::GetInstance()->GetOnline())
		{//TODO : カードの対象が複数になったら処理の変更の必要があり
			CRakNet::GetInstance()->SendCastCard(Status.nCardID, CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx(), itr->GetPlayerIdx());
		}
	}

	//ターゲットの設定
	SetTarget(Target);
}

//===========================================================================================================
//トリガー処理
//===========================================================================================================
void My::CCardDeffence::Trigger()
{
	//攻撃時に起動
}