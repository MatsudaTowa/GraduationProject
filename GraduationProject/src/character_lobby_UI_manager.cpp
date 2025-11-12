//=============================================
//
//ロビー全体のUI管理「lobby_UI_manager.cpp」
// Author松田永久
//
//=============================================
#include "character_lobby_UI_manager.h"
#include "ready_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CCharacterLobbyUIManager::CCharacterLobbyUIManager():
	m_pReadyUI(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCharacterLobbyUIManager::~CCharacterLobbyUIManager()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCharacterLobbyUIManager::Init(CActiveSceneCharacter* character)
{
	if (m_pReadyUI == nullptr)
	{
		m_pReadyUI = new CReadyUI;
		m_pReadyUI->Init(character);
	}

	if (m_pReadyButton == nullptr)
	{
		m_pReadyButton = CReadyButton::Create({ 1000.0f,300.0f,0.0f }, {100.0f,50.0f});
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCharacterLobbyUIManager::Uninit()
{
	if (m_pReadyUI != nullptr)
	{
		m_pReadyUI->Uninit();
		delete m_pReadyUI;
		m_pReadyUI = nullptr;
	}
	if (m_pReadyButton != nullptr)
	{
		m_pReadyButton->Uninit();
	}
}

//=============================================
// 更新
//=============================================
void My::CCharacterLobbyUIManager::Update()
{
}
