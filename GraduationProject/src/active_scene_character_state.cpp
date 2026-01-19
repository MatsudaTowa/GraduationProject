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
void My::CDuelCharacter::Duel(CActiveSceneCharacter* character)
{
	//更新するカードを保管するリスト
	std::list<CCard*> CardList;
	CardList.clear();
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

	//墓地の更新
	if (m_pCemetery != nullptr)
	{
		m_pCemetery->Update(this, character);
	}

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
	m_pZoneManager(nullptr),	//ゾーンマネージャー
	m_pCemetery(nullptr)
{
	//カウント用のマップ
	std::map<int, int> SameTypeCounter;

	//同じ種類のカードの何番目かを算出
	auto CalcSameTypeCount = [&SameTypeCounter](int cardid)
	{
		//返す用
		int nCardId = -1;

		//要素を見つける
		auto nCount = SameTypeCounter.find(cardid);

		//あるなら
		if (nCount != SameTypeCounter.end())
		{
			//カウントアップ
			nCount->second++;
			SameTypeCounter[cardid] = nCount->second;
			nCardId = nCount->second;
		}
		else
		{
			//0の新しい要素を追加
			SameTypeCounter[cardid] = 0;
			nCardId = 0;
		}

		return nCardId;
	};

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

	//シャッフル処理
	//std::random_device rd;
	//std::mt19937 g(rd());

	//std::shuffle(DeckVector.begin(), DeckVector.end(), g);

	//カードのIDから生成
	for (auto iter : DeckVector)
	{
		CCard* pCard = My::CCardManager::GetInstance()->CreateCard(iter);
		pCard->SetUserId(character->GetPlayerIdx());
		pCard->SetCurrentZone(CCard::DECK);
		pCard->Init();
		pCard->SetSameTypeId(CalcSameTypeCount(pCard->GetBaseStatus().nCardID));
		m_pZoneManager->GetDeck()->AddList(pCard);
	}

	if (m_pCemetery == nullptr)
	{
		m_pCemetery = CCemetery::Create();
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

	//墓地ポインタの削除
	if (m_pCemetery != nullptr)
	{
		delete m_pCemetery;
		m_pCemetery = nullptr;
	}
}