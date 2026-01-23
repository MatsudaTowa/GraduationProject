//=============================================
//
//プレイヤーロビー全体のUI管理「player_lobby_UI_manager.cpp」
// Author松田永久
//
//=============================================
#include "player_lobby_UI_manager.h"
#include "ready_button.h"
#include "match_start_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CPlayerLobbyUIManager::CPlayerLobbyUIManager():m_pReadyButton(nullptr),
m_pStartButton(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerLobbyUIManager::~CPlayerLobbyUIManager()
{
}

//=============================================
// 初期化                             
//=============================================
HRESULT My::CPlayerLobbyUIManager::Init(CActiveSceneCharacter* character)
{
	if (m_pReadyButton == nullptr)
	{
		m_pReadyButton = CReadyButton::Create({ 1000.0f,550.0f,0.0f }, { 150.0f,50.0f });
	}
	if (m_pStartButton == nullptr)
	{
		m_pStartButton = CMatchStartButton::Create({ 1100.0f,660.0f,0.0f }, { 150.0f,50.0f });
	}
	CCharacterLobbyUIManager::Init(character);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CPlayerLobbyUIManager::Uninit()
{
	if (m_pReadyButton != nullptr)
	{
		m_pReadyButton->Uninit();
		m_pReadyButton = nullptr;
	}
	if (m_pStartButton != nullptr)
	{
		m_pStartButton->Uninit();
		m_pStartButton = nullptr;
	}
	CCharacterLobbyUIManager::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CPlayerLobbyUIManager::Update()
{
}
