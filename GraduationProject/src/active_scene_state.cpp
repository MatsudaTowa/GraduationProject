//=============================================
//
//ゲームのステートパターン[active_scene_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_state.h"
#include "game.h"
#include "active_manager.h"
#include "game_field.h"
#include "energy_charge.h"
#include "energy_gauge.h"
#include "enemy_state.h"
#include "raknet.h"

int My::CLobby::m_characterIdx = -1;

//=============================================
// コンストラクタ
//=============================================
My::CLobby::CLobby()
{
	GET_CAMERA(GET_CAMERA_IDX)->ChangeCameraState(new CSideView);
	GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
}

//=============================================
// デストラクタ
//=============================================
My::CLobby::~CLobby()
{
}

//=============================================
// ロビー
//=============================================
void My::CLobby::Lobby(CGame* game)
{
	std::list<CEnemy*> enemy = CGameManager::GetInstance()->GetEnemyManager()->GetList();

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	bool retflag;

	//TODO:生成は接続されたら行うカタチになる
	CreatePlayers(pKeyboard, enemy);

	if (pKeyboard->GetTrigger(DIK_RETURN) && game->GetPauseKey())
	{
		int total = (int)enemy.size() + 1; // プレイヤー含む

		//足りてないプレイヤーを生成
		FillEmptyPlayer(total, enemy);
	
		for (auto& itr : enemy)
		{
			if (itr == nullptr)
			{
				continue;
			}
			itr->ChangeState(new CEnemyDuelState);
		}
		game->ResetPauseCnt();

		if (CGameManager::GetInstance()->GetPlayer() != nullptr)
		{
			CGameManager::GetInstance()->GetPlayer()->ChangeState(new CPlayerDuelState);
		}
		//地面生成
		CField::Create(VEC3_RESET_ZERO, { FIELD_SIZE,0.0f,FIELD_SIZE }, new CGameField);

		//エナジーUI枠表示
		CEnergy_Gauge::CreateEnergy();
		
		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
}

//=============================================
// 足りていないプレイヤーを生成
//=============================================
void My::CLobby::FillEmptyPlayer(int& total, std::list<My::CEnemy*>& enemy)
{
	while (total < NUM_PLAYER)
	{
		if (CGameManager::GetInstance()->GetPlayer() == nullptr)
		{
			++m_characterIdx;
			CPlayer::Create(new CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
			continue;
		}
		++m_characterIdx;
		CEnemy::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		enemy = CGameManager::GetInstance()->GetEnemyManager()->GetList();
		total = (int)enemy.size() + 1; // プレイヤー含む
	}
}

//=============================================
// プレイヤー生成
//=============================================
void My::CLobby::CreatePlayers(My::CInputKeyboard* pKeyboard, std::list<My::CEnemy*>& enemy)
{
	if (pKeyboard->GetTrigger(DIK_1))
	{
		if (enemy.size() < 3)
		{
			++m_characterIdx;
			CEnemy::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		}
		else
		{
			if (CGameManager::GetInstance()->GetPlayer() != nullptr) { return; }
			++m_characterIdx;

			CPlayer::Create(new CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		}
	}
	else if (pKeyboard->GetTrigger(DIK_2))
	{
		if (CGameManager::GetInstance()->GetPlayer() != nullptr) { return; }
		++m_characterIdx;

		CPlayer::Create(new CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
	}
}

//=============================================
//通信処理
//=============================================
void My::CLobby::Connect(CGame* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}

//=============================================
// コンストラクタ
//=============================================
My::CDuel::CDuel()
{
	GET_CAMERA(GET_CAMERA_IDX)->ChangeCameraState(new CBirdView);
	GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
}

//=============================================
// デストラクタ
//=============================================
My::CDuel::~CDuel()
{
}

//=============================================
// デュエル
//=============================================
void My::CDuel::Duel(CGame* game)
{
	//オブジェクトのアップデートを許可する
	game->StopObject(false);

	ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);

	//ゲージ用チャージの更新
	CEnergy_Charge* pCharge = CEnergy_Charge::GetInstance();
	pCharge->Update();

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_RETURN) && game->GetPauseKey())
	{
		GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
	}
#ifdef _DEBUG
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CCardCast);
	}
