//=============================================
//
//ゲームプレイヤーのステートパターン[game_player_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player_state.h"
#include "game_player.h"
#include "active_manager.h"
#include "field.h"

//=============================================
// コンストラクタ
//=============================================
My::CLobbyState::CLobbyState()
{
}

//=============================================
// デストラクタ
//=============================================
My::CLobbyState::~CLobbyState()
{
}

//=============================================
// ロビー処理
//=============================================
void My::CLobbyState::Lobby(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CGamePlayer))
	{
		return;
	}
	CGamePlayer* player = dynamic_cast<CGamePlayer*>(character);
	//モーション設定
	player->SetMotion(CPlayer::MOTION_NEUTRAL);
}

//=============================================
// コンストラクタ
//=============================================
My::CDuelState::CDuelState():m_pEnergyUpCount(nullptr)
{
	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
	}
}

//=============================================
// デストラクタ
//=============================================
My::CDuelState::~CDuelState()
{
	if (m_pEnergyUpCount != nullptr)
	{
		delete m_pEnergyUpCount;
		m_pEnergyUpCount = nullptr;
	}
}

//=============================================
// デュエル処理
//=============================================
void My::CDuelState::Duel(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CGamePlayer))
	{
		return;
	}
	CGamePlayer* player = dynamic_cast<CGamePlayer*>(character);
	//UIが生成されてなければ生成される
	CreateDuelUI(player);
	//モーション設定
	player->SetMotion(CPlayer::MOTION_NEUTRAL);

	/*
		* @brief 開始
		* TODO : 今だけここにおいている。のちにゲーム開始時に呼び出す
		*/

	CHand* pHand = player->GetHand();
	if (pHand != nullptr)
	{
		pHand->Start();

		pHand->Update();
	}

	EnergyUp(player);
}

//=============================================
// UI設定
//=============================================
void My::CDuelState::CreateDuelUI(CGamePlayer* player)
{
	if (player->GetHand() == nullptr)
	{// 手札生成
		player->SetHand(CHand::Create());
	}

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), player->GetPos()); //スクリーン座標に変換

	if (player->GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		player->SetLifeUI(pLifeUI);
	}

	if (player->GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
		player->SetEnergyUI(pEnergyUI);
	}
}


//=============================================
//エナジー上げる処理
//=============================================
void My::CDuelState::EnergyUp(CGamePlayer* player)
{
	int energy = player->GetEnergy();

#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_1))
	{
		energy += 10;
		player->SetEnergy(energy);
	}
#endif // _DEBUG

	if (m_pEnergyUpCount == nullptr)
	{
		return;
	}

	if (!m_pEnergyUpCount->CountUp())
	{
		return;
	}

	//エナジー増加
	++energy;
	player->SetEnergy(energy);
	m_pEnergyUpCount->SetCnt(INT_ZERO);
}
