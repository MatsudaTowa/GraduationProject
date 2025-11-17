//===============================================================================
//
//アクティブシーンのキャラクターステートパターン[active_scene_character_state.cpp]
//Author Matsuda Towa
//
//===============================================================================
#include "active_scene_character_state.h"
#include "character_lobby_UI_manager.h"
#include "card_manager.h"
#include "zone_manager.h"

//===============================================================================
// コンストラクタ
//===============================================================================
My::CLobbyCharacter::CLobbyCharacter(CActiveSceneCharacter* /*character*/):m_pLobbyUImanager(nullptr),m_data()
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
void My::CDuelCharacter::Duel(CActiveSceneCharacter* /*character*/)
{
}

//===============================================================================
// コンストラクタ
//===============================================================================
My::CDuelCharacter::CDuelCharacter(CActiveSceneCharacter* character) : 
	m_pZoneManager(nullptr)	//ゾーンマネージャー
{
	//ゾーンマネージャーの作成
	m_pZoneManager = new CZoneManager;
	m_pZoneManager->Init();

	//カードのIDから生成
	for (auto iter : character->GetDeck())
	{
		m_pZoneManager->GetDeck()->AddList(My::CCardManager::GetInstance()->CreateCard(iter));
	}
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CDuelCharacter::~CDuelCharacter()
{
	//ゾーンマネージャーの削除
	if (m_pZoneManager != nullptr)
	{
		m_pZoneManager->Uninit();
		delete m_pZoneManager;
		m_pZoneManager = nullptr;
	}
}