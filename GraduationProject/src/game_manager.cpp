//=============================================
//
//ゲームマネージャー[gamemaneger.cpp]
//Auther MatsudaTowa
//
//=============================================
#include "game_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CGameManager::CGameManager():
m_pState(nullptr),			//ゲームのステート
m_pEnemyManager(),			//エネミーマネージャー
m_pPlayer(nullptr),			//プレイヤーのポインタ
m_pField(nullptr),			//床のポインタ
m_isFinish(false)
{
	//エネミーマネージャーの生成
	m_pEnemyManager = new CEnemyManager();		//メモリ確保
	m_pEnemyManager->Init();					//初期設定
}

//=============================================
// デストラクタ
//=============================================
My::CGameManager::~CGameManager()
{
	//エネミーマネージャーの破棄
	if (m_pEnemyManager != nullptr)
	{
		//終了処理後に開放
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}
}

//=============================================
// 終了
//=============================================
void My::CGameManager::Uninit()
{
	if (m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Uninit();
		m_pPlayer = nullptr;
	}
	if (m_pField != nullptr)
	{
		m_pField->Uninit();
		m_pField = nullptr;
	}
}

//=============================================
//ステート変更
//=============================================
void My::CGameManager::ChangeState(CGameState* state)
{
	CGameState* current_state = CGameManager::GetInstance()->GetState();
	//今のステートを消し引数のステートに切り替える
	if (current_state != nullptr)
	{
		if (typeid(*current_state) == typeid(*state))
		{//現在と同じだったら関数を抜ける
			return;
		}
		delete current_state;

		current_state = state;
		CGameManager::GetInstance()->SetState(current_state);
	}
}