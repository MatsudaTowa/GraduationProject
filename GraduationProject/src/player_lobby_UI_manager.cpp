//=============================================
//
//プレイヤーロビー全体のUI管理「player_lobby_UI_manager.cpp」
// Author松田永久
//
//=============================================
#include "player_lobby_UI_manager.h"
#include "ready_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CPlayerLobbyUIManager::CPlayerLobbyUIManager()
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
		m_pReadyButton = CReadyButton::Create({ 1000.0f,300.0f,0.0f }, { 100.0f,50.0f });
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
	CCharacterLobbyUIManager::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CPlayerLobbyUIManager::Update()
{
}
