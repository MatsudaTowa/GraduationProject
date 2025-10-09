//=============================================
//
//エネミー[enemy.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy.h"
#include "game_manager.h"

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
	CCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //パーツ読み込み

	//初期モーション設定
	SetMotion(MOTION_NEUTRAL);

	Motion(); //モーション処理

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //スクリーン座標に変換

	//if (GetLifeUI() == nullptr)
	//{
	//	CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
	//	SetLifeUI(pLifeUI);
	//}

	//if (GetEnergyUI() == nullptr)
	//{
	//	CEnergy_UI* pEnergyUI = CEnergy_UI::Create({screen_pos.x + 100.0f,screen_pos.y,screen_pos.z});
	//	SetEnergyUI(pEnergyUI);
	//}

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CEnemy::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CCharacter::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CEnemy::Update()
{
	CCharacter::Update();

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
My::CEnemy* My::CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) { return nullptr; }
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->Init();
	return pEnemy;
}