#endif

	//ポーズ移行
	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CPause);
		return;
	}
}

//=============================================
// 時計回りに配置
//=============================================
void My::CDuel::ArrangePlayerClockwise(const D3DXVECTOR3 center, float radius)
{
	// プレイヤーを中心に配置
	CPlayer* player = CGameManager::GetInstance()->GetPlayer();
	if (player == nullptr)
	{
		return;
	}
	// 敵一覧を取得
	std::list<CEnemy*> enemy = CGameManager::GetInstance()->GetEnemyManager()->GetList();

	int total = (int)enemy.size() + 1; // プレイヤー含む

	if (total < 0)
	{
		return;
	}

	// プレイヤーを下に固定
	float baseAngle = D3DX_PI;
	float step = -D3DX_PI * 2.0f / total; // 時計回り

	float angle = baseAngle;
	D3DXVECTOR3 pos;
	pos.x = radius * sinf(angle);
	pos.y = 0.0f;
	pos.z = radius * cosf(angle);

	player->SetPos(pos);
	player->SetArea(CInputMouse::AREA::DOWN);

	// 中心を向く
	D3DXVECTOR3 dir = center - pos;
	float rotY = atan2f(-dir.x, -dir.z); // 中心方向
	player->SetRot({ 0.0f, rotY, 0.0f });

	int i = 0;
	for (auto& itr : enemy)
	{
		float angle = baseAngle + step * (i + 1);
		D3DXVECTOR3 pos;
		pos.x = radius * sinf(angle);
		pos.y = 0.0f;
		pos.z = radius * cosf(angle);


		// 中心方向を向く
		D3DXVECTOR3 dir = center - pos;
		float rotY = atan2f(-dir.x, -dir.z);

		itr->SetPos(pos);
		itr->SetRot({ 0.0f, rotY, 0.0f });

		itr->SetArea(CharacterArea(angle));
		++i;
	}
}

//=============================================
// キャラクターのエリアを判断
//=============================================
My::CInputMouse::AREA My::CDuel::CharacterArea(float angle)
{
	// 基準（Z+が前＝UP、Z-が下＝DOWN）と対応づけ
	if (angle >= -D3DX_PI * 0.25f && angle < D3DX_PI * 0.25f)
	{
		return CInputMouse::AREA::UP;
	}
	else if (angle >= D3DX_PI * 0.25f && angle < D3DX_PI * 0.75f)
	{
		return CInputMouse::AREA::RIGHT;
	}
	else if (angle >= -D3DX_PI * 0.75f && angle < -D3DX_PI * 0.25f)
	{
		return CInputMouse::AREA::LEFT;
	}
	else
	{
		return CInputMouse::AREA::DOWN;
	}
}

//=============================================
//通信処理
//=============================================
void My::CDuel::Connect(CGame* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}

//=============================================
// デストラクタ
//=============================================
My::CPause::~CPause()
{
}

//=============================================
// ポーズ
//=============================================
void My::CPause::Pause(CGame* game)
{
	//オブジェクトのアップデートを止める
	game->StopObject(true);

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CDuel);
		return;
	}
}

//=============================================
//通信処理
//=============================================
void My::CPause::Connect(CGame* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}

//=============================================
// コンストラクタ
//=============================================
My::CCardCast::CCardCast()
{
	CAreaManager* area_manager = CGameManager::GetInstance()->GetAreaManager();
	area_manager->SetActive(true);
}

//=============================================
// デストラクタ
//=============================================
My::CCardCast::~CCardCast()
{
	CAreaManager* area_manager = CGameManager::GetInstance()->GetAreaManager();
	if (area_manager == nullptr) { return; }
	area_manager->SetActive(false);
}

//=============================================
// カードキャスト
//=============================================
void My::CCardCast::CardCast(CGame* game)
{
	CGameManager::GetInstance()->GetAreaManager()->SelectArea();
#ifdef _DEBUG
	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
#endif
}

//=============================================
//通信処理
//=============================================
void My::CCardCast::Connect(CGame* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}