//=============================================
//
//エネミー[enemy.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy.h"
#include "active_manager.h"
#include "enemy_state.h"

//=============================================
// コンストラクタ
//=============================================
My::CEnemy::CEnemy(int nPriority)
{
	//マネージャーに登録
	CGameManager::GetInstance()->GetEnemyManager()->Regist(this);
}

//=============================================
// デストラクタ
//=============================================
My::CEnemy::~CEnemy()
{
	//マネージャーから削除
	if (CGameManager::GetInstance()->GetEnemyManager() != nullptr)
	{
		CGameManager::GetInstance()->GetEnemyManager()->Erase(this);
	}
}

//=============================================
// 初期化
//=============================================
HRESULT My::CEnemy::Init()
{
	if (GetState() == nullptr)
	{
		ChangeState(new CEnemyLobbyState);
	}

	CActiveSceneCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //パーツ読み込み

	//初期モーション設定
	SetMotion(MOTION_NEUTRAL);

	Motion(); //モーション処理

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CEnemy::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CActiveSceneCharacter::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CEnemy::Update()
{
	CActiveSceneCharacter::Update();

	Motion(); //モーション処理
}

//=============================================
// 描画
//=============================================
void My::CEnemy::Draw()
{
	//親クラスのモーション用の描画を呼ぶ
	MotionDraw();
}

//=============================================
// 生成
//=============================================
My::CEnemy* My::CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int id)
{
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) { return nullptr; }
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetPlayerIdx(id);
	pEnemy->Init();
	return pEnemy;
}
