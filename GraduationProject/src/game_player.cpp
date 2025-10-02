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
m_pState(nullptr)		//ステート初期化
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

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\nmove:%.1f,%.1f,%.1f"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetMove().x, GetMove().y, GetMove().z);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}