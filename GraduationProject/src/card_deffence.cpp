#include "card_deffence.h"
#include "card_strategy.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "zone_manager.h"

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
//キャストをしたかの確認
//===========================================================================================================
bool My::CCardDeffence::IsCast(CDuelCharacter* duel)
{
	//プレイヤーが対象かの確認
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player->GetArea() == GetTarget())
	{
		RegistTargetList(player);
		return true;
	}

	//キャストしたか
	bool isCast = false;

	//敵が対象かの確認
	std::list<CEnemy*> enemy_list = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();
	for (auto& itr : enemy_list)
	{
		if (itr == nullptr) { continue; }
		if (itr->GetArea() != GetTarget()) { continue; }

		//対象の状態を取得
		if (typeid(CDuelCharacter) != typeid(*itr->GetState())) continue;		//中身の一致を確認
		CDuelCharacter* State = dynamic_cast<CDuelCharacter*>(itr->GetState());	//キャスト
		if (State == nullptr) continue;											//中身があるか確認

		//TODO::現在は先に出した攻撃カードを参照するが、将来的には相手のステイ中のカードから選択する
		if (State->GetZoneManager()->GetCastPreviewZone()->GetList().empty()) continue;

		//キャストゾーンのカード周回
		for (auto& iter : State->GetZoneManager()->GetCastPreviewZone()->GetList())
		{
			//攻撃のカードが存在するかを確認
			if (iter->GetCardType() != CCard::CARDTYPE_::TYPE_ATTACK) continue;

			RegistTargetList(itr);
			isCast = true;
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
//トリガー処理
//===========================================================================================================
void My::CCardDeffence::Trigger()
{
	//攻撃時に起動
}