//===============================================================================
//
//アクティブシーンのキャラクターステートパターン[active_scene_character_state.cpp]
//Author Matsuda Towa
//
//===============================================================================
#include "active_scene_character_state.h"
#include "character_lobby_UI_manager.h"

//===============================================================================
// コンストラクタ
//===============================================================================
My::CLobbyCharacter::CLobbyCharacter(CActiveSceneCharacter* character):m_pLobbyUImanager(nullptr),m_data()
{
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CLobbyCharacter::~CLobbyCharacter()
{
	if (m_pLobbyUImanager == nullptr) { return; }
	m_pLobbyUImanager->Uninit();
	delete m_pLobbyUImanager;
	m_pLobbyUImanager = nullptr;
}

//===============================================================================
// ロビー
//===============================================================================
void My::CLobbyCharacter::Lobby(CActiveSceneCharacter* character)
{
	if (m_pLobbyUImanager != nullptr)
	{
		D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換

		m_pLobbyUImanager->GetReadyUI()->SetCurrentReady_UI(screen_pos, m_data.isReady);
	}
	if (m_pLobbyUImanager != nullptr) { return; }
	m_pLobbyUImanager = new CCharacterLobbyUIManager;
	m_pLobbyUImanager->Init(character);
}

//===============================================================================
// 戦闘
//===============================================================================
void My::CDuelCharacter::Duel(CActiveSceneCharacter* character)
{
}
