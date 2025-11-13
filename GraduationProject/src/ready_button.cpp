//=============================================
//
//準備できているかの切り替えボタン[ready_button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "ready_button.h"
#include "active_scene_manager.h"
#include "raknet.h"

//=============================================
// コンストラクタ
//=============================================
My::CReadyButton::CReadyButton(int nPriority):CButton(nPriority),
m_font_manager(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CReadyButton::~CReadyButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CReadyButton::Init()
{
	CButton::Init();

	SetColor(COLOR_WHITE);

	if (m_font_manager == nullptr)
	{
		m_font_manager = new CFontManager;
		m_font_manager->Init();

		const wchar_t* txt = L"READY";
		D3DXVECTOR3 pos = GetPos();
		m_font_manager->Regist(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CReadyButton::Uninit()
{
	if (m_font_manager != nullptr)
	{
		m_font_manager->Uninit();
		delete m_font_manager;
		m_font_manager = nullptr;
	}

	CButton::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CReadyButton::Update()
{
#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_RIGHT))
	{
		const wchar_t* txt = L"CANCEL";
		D3DXVECTOR3 pos = GetPos();

		m_font_manager->SetText(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_LEFT))
	{
		const wchar_t* txt = L"READY";
		D3DXVECTOR3 pos = GetPos();

		m_font_manager->SetText(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}
#endif // _DEBUG
	CButton::Update();
}

//=============================================
// 描画
//=============================================
void My::CReadyButton::Draw()
{
	CButton::Draw();
}

//=============================================
// ボタンが押された時の処理
//=============================================
void My::CReadyButton::ButtonTrigger()
{
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player == nullptr) { return; }

	CActiveSceneCharacterState* state = player->GetState();
	// ロビーじゃなかったら抜ける
	if (typeid(*state) != typeid(CPlayerLobbyState)) { return; }

	CPlayerLobbyState* lobby_state = dynamic_cast<CPlayerLobbyState*>(state);

	//準備OKか切り替え
	bool isReady = lobby_state->GetIsReady();
	isReady = isReady ? false : true;
	lobby_state->SetIsReady(isReady);

	if (isReady)
	{
		const wchar_t* txt = L"CANCEL";
		D3DXVECTOR3 pos = GetPos();

		GetFontManager()->SetText(txt, { pos.x - GetSize().x * 0.6f,pos.y,pos.z }, 25.0f, 30.0f, 50, 6);
	}
	else if (!isReady)
	{
		const wchar_t* txt = L"READY";
		D3DXVECTOR3 pos = GetPos();

		GetFontManager()->SetText(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}

	if (!CRakNet::GetInstance()->GetOnline()) return;
	//通信処理
	CRakNet::GetInstance()->GetClient()->SendReady(nullptr, CRakNet::GetInstance()->GetPeer());
}

//=============================================
// マウスとの判定
//=============================================
bool My::CReadyButton::ProcessMouseEvent()
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
	else if(!ishit)
	{
		SetColor({ 0.2f,0.2f,0.2f,1.0f });
	}

	return ishit;
}

//=============================================
// 生成
//=============================================
My::CReadyButton* My::CReadyButton::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CReadyButton* pButton = new CReadyButton;
	if (pButton == nullptr) { return nullptr; }
	pButton->SetPos(pos);
	pButton->SetSize(size);
	
	pButton->Init();
	return pButton;

}
