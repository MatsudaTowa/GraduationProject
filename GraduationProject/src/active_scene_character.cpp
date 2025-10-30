//=============================================
//
//アクティブシーンキャラクター処理[active_scene_character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_character.h"
#include "active_scene_state.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneCharacter::CActiveSceneCharacter(int nPriority):CCharacter(nPriority),
m_pState(nullptr),												//ステート初期化
m_status(),
m_isHost(false),
m_playerIdx(-1),
m_pPlayerUI(nullptr),
m_area()
{
	m_DebuffList.clear();
}

//=============================================
// デストラクタ
//=============================================
My::CActiveSceneCharacter::~CActiveSceneCharacter()
{
	for (auto& itr : m_DebuffList)
	{
		if (itr == nullptr) { continue; }
		delete itr;
		itr = nullptr;
	}

	m_DebuffList.clear();
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
	//ステータス設定
	m_status.deckSize = START_DECK;
	m_status.hand_num = START_HAND;
	m_status.life = START_LIFE;
	m_status.energy = START_ENERGY;
	m_status.trash = INT_ZERO;

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
	for (auto& itr : m_DebuffList)
	{
		if (itr == nullptr) { continue; }
		itr->Debuff(this);
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
void My::CActiveSceneCharacter::Regist(CDebuff* debuff)
{
	for (auto& itr : m_DebuffList)
	{//デバフの情報を回してdeathがあれば抜ける
	 //NOTE:二回死をしないため
		if (itr == nullptr) { continue; }
		if (itr->GetDebuffType() == CDebuff::DEBUFF_TYPE::DEATH)
		{
			delete debuff;
			return;
		}
	}
	m_DebuffList.push_back(debuff);
}

//=============================================
// デバフ削除
//=============================================
void My::CActiveSceneCharacter::Remove(CDebuff* debuff)
{
	//サイズが0なら抜ける
	if (m_DebuffList.size() == 0)
	{
		return;
	}
	delete debuff;
	debuff = nullptr;
	//デバフの情報を削除
	m_DebuffList.remove(debuff);
}
