//=============================================
//
//墓地の墓地ボタン[cemetery_button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "cemetery_button.h"
#include "zone_manager.h"
#include "enemy_state.h"
#include "active_scene_manager.h"

namespace
{
	const D3DXVECTOR2 POLY_SIZE = { 30.0f,30.0f };
	const std::string TEX_NAME = "data\\TEXTURE\\cementery_icon.png";
	const std::string USE_TEX_NAME = "data\\TEXTURE\\cancel.png";	//使われているときのテクスチャの名前
}

//=============================================
// コンストラクタ
//=============================================
My::CCemeteryButton::CCemeteryButton(int nPriority):CButton(nPriority),
m_pCharacter(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCemeteryButton::~CCemeteryButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCemeteryButton::Init()
{
	SetColor(COLOR_WHITE);

	SetSize(POLY_SIZE);

	CButton::Init();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCemeteryButton::Uninit()
{
	if (m_pCharacter != nullptr)
	{
		m_pCharacter = nullptr;
	}
	CButton::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCemeteryButton::Update()
{	
	CardisView();
	CButton::Update();
}

//=============================================
// 描画
//=============================================
void My::CCemeteryButton::Draw()
{
	CButton::Draw();
}

//=============================================
// ボタントリガー
//=============================================
void My::CCemeteryButton::ButtonTrigger()
{
	CActiveSceneManager::GetInstance()->GetCharacterList();
	CActiveSceneCharacterState* state = m_pCharacter->GetState();
	bool isView = false;

	std::list<CActiveSceneCharacter*> charcter_list = CActiveSceneManager::GetInstance()->GetCharacterList();
	for (auto& itr : charcter_list)
	{
		// NOTE:今操作するものはいじくらない
		if (itr == m_pCharacter) { continue; }


		// NOTE:他は解除
		CDuelCharacter* duel_state = dynamic_cast<CDuelCharacter*>(itr->GetState());
		duel_state->SetIsCemeteryView(false);
		duel_state->GetZoneManager()->GetCemetery()->GetSelectionRange()->SetisDraw(false);
	}

	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CCemeteryZone* zone = duel_state->GetZoneManager()->GetCemetery();
		std::list<CCard*> card_list = zone->GetList();

		//準備OKか切り替え
		isView = duel_state->GetIsCemeteryView();
		isView = isView ? false : true;

		duel_state->SetIsCemeteryView(isView);

		zone->GetSelectionRange()->SetisDraw(isView);
	}
	else if (typeid(*state) == typeid(CEnemyDuelState))
	{
		CEnemyDuelState* duel_state = dynamic_cast<CEnemyDuelState*>(state);
		CCemeteryZone* zone = duel_state->GetZoneManager()->GetCemetery();

		//準備OKか切り替え
		isView = duel_state->GetIsCemeteryView();
		isView = isView ? false : true;
		duel_state->SetIsCemeteryView(isView);
		zone->GetSelectionRange()->SetisDraw(isView);
	}

	if (isView)
	{
		CActiveSceneCharacterState* player_state = CActiveSceneManager::GetInstance()->GetPlayer()->GetState();
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(player_state);

		duel_state->SetIsWaitView(false);
	}

}

//=============================================
// カードを表示
//=============================================
void My::CCemeteryButton::CardisView()
{
	CActiveSceneCharacterState* state = m_pCharacter->GetState();
	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CCemeteryZone* zone = duel_state->GetZoneManager()->GetCemetery();
		//準備OKか切り替え
		bool isView = duel_state->GetIsCemeteryView();
		std::list<CCard*> card_list = zone->GetList();

		for (auto& itr : card_list)
		{
			if (itr == nullptr) { continue; }
			itr->SetisDraw(isView);
		}
	}
	// ロビーじゃなかったら抜ける
	else if (typeid(*state) == typeid(CEnemyDuelState))
	{
		CEnemyDuelState* duel_state = dynamic_cast<CEnemyDuelState*>(state);
		CCemeteryZone* zone = duel_state->GetZoneManager()->GetCemetery();
		//準備OKか切り替え
		bool isView = duel_state->GetIsCemeteryView();
		std::list<CCard*> card_list = zone->GetList();

		for (auto& itr : card_list)
		{
			if (itr == nullptr) { continue; }
			itr->SetisDraw(isView);
		}
	}
}

//=============================================
// マウス接触
//=============================================
bool My::CCemeteryButton::ProcessMouseEvent()
{
	//TODO:通常時はこれでいいが、墓地を見ているときは墓地のエリア以外の場所を触ったらのboolに変わる
	bool ishit = CButton::ProcessMouseEvent();
	CActiveSceneCharacterState* state = m_pCharacter->GetState();

	CDuelCharacter* duel_state = dynamic_cast<CDuelCharacter*>(m_pCharacter->GetState());
	bool isView = duel_state->GetIsCemeteryView();

	if (isView)
	{
		CSelectionRange* range = duel_state->GetZoneManager()->GetCemetery()->GetSelectionRange();
		bool is_hit_area = GET_COLISION->Check2DPolygonColision(GET_INPUT_MOUSE->GetMousePos(), { 3.0f,3.0f }, { range->GetPos().x,range->GetPos().y,0.0f }, range->GetSize());
		//テクスチャ読み込み
		BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(USE_TEX_NAME)));
		SetColor(COLOR_WHITE);

		//墓地を見ているときは
		if (GET_INPUT_MOUSE->GetTrigger(0)&& !is_hit_area)
		{
			//押された時の処理の名前
			ButtonTrigger();
		}
		return ishit;

	}
	else if (!isView)
	{
		//テクスチャ読み込み
		BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_NAME)));
	}

	if (ishit)
	{
		SetColor(COLOR_WHITE);
		if (GET_INPUT_MOUSE->GetTrigger(0))
		{
			//押された時の処理の名前
			ButtonTrigger();
		}
	}
	else if (!ishit)
	{
		SetColor({ 0.2f,0.2f,0.2f,1.0f });
	}


	return ishit;
}

//=============================================
// 生成
//=============================================
My::CCemeteryButton* My::CCemeteryButton::Create(D3DXVECTOR3 pos,CActiveSceneCharacter* pCharacter)
{
	CCemeteryButton* pButton = new CCemeteryButton;
	if (pButton == nullptr) { return nullptr; }
	pButton->SetPos(pos);
	pButton->m_pCharacter = pCharacter;

	pButton->Init();
	return pButton;
}
