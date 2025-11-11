//=============================================
//
//ロビー全体のUI管理「lobby_UI_manager.cpp」
// Author松田永久
//
//=============================================
#include "lobby_UI_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CLobbyUIManager::CLobbyUIManager():
	m_pReadyUI(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CLobbyUIManager::~CLobbyUIManager()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CLobbyUIManager::Init(CActiveSceneCharacter* character)
{
	if (m_pReadyUI != nullptr) { return S_OK; }
	m_pReadyUI = new CReadyUI;
	m_pReadyUI->Init(character);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CLobbyUIManager::Uninit()
{
	if (m_pReadyUI == nullptr) { return; }
	m_pReadyUI->Uninit();
	delete m_pReadyUI;
	m_pReadyUI = nullptr;
}

//=============================================
// 更新
//=============================================
void My::CLobbyUIManager::Update()
{
}
