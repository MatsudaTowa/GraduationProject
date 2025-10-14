//=============================================
//
//プレイヤー[player.cpp]
//Author Matsuda Towa
//
//=============================================
#include "player.h"
#include "manager.h"
#include "input.h"
#include "field.h"
#include "camera.h"
#include "camera_state.h"

//スポーン位置
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_POS = { 0.0f, 0.0f, -200.0f };

//スポーン方向
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_ROT = { 0.0f, 3.14f, 0.0f};

//=============================================
//コンストラクタ
//=============================================
My::CPlayer::CPlayer(int nPriority) :CActiveSceneCharacter(nPriority)
{//イニシャライザーでメンバ変数初期化
}

//=============================================
//デストラクタ
//=============================================
My::CPlayer::~CPlayer()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CPlayer::Init()
{
	CActiveSceneCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //パーツ読み込み

	//スポーン時の設定
	SetPos(CPlayer::PLAYER_SPAWN_POS);
	SetRot(CPlayer::PLAYER_SPAWN_ROT);

	//初期モーション設定
	SetMotion(MOTION_NEUTRAL);

	//影のサイズ設定
	SetShadowSize({SHADOW_SIZE_X,0.0f,SHADOW_SIZE_Z});
	Motion(); //モーション処理

	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CPlayer::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CActiveSceneCharacter::Uninit();
}

//=============================================
//更新
//=============================================
void My::CPlayer::Update()
{
	CActiveSceneCharacter::Update();
	
	Motion(); //モーション処理
}

//=============================================
//描画
//=============================================
void My::CPlayer::Draw()
{
	//親クラスのモーション用の描画を呼ぶ
	MotionDraw();

	//プレイヤーのデバッグ表示
	Debug();
}

//=============================================
//生成
//=============================================
My::CPlayer* My::CPlayer::Create(CPlayer* player)
{
	CModel* pModel = GET_MODEL;

	CPlayer* pPlayer = player;

	// nullならnullを返す
	if (pPlayer == nullptr) { return nullptr; }

	pPlayer->Init(); //初期化処理

	pPlayer->SetType(OBJECT_TYPE_PLAYER); //タイプ設定

	return pPlayer;
}

//=============================================
//移動処理
//=============================================
void My::CPlayer::Input()
{
}

//=============================================
//プレイヤーの方向をカメラのほうへ
//=============================================
void My::CPlayer::ResetRot()
{
	float rotMoveY = CManager::GetInstance()->GetCamera(0)->GetRot().y;

	//親クラスからrotを取得
	D3DXVECTOR3 rot = GetRot();

	rot.y = rotMoveY + D3DX_PI;

	SetRot(rot);
}

//=============================================
//プレイヤーのデバッグ表示
//=============================================
void My::CPlayer::Debug()
{
#ifdef _DEBUG
	LPD3DXFONT pFont = CManager::GetInstance()->GetRenderer()->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
