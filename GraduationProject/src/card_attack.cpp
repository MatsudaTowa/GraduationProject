//===========================================================================================================================================================
// 
// アタックカードの処理 [card_attack.cpp]
// Author : 島津　湧人
// 
//===========================================================================================================================================================
#include "card_attack.h"
#include "active_scene_manager.h"
#include "raknet.h"

My::CCardAttack::CCardAttack(int nPriority):CCard(nPriority),
m_AttackType(),
m_nAttackValue(INT_ZERO)
{
}

My::CCardAttack::~CCardAttack()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCardAttack::Init()
{
	SetCardType(CCard::CARDTYPE_::TYPE_ATTACK);
	CCard::Init();

	//トリガー時のストラテジーを代入
	SetCardStrategy(new CAttack);
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
//キャスト処理
//===========================================================================================================
void My::CCardAttack::Cast(CDuelCharacter* duel)
{
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
void My::CCardAttack::Trigger()
{
	//ステイ後に起動
}