//=============================================
//
//ゲームマネージャー[activemaneger.cpp]
//Auther MatsudaTowa
//
//=============================================
#include "active_scene_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneManager::CActiveSceneManager():
m_pState(nullptr),			//ゲームのステート
m_pEnemyManager(),			//エネミーマネージャー
m_pAreaManager(),			//エリアマネージャー
m_pPlayer(nullptr),			//プレイヤーのポインタ
m_pField(nullptr),			//床のポインタ
m_isFinish(false)
{
}

//=============================================
// デストラクタ
//=============================================
My::CActiveSceneManager::~CActiveSceneManager()
{
}

//=============================================
// 終了
//=============================================
void My::CActiveSceneManager::Uninit()
{
	if (m_pAreaManager != nullptr)
	{
		m_pAreaManager->Uninit();
		delete m_pAreaManager;
		m_pAreaManager = nullptr;
	}
	//エネミーマネージャーの破棄
	if (m_pEnemyManager != nullptr)
	{
		//終了処理後に開放
		m_pEnemyManager->Uninit();
		delete m_pEnemyManager;
		m_pEnemyManager = nullptr;
	}
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
void My::CActiveSceneManager::ChangeState(CActiveSceneState* state)
{
	CActiveSceneState* current_state = CActiveSceneManager::GetInstance()->GetState();
	//今のステートを消し引数のステートに切り替える
	if (current_state != nullptr)
	{
		if (typeid(*current_state) == typeid(*state))
		{//現在と同じだったら関数を抜ける
			return;
		}
		delete current_state;

		current_state = state;
		CActiveSceneManager::GetInstance()->SetState(current_state);
	}
}

//=============================================
//キャラクターのリストを返す
//=============================================
std::list<My::CActiveSceneCharacter*> My::CActiveSceneManager::GetCharacterList()
{
	//返す用のリスト
	std::list<My::CActiveSceneCharacter*> List;
	List.clear();

	//プレイヤーの追加
	List.push_back(m_pPlayer);

	//敵の追加
	for (auto& iter : m_pEnemyManager->GetList())
	{
		List.push_back(iter);
	}

	//返す
	return List;
}
