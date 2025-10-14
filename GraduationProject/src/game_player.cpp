//=============================================
//
//ゲームプレイヤー[game_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player.h"
#include "active_manager.h"
#include "hand.h"

//=============================================
// コンストラクタ
//=============================================
My::CGamePlayer::CGamePlayer(int nPriority):CPlayer(nPriority),
m_pEnergyUpCount(nullptr),										//エナジー計測カウント
m_pState(nullptr),												//ステート初期化
m_pHand(nullptr)												// 手札初期化
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

	if (m_pEnergyUpCount == nullptr)
	{
		m_pEnergyUpCount = new CCount;
		m_pEnergyUpCount->SetCnt(INT_ZERO);
		m_pEnergyUpCount->SetFrame(ENERGY_UP_FRAME);
	}

	if (m_pHand == nullptr)
	{// 手札生成
		m_pHand = CHand::Create();
	}

	//親クラスの初期化実行
	CPlayer::Init();

	//ゲームのマネージャに自分を代入
	CGameManager::GetInstance()->SetPlayer(this);
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //スクリーン座標に変換

	if (GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		SetLifeUI(pLifeUI);
	}

	if (GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
		SetEnergyUI(pEnergyUI);
	}
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

	if (m_pHand != nullptr)
	{
		delete m_pHand;
		m_pHand = nullptr;
	}
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
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

	/*
		* @brief 開始
		* TODO : 今だけここにおいている。のちにゲーム開始時に呼び出す
		*/

	if (m_pHand != nullptr)
	{
		m_pHand->Start();

		m_pHand->Update();
	}
	
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
	else if (m_pState == nullptr)
	{
		delete state;
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