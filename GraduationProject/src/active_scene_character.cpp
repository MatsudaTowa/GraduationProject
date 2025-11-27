//=============================================
//
//アクティブシーンキャラクター処理[active_scene_character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_character.h"
#include "active_scene_state.h"
#include "debuff.h"
#include "card_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneCharacter::CActiveSceneCharacter(int nPriority):CCharacter(nPriority),
m_pState(nullptr),												//ステート初期化
m_status(),
m_isHost(false),
m_playerIdx(-1),
m_pPlayerUI(nullptr),
m_area(),
m_Deck(),
m_pTargetArrowManager()
{
	m_ConditionList.clear();
	m_Deck.clear();
}

//=============================================
// デストラクタ
//=============================================
My::CActiveSceneCharacter::~CActiveSceneCharacter()
{
	for (auto& itr : m_ConditionList)
	{
		if (itr == nullptr) { continue; }
		delete itr;
		itr = nullptr;
	}

	m_ConditionList.clear();
}

//=============================================
// 初期化
//=============================================
HRESULT My::CActiveSceneCharacter::Init()
{
	if (m_pState== nullptr)
	{
		m_pState = new CLobbyCharacter(this);
	}
	if (m_pPlayerUI == nullptr)
	{
		m_pPlayerUI = new CPlayerUI;
		m_pPlayerUI->Init(this);
	}
	if (m_pTargetArrowManager == nullptr)
	{
		m_pTargetArrowManager = new CTargetArrowManager;
	}

	//ステータス設定
	m_status.deckSize = START_DECK;
	m_status.hand_num = START_HAND;
	m_status.life = START_LIFE;
	m_status.energy = START_ENERGY;
	m_status.trash = INT_ZERO;

	//デッキの生成
	CreateDeck();

	CCharacter::Init();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CActiveSceneCharacter::Uninit()
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}
	if (m_pPlayerUI != nullptr)
	{
		m_pPlayerUI->Uninit();
		delete m_pPlayerUI;
		m_pPlayerUI = nullptr;
	}
	if (m_pTargetArrowManager != nullptr)
	{
		delete m_pTargetArrowManager;
		m_pTargetArrowManager = nullptr;
	}

	CCharacter::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CActiveSceneCharacter::Update()
{
	if (m_pState != nullptr)
	{
		m_pState->Lobby(this);

		m_pState->Duel(this);
	}
	for (auto& itr : m_ConditionList)
	{
		if (itr == nullptr) { continue; }
		itr->Condition(this);
	}

	UpdateUI();
	CCharacter::Update();
}

//=============================================
// UIの更新
//=============================================
void My::CActiveSceneCharacter::UpdateUI()
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //スクリーン座標に変換

	//HPが0なら殺す
	DeathRegist();
	
	//TODO:上の処理をここにまとめよう
	if (m_pPlayerUI != nullptr)
	{
		m_pPlayerUI->SetCurrentPlayer_UI({screen_pos},this);
	}
}

//=============================================
// 描画
//=============================================
void My::CActiveSceneCharacter::Draw()
{
	CCharacter::Draw();
}

//=============================================
// ステート変更
//=============================================
void My::CActiveSceneCharacter::ChangeState(CActiveSceneCharacterState* state)
{
	//今のステートを消し引数のステートに切り替える
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = state;
	}
	else if (m_pState == nullptr)
	{
		delete state;
	}
}

//=============================================
//  HPが0になったら殺す処理
//=============================================
void My::CActiveSceneCharacter::DeathRegist()
{
	if (m_status.life <= INT_ZERO)
	{
		Regist(new CDeath);
	}
}

//=============================================
// デバフ登録
//=============================================
void My::CActiveSceneCharacter::Regist(CCondition* condition)
{
	for (auto& itr : m_ConditionList)
	{//デバフの情報を回してdeathがあれば抜ける
	 //NOTE:二回死をしないため
		if (itr == nullptr) { continue; }
		if (itr->GetConditionType() == CCondition::CONDITION_TYPE::DEATH)
		{
			delete condition;
			return;
		}
	}
	m_ConditionList.push_back(condition);
}

//=============================================
// デバフ削除
//=============================================
void My::CActiveSceneCharacter::Remove(CCondition* condition)
{
	//サイズが0なら抜ける
	if (m_ConditionList.size() == 0)
	{
		return;
	}
	delete condition;
	condition = nullptr;
	//デバフの情報を削除
	m_ConditionList.remove(condition);
}

//=============================================
// デッキの生成
//=============================================
void My::CActiveSceneCharacter::CreateDeck()
{
	//カード番号
	int nCardNum = My::CCardManager::GetInstance()->GetUseCardVector().size();

	//デッキの枚数分周回
	for (int i = 0; i < 40; i++)
	{
		//ランダムで仮生成
		int nID = static_cast<int>(Rundom(1, nCardNum));
		m_Deck.push_back(nID);			//リストに保存
	}
}

//=============================================
// デッキの生成
//=============================================
void My::CActiveSceneCharacter::ReceiveDamage(int damage)
{
	//ダメージを受ける
	if (m_status.life > INT_ZERO)
	{
		m_status.life -= damage;
	}
}
