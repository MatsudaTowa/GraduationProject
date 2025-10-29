//=============================================
//
//アクティブシーンキャラクター処理[active_scene_character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_character.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneCharacter::CActiveSceneCharacter(int nPriority):CCharacter(nPriority),
m_pState(nullptr),												//ステート初期化
m_status(),
m_isHost(false),
m_playerIdx(-1),
m_pEneryUI(nullptr),
m_pLifeUI(nullptr),
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
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}
	if (m_pEneryUI != nullptr)
	{
		m_pEneryUI->Uninit();
		m_pEneryUI = nullptr;
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
	if (m_pLifeUI != nullptr)
	{
		if (m_status.life > MAX_LIFE)
		{
			SetLife(MAX_LIFE);
		}
		if (m_status.life < INT_ZERO)
		{
			SetLife(INT_ZERO);
		}
		m_pLifeUI->SetLifeNumber(m_status.life);

		// スクリーン座標に数字を描画
		int i = INT_ZERO;

		for (auto& itr : m_pLifeUI->GetNumVector())
		{
			if (itr == nullptr) { continue; }

			// TODO: 30.0fは桁ずらし値 取得できるように変更予定
			itr->SetPos({ screen_pos.x - (i * 30.0f),screen_pos.y,screen_pos.z });

			//桁ずらす
			++i;
		}
	}
	if (m_pEneryUI != nullptr)
	{
		if (m_status.energy > MAX_ENERGY)
		{
			SetEnergy(MAX_LIFE);
		}
		if (m_status.energy < INT_ZERO)
		{
			SetEnergy(INT_ZERO);
		}
		m_pEneryUI->SetEnergyNumber(m_status.energy);

		// スクリーン座標に数字を描画
		int i = INT_ZERO;

		for (auto& itr : m_pEneryUI->GetNumVector())
		{
			if (itr == nullptr) { continue; }

			// TODO: 30.0fは桁ずらし値 取得できるように変更予定
			itr->SetPos({ screen_pos.x + 100.0f - (i * 30.0f),screen_pos.y,screen_pos.z });

			//桁ずらす
			++i;
		}
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
