//=============================================
//
//ゲームプレイヤー[game_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player.h"
#include "game_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CGamePlayer::CGamePlayer(int nPriority):CPlayer(nPriority),
m_pEnergyUpCount(nullptr),										//エナジー計測カウント
m_pState(nullptr)												//ステート初期化
{
}

//=============================================
// デストラクタ
//=============================================
My::CGamePlayer::~CGamePlayer()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CGamePlayer::Init()
{
	if (m_pState == nullptr)
	{
		m_pState = new CNeutralState;
	}

	if (GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create({600.0f,600.0f,0.0f});
		SetLifeUI(pLifeUI);
	}

	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
	}

	//親クラスの初期化実行
	CPlayer::Init();

	//ゲームのマネージャに自分を代入
	CGameManager::GetInstance()->SetPlayer(this);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CGamePlayer::Uninit()
{
	if (m_pEnergyUpCount != nullptr)
	{
		delete m_pEnergyUpCount;
		m_pEnergyUpCount = nullptr;
	}
	//親クラスの終了処理
	CPlayer::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CGamePlayer::Update()
{
	if (m_pState != nullptr)
	{
		m_pState->Neutral(this);
	}

#ifdef _DEBUG
	int life = GetLife();
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_L))
	{
		++life;
	}
	else if (GET_INPUT_KEYBOARD->GetTrigger(DIK_K))
	{
		--life;
	}
	SetLife(life);
#endif // _DEBUG


	EnergyUp();

	//親クラスの更新
	CPlayer::Update();
}

//=============================================
// 描画
//=============================================
void My::CGamePlayer::Draw()
{
	//親クラスの描画
	CPlayer::Draw();
}

//=============================================
// ステート変更
//=============================================
void My::CGamePlayer::ChangeState(CGamePlayerState* state)
{
	//今のステートを消し引数のステートに切り替える
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = state;
	}
}

//=============================================
//エナジー上げる処理
//=============================================
void My::CGamePlayer::EnergyUp()
{
	int energy = GetEnergy();

#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_1))
	{
		energy += 10;
		SetEnergy(energy);
	}
#endif // _DEBUG

	if (m_pEnergyUpCount == nullptr)
	{
		return;
	}

	if (!m_pEnergyUpCount->CountUp())
	{
		return;
	}

	//エナジー増加
	++energy;
	SetEnergy(energy);
	m_pEnergyUpCount->SetCnt(INT_ZERO);
}

//=============================================
// 入力
//=============================================
void My::CGamePlayer::Input()
{
}

//=============================================
//プレイヤーのデバッグ表示
//=============================================
void My::CGamePlayer::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = CManager::GetInstance()->GetRenderer()->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\nエナジー:%d\n体力:%d Kで減少 Lで増加"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z,GetEnergy(),GetLife());
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}