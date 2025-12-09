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
	//更新するカードを保管するリスト
	std::list<CCard*> CardList;
	CardList.clear();

	m_pZoneManager->Update();

	//カードをリストに追加
	auto AddList = [&](CZone* zone)
	{
		for (auto& iter : zone->GetList())
		{
			CardList.push_back(iter);
		}
	};

	//更新が必要なカードのゾーンを追加
	AddList(m_pZoneManager->GetCastPreviewZone());	//キャスト
	//AddList(m_pZoneManager->GetWaitZone());			//守備待機

	//更新
	for (auto& iter : CardList)
	{
		iter->Update(this);
	}

	//クリア
	CardList.clear();

	//待機ゾーンの表示処理
	ViewWait(m_pZoneManager->GetWaitZone());

	//描画を行いたいゾーンのみ更新
	//m_pZoneManager->GetCastPreviewZone()->Update(this);		//キャスト
	//m_pZoneManager->GetHandZone()->Update(this);			//手札
	//m_pZoneManager->GetWaitZone()->Update(this);			//守備待機ゾーン
	//m_pZoneManager->GetCemetery()->Update(this);			//墓地ゾーン
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

	//デッキを混ぜる処理
	std::list<int> DeckList = character->GetDeck();
	std::vector<int> DeckVector;

	for (auto iter : character->GetDeck())
	{
		DeckVector.push_back(iter);
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(DeckVector.begin(), DeckVector.end(), g);

	//カードのIDから生成
	for (auto iter : DeckVector)
	{
		CCard* pCard = My::CCardManager::GetInstance()->CreateCard(iter);
		pCard->SetUserId(character->GetPlayerIdx());
		pCard->SetCurrentZone(CCard::DECK);
		pCard->Init();
		m_pZoneManager->GetDeck()->AddList(pCard);
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