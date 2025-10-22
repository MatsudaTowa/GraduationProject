//===============================================================================
//
//アクティブシーンのキャラクターステートパターン[active_scene_character_state.cpp]
//Author Matsuda Towa
//
//===============================================================================
#include "active_scene_character_state.h"

//===============================================================================
// コンストラクタ
//===============================================================================
My::CLobbyCharacter::CLobbyCharacter(CActiveSceneCharacter* character):m_pReadyUI(nullptr)
{
	if (m_pReadyUI != nullptr) { return; }
	m_pReadyUI = new CReadyUI;
	m_pReadyUI->Init(character);
}

//===============================================================================
// デストラクタ
//===============================================================================
My::CLobbyCharacter::~CLobbyCharacter()
{
	if (m_pReadyUI == nullptr) { return; }
	m_pReadyUI->Uninit();
	delete m_pReadyUI;
	m_pReadyUI = nullptr;
}

//===============================================================================
// ロビー
//===============================================================================
void My::CLobbyCharacter::Lobby(CActiveSceneCharacter* character)
{
	if (m_pReadyUI != nullptr)
	{
		m_pReadyUI->SetCurrentReady_UI(character);
	}
}

//===============================================================================
// 戦闘
//===============================================================================
void My::CDuelCharacter::Duel(CActiveSceneCharacter* character)
{
}
