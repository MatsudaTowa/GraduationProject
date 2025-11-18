//=============================================
//
//マッチ開始ボタン[match_start_button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "match_start_button.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "card_manager.h"
//=============================================
// コンストラクタ
//=============================================
My::CMatchStartButton::CMatchStartButton(int nPriority):CButton(nPriority),
m_font_manager(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CMatchStartButton::~CMatchStartButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CMatchStartButton::Init()
{
	CButton::Init();

	SetColor(COLOR_WHITE);

	if (m_font_manager == nullptr)
	{
		m_font_manager = new CFontManager;
		m_font_manager->Init();

		//TODO:これでカードのテキスト読み込み
		//std::vector<CCard_Client::Param> vector = CCardManager::GetInstance()->GetUseCardVector();
		//const char* a = vector.at(0).Name.c_str();
		//int len = MultiByteToWideChar(CP_UTF8, 0, a, -1, NULL, 0);

		//std::wstring wtxt(len, L'\0');
		//MultiByteToWideChar(CP_UTF8, 0, a, -1, &wtxt[0], len);
	
		//D3DXVECTOR3 pos = GetPos();
		//m_font_manager->Regist(wtxt.c_str(), { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
		const wchar_t* txt = L"STARTaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR2 size = GetSize();
		m_font_manager->Regist(txt, { pos.x - size.x * 0.55f,pos.y,pos.z }, { size.x * 2.0f,size.y*2.0f }, 30.0f, 35.0f, 50, 6,COLOR_RED);
	}

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CMatchStartButton::Uninit()
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
void My::CMatchStartButton::Update()
{
	CButton::Update();
}

//=============================================
// 描画
//=============================================
void My::CMatchStartButton::Draw()
{
	CObject2D::Draw();
}

//=============================================
// ボタンが押された時の処理
//=============================================
void My::CMatchStartButton::ButtonTrigger()
{
	CActiveSceneState* scene_state = CActiveSceneManager::GetInstance()->GetState();
	if (typeid(*scene_state) != typeid(CLobby)) { return; }
	CLobby* lobby_state = dynamic_cast<CLobby*>(scene_state);

	bool is_online = CRakNet::GetInstance()->GetOnline();
	if (is_online)
	{
		//サーバーに対戦開始の合図を送る
		CRakNet::GetInstance()->SendStartSign();
	}
	else if(!is_online)
	{
		lobby_state->SetBattleSign(true);
	}

}

//=============================================
// マウスとの判定
//=============================================
bool My::CMatchStartButton::ProcessMouseEvent()
{
	SetColor({ 0.2f,0.2f,0.2f,1.0f });

	bool ishit = CButton::ProcessMouseEvent();

	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player->GetPlayerIdx() != INT_ZERO) { return ishit; }

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
My::CMatchStartButton* My::CMatchStartButton::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CMatchStartButton* pButton = new CMatchStartButton;
	if (pButton == nullptr) { return nullptr; }
	pButton->SetPos(pos);
	pButton->SetSize(size);

	pButton->Init();
	return pButton;
}
