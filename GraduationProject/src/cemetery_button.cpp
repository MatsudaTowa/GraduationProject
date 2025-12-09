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

	//テクスチャ読み込み
	BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_NAME)));

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

	std::list<CActiveSceneCharacter*> charcter_list = CActiveSceneManager::GetInstance()->GetCharacterList();
	for (auto& itr : charcter_list)
	{
		// NOTE:今操作するものはいじくらない
		if (itr == m_pCharacter) { continue; }

		CDuelCharacter* duel_state = dynamic_cast<CDuelCharacter*>(itr->GetState());
		duel_state->SetIsCemeteryView(false);
	}

	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CardisView(duel_state);
	}
	else if (typeid(*state) == typeid(CEnemyDuelState))
	{
		CEnemyDuelState* duel_state = dynamic_cast<CEnemyDuelState*>(state);
		CardisView(duel_state);
	}

}

//=============================================
// カードを表示
//=============================================
void My::CCemeteryButton::CardisView(My::CDuelCharacter* duel_state)
{
	std::list<CCard*> card_list = duel_state->GetZoneManager()->GetCemetery()->GetList();

	//準備OKか切り替え
	bool isView = duel_state->GetIsCemeteryView();
	isView = isView ? false : true;
	duel_state->SetIsCemeteryView(isView);
}

//=============================================
// マウス接触
//=============================================
bool My::CCemeteryButton::ProcessMouseEvent()
{
	bool ishit = CButton::ProcessMouseEvent();

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
