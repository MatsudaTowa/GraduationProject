//=============================================
//
//ゲームプレイヤー[game_player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_player.h"
#include "active_scene_manager.h"
#include "hand.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveScenePlayer::CActiveScenePlayer(int nPriority):CPlayer(nPriority)//,
//m_pHand(nullptr)												// 手札初期化
{
}

//=============================================
// デストラクタ
//=============================================
My::CActiveScenePlayer::~CActiveScenePlayer()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CActiveScenePlayer::Init()
{
	//親クラスの初期化実行
	CPlayer::Init();

	ChangeState(new CPlayerLobbyState(this));

	//ゲームのマネージャに自分を代入
	CActiveSceneManager::GetInstance()->SetPlayer(this);

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CActiveScenePlayer::Uninit()
{
	/*if (m_pHand != nullptr)
	{
		delete m_pHand;
		m_pHand = nullptr;
	}*/

	// ゲームマネージャーに登録されているプレイヤーにnullptrを代入してあげる
	if (CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
	{
		CActiveSceneManager::GetInstance()->SetPlayer(nullptr);
	}
	//親クラスの終了処理
	CPlayer::Uninit();

}

//=============================================
// 更新
//=============================================
void My::CActiveScenePlayer::Update()
{
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

	//親クラスの更新
	CPlayer::Update();
}

//=============================================
// 描画
//=============================================
void My::CActiveScenePlayer::Draw()
{
	//親クラスの描画
	CPlayer::Draw();
}

//=============================================
// 入力
//=============================================
void My::CActiveScenePlayer::Input()
{
}

//=============================================
//プレイヤーのデバッグ表示
//=============================================
void My::CActiveScenePlayer::Debug()
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