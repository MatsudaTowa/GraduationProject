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
#include "wait_zone.h"

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

//デュエルステートで使う定数
namespace
{
	const D3DXVECTOR3 FIRST_POS{ 1050.0f, 660.0f, 0.0f };
	const float CARD_SPACE{ 40.0f };
}

//=============================================
// コンストラクタ
//=============================================
My::CPlayerDuelState::CPlayerDuelState(CActiveSceneCharacter* character) :CDuelCharacter(character),
m_nEnergyUpCount(0),
m_EnergyUpFrame(INT_ZERO),
m_pHand(nullptr),
m_pCemetery(nullptr)
{
	m_EnergyUpFrame = ENERGY_UP_FRAME;
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerDuelState::~CPlayerDuelState()
{
	if (m_pCemetery != nullptr)
	{
		delete m_pCemetery;
		m_pCemetery = nullptr;
	}
	if (m_pCemetery != nullptr)
	{
		delete m_pCemetery;
		m_pCemetery = nullptr;
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
		pCharge->Update(static_cast<float>(m_nEnergyUpCount), ENERGY_UP_FRAME);
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

	if (m_pCemetery != nullptr)
	{
		m_pCemetery->Update(this, character);
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
	if (m_pCemetery == nullptr)
	{
		m_pCemetery = CCemetery::Create();
	}
	


	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), player->GetPos()); //スクリーン座標に変換

	if (player->GetPlayerUI()->GetCemeteryButton() == nullptr)
	{
		CCemeteryButton* pCemeteryButton = CCemeteryButton::Create(screen_pos, player);
		player->GetPlayerUI()->SetCemeteryButton(pCemeteryButton);

	}
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
	
	if (GET_INPUT_KEYBOARD->GetPress(DIK_1))
	{
		m_EnergyUpFrame  -= 100;
		if (m_EnergyUpFrame <= 0)
		{
			m_EnergyUpFrame = 1;
		}
	}
	else if (GET_INPUT_KEYBOARD->GetPress(DIK_2))
	{
		m_EnergyUpFrame += 100;
	}
	

#endif // _DEBUG

	/*if (m_pEnergyUpCount == nullptr)
	{
		return;
	}

	if (!m_pEnergyUpCount->CountUp())
	{
		return;
	}*/

	//経過時間を取得
	m_nEnergyUpCount += CManager::GetInstance()->GetElapsedTime();

	//エナジーの更新時間を超えているなら繰り返す
	while (m_nEnergyUpCount > m_EnergyUpFrame)
	{
		m_nEnergyUpCount -= m_EnergyUpFrame;

		//エナジー増加
		++energy;
	}

	if (CRakNet::GetInstance()->GetOnline()) return;

	//エナジーの設定
	player->SetEnergy(energy);
}


//=============================================
//カードを引く処理
//=============================================
void My::CPlayerDuelState::DrawCard()
{

}

//=============================================
//待機ゾーンのカード表示処理
//=============================================
void My::CPlayerDuelState::ViewWait(CWaitZone* zone)
{
	//周回数
	int nCount = 0;

	//待機カードの周回
	for (CCard* pCard : zone->GetList())
	{
		//座標変換しずらす
		D3DXVECTOR3 pos = ConvertToWorldPoint(GET_CAMERA(GET_CAMERA_IDX), FIRST_POS, FIRST_POS);
		pos.x -= CARD_SPACE * nCount;
		pCard->SetPos(pos);

		//最初のカード以外は小さくする
		//TODO : カードフレームを取得できないので取得できるようになったら実装
		if (nCount > 0)
		{
			/*pCard->SetSize({ 0.0f, 0.0f, 0.0f });
			pos.z += 100.0f;
			pCard->SetPos(pos);*/
		}

		nCount++;	//カウントアップ
	}
}