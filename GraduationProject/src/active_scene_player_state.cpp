//=============================================
//
//ゲームプレイヤーのステートパターン[game_player_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_player_state.h"
#include "game_player.h"
#include "active_scene_manager.h"
#include "field.h"
#include "raknet.h"
#include "energy_charge.h"
#include "player_lobby_UI_manager.h"
#include "ready_button.h"
#include "match_start_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CPlayerLobbyState::CPlayerLobbyState(CActiveSceneCharacter* character):CLobbyCharacter(character)
{
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerLobbyState::~CPlayerLobbyState()
{
}

//=============================================
// ロビー処理
//=============================================
void My::CPlayerLobbyState::Lobby(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CActiveScenePlayer))
	{
		return;
	}
	CActiveScenePlayer* player = dynamic_cast<CActiveScenePlayer*>(character);

	CCharacterLobbyUIManager* lobbyUI = GetLobbyUI();
	if (lobbyUI == nullptr)
	{
		lobbyUI = new CPlayerLobbyUIManager;
		lobbyUI->Init(character);
		SetLobbyUI(lobbyUI);
	}
	CLobbyCharacter::Lobby(character);

	//モーション設定
	player->SetMotion(CPlayer::MOTION_NEUTRAL);


	if(typeid(*lobbyUI) != typeid(CPlayerLobbyUIManager))
	{
		return;
	}
	CPlayerLobbyUIManager* player_lobby_manager = dynamic_cast<CPlayerLobbyUIManager*>(lobbyUI);
	
	CReadyButton* pReadyButton = player_lobby_manager->GetReadyButton();
	CMatchStartButton* pStartButton = player_lobby_manager->GetStartButton();

	if (pReadyButton == nullptr && pStartButton ==  nullptr)
	{
		return;
	}

	// それぞれのマウスのイベントを呼ぶ
	pReadyButton->ProcessMouseEvent();
	pStartButton->ProcessMouseEvent();
}

//=============================================
// コンストラクタ
//=============================================
My::CPlayerDuelState::CPlayerDuelState(CActiveSceneCharacter* character):CDuelCharacter(character),
	m_pEnergyUpCount(nullptr),
	m_EnergyUpFrame(INT_ZERO),
	m_pHand(nullptr)
{
	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
		m_EnergyUpFrame = ENERGY_UP_FRAME;
	}

	//手札の生成
	//m_pHand = new CHand;
	//m_pHand->Init();
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerDuelState::~CPlayerDuelState()
{
	if (m_pHand != nullptr)
	{
		delete m_pHand;
		m_pHand = nullptr;
	}

	if (m_pEnergyUpCount != nullptr)
	{
		delete m_pEnergyUpCount;
		m_pEnergyUpCount = nullptr;
	}
}

//=============================================
// デュエル処理
//=============================================
void My::CPlayerDuelState::Duel(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CActiveScenePlayer))
	{
		return;
	}
	CActiveScenePlayer* player = dynamic_cast<CActiveScenePlayer*>(character);

	//ゲージ用チャージの更新
	CEnergy_Charge* pCharge = CEnergy_Charge::GetInstance();

	if (character->GetEnergy() < CActiveSceneCharacter::MAX_ENERGY)
	{//エナジーがMAXになったらUIの更新はしない
		pCharge->Update(static_cast<float>(m_pEnergyUpCount->GetCnt()), m_pEnergyUpCount->GetFrame());
		EnergyUp(player);
	}

	std::list<CCondition*> DebuffList = character->GetConditionList();

	//死んでいたら関数を抜ける
	for (auto& itr : DebuffList)
	{
		if (itr == nullptr) { continue; }
		if (itr->GetConditionType() == CCondition::CONDITION_TYPE::DEATH)
		{
			return; 
		}
	}

	//UIが生成されてなければ生成される
	CreateDuelUI(player);
	//モーション設定
	player->SetMotion(CPlayer::MOTION_NEUTRAL);

	/*
		* @brief 開始
		* TODO : 今だけここにおいている。のちにゲーム開始時に呼び出す
		*/

	//CHand* pHand = player->GetHand();
	if (m_pHand != nullptr)
	{
		m_pHand->Start(this);

		m_pHand->Update(this, character);

		//TODO:ハンドの総数をここで取得し設定
		//player->SetHandNum(pHand->GetTotal())
	}

	//親の更新
	CDuelCharacter::Duel(character);
}

//=============================================
// UI設定
//=============================================
void My::CPlayerDuelState::CreateDuelUI(CActiveScenePlayer* player)
{
	if (m_pHand == nullptr)
	{// 手札生成
		m_pHand = CHand::Create();
	}

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), player->GetPos()); //スクリーン座標に変換

	if (player->GetPlayerUI()->GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		player->GetPlayerUI()->SetLifeUI(pLifeUI);
	}

	if (player->GetPlayerUI()->GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 10000.0f,screen_pos.y,screen_pos.z });
		player->GetPlayerUI()->SetEnergyUI(pEnergyUI);
	}
}


//=============================================
//エナジー上げる処理
//=============================================
void My::CPlayerDuelState::EnergyUp(CActiveScenePlayer* player)
{
	int energy = player->GetEnergy();

#ifdef _DEBUG
	int frame = m_pEnergyUpCount->GetFrame();

	if (GET_INPUT_KEYBOARD->GetPress(DIK_1))
	{
		--frame;
		if (frame <= 0)
		{
			frame = 1;
		}
	}
	else if (GET_INPUT_KEYBOARD->GetPress(DIK_2))
	{
		++frame;
	}
	m_pEnergyUpCount->SetFrame(frame);

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


//=============================================
//カードを引く処理
//=============================================
void My::CPlayerDuelState::DrawCard()
{

}
