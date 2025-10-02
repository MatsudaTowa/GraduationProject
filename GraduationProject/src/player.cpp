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
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_POS = { 0.0f, 0.0f, 0.0f };

//スポーン方向
const D3DXVECTOR3 My::CPlayer::PLAYER_SPAWN_ROT = { 0.0f, 0.0f, 0.0f};

//影のサイズ
const D3DXVECTOR3 My::CPlayer::SHADOW_SIZE = { 20.0f, 0.0, 20.0f };

//通常の移動抵抗
const float My::CPlayer::DAMPING_COEFFICIENT = 0.3f;

//ベルトの補正値
const float My::CPlayer::CORRECTION_VALUE_BELT = -0.5f;

//=============================================
//コンストラクタ
//=============================================
My::CPlayer::CPlayer(int nPriority) :CCharacter(nPriority)
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
	CCharacter::Init();

	Load_Parts("data\\MOTION\\motion_Karateka.bin"); //パーツ読み込み

	//スポーン時の設定
	SetPos(CPlayer::PLAYER_SPAWN_POS);
	SetRot(CPlayer::PLAYER_SPAWN_ROT);
	SetLife(CPlayer::PLAYER_LIFE);

	//移動量初期化
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ムーブ値代入
	SetMove(move);

	//初期モーション設定
	SetMotion(MOTION_NEUTRAL);

	//影のサイズ設定
	SetShadowSize(SHADOW_SIZE);
	Motion(); //モーション処理

	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CPlayer::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CCharacter::Uninit();
}

//=============================================
//更新
//=============================================
void My::CPlayer::Update()
{
	CCharacter::Update();
	
	Motion(); //モーション処理

	if (GetPos().y > KILL_Y)
	{
		return;
	}

	ReSpawn();
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
//リスポーン
//=============================================
void My::CPlayer::ReSpawn()
{
	//自分自身のpos取得
	D3DXVECTOR3 PlayerPos = GetPos();

	//スポーン時の設定にもどす
	SetPos(CPlayer::PLAYER_SPAWN_POS);
	SetMove({0.0f,0.0f,0.0f});
	SetRot(CPlayer::PLAYER_SPAWN_ROT);
	SetLife(CPlayer::PLAYER_LIFE);
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

	sprintf(&aStr[0], "\n\n[player]\npos:%.1f,%.1f,%.1f\nrot:%.1f,%.1f,%.1f\nmove:%.1f,%.1f,%.1f"
		, GetPos().x, GetPos().y, GetPos().z, GetRot().x, GetRot().y, GetRot().z, GetMove().x, GetMove().y, GetMove().z);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
