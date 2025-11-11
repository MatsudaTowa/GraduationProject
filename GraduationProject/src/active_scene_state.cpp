//=============================================
//
//ゲームのステートパターン[active_scene_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_state.h"
#include "active_scene.h"
#include "active_scene_manager.h"
#include "game_field.h"
#include "energy_charge.h"
#include "energy_gauge.h"
#include "enemy_state.h"
#include "raknet.h"
#include "card_manager.h"

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
	m_characterIdx = -1;
}

//=============================================
// ロビー
//=============================================
void My::CLobby::Lobby(CActiveScene* game)
{
	std::list<CEnemy*> enemy = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	bool retflag;

	//TODO:フォントは要修正
	//const wchar_t* text_000 = L"ワロタ";
	//const wchar_t* text_001 = L"うおｗ";


	//
	//GET_FONT_MANAGER->Regist(text_001, { 100.0f,300.0f,0.0f }, 50.0f, 80, 0, 6);

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

		if (CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
		{
			CActiveSceneManager::GetInstance()->GetPlayer()->ChangeState(new CPlayerDuelState);
		}
		//地面生成
		CField::Create(VEC3_RESET_ZERO, { FIELD_SIZE,0.0f,FIELD_SIZE }, new CGameField);

		//エナジーUI枠表示
		CEnergy_Gauge::CreateEnergy();

		CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
	}

	//オンラインの対戦開始処理
	OnlineChangeToDuel();
}

//=============================================
// 足りていないプレイヤーを生成
//=============================================
void My::CLobby::FillEmptyPlayer(int& total, std::list<My::CEnemy*>& enemy)
{
	while (total < NUM_PLAYER)
	{
		if (CActiveSceneManager::GetInstance()->GetPlayer() == nullptr)
		{
			++m_characterIdx;
			CPlayer::Create(new CActiveScenePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
			continue;
		}
		++m_characterIdx;
		CEnemy::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		enemy = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();
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
			if (CActiveSceneManager::GetInstance()->GetPlayer() != nullptr) { return; }
			++m_characterIdx;

			CPlayer::Create(new CActiveScenePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		}
	}
	else if (pKeyboard->GetTrigger(DIK_2))
	{
		if (CActiveSceneManager::GetInstance()->GetPlayer() != nullptr) { return; }
		++m_characterIdx;

		CPlayer::Create(new CActiveScenePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
	}
}

//=============================================
//通信処理
//=============================================
void My::CLobby::Connect(CActiveScene* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}

//=============================================
//オンライン時に行う処理
//=============================================
void My::CLobby::Online()
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//デュエルシーンに切り替える
	OnlineChangeToDuel();
}

//=============================================
//オンラインで敵を追加する処理
//=============================================
void My::CLobby::OnlineAddEnemy()
{
	//弾く条件
	if (CActiveSceneManager::GetInstance()->GetPlayer() == nullptr) return;			//プレイヤーが生成されているか
	if (CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() != 0) return;	//プレイヤーが１人目じゃないなら飛ばす
}

//=============================================
//オンラインでデュエルシーンに切り替える処理
//=============================================
void My::CLobby::OnlineChangeToDuel()
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//弾く条件
	if (CActiveSceneManager::GetInstance()->GetPlayer() == nullptr) return;			//プレイヤーが生成されているか

	//バトルを開始する合図が出ていたら状態を対戦に変更
	if (m_isBattle == true)
	{
		//敵の状態を変更
		std::list<CEnemy*> enemy = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();

		//敵の数だけ周回
		for (auto& itr : enemy)
		{
			if (itr == nullptr)
			{
				continue;
			}
			itr->ChangeState(new CEnemyDuelState);
		}

		//プレイヤーの状態を変更
		CActiveSceneManager::GetInstance()->GetPlayer()->ChangeState(new CPlayerDuelState);

		//地面生成
		CField::Create(VEC3_RESET_ZERO, { FIELD_SIZE,0.0f,FIELD_SIZE }, new CGameField);

		//エナジーUI枠表示
		CEnergy_Gauge::CreateEnergy();

		//ゲームの状態を変更
		CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
		return;
	}

	if (CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() != 0) return;	//プレイヤーが１人目じゃないなら飛ばす

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	//Mキーでデュエルシーンに切り替え
	if (pKeyboard->GetTrigger(DIK_M))
	{
		//サーバーに対戦開始の合図を送る
		CRakNet::GetInstance()->SendStartSign();
	}
}

//=============================================
// コンストラクタ
//=============================================
My::CDuel::CDuel()
{
	GET_CAMERA(GET_CAMERA_IDX)->ChangeCameraState(new CBirdView);
	GET_CAMERA(GET_CAMERA_IDX)->SetCamera();

	//std::list<CCard*> card_list = CCardManager::GetInstance()->GetUseCardList();
	//for (auto& itr : card_list)
	//{
	//	if (itr == nullptr) { continue; }
	//	const char* c = itr->GetBaseStatus().name.c_str();
	//	wchar_t* text_000 = NULL;
	//	mbstowcs(text_000, c, sizeof(c));
	//	GET_FONT_MANAGER->Regist(text_000, { 100.0f,500.0f,0.0f }, 50.0f, 80, 0, 5);
	//	return;
	//}
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
void My::CDuel::Duel(CActiveScene* game)
{
	//オブジェクトのアップデートを許可する
	game->StopObject(false);

	ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);

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
		CActiveSceneManager::GetInstance()->ChangeState(new CCardCast);
	}
#endif

	//ポーズ移行
	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CActiveSceneManager::GetInstance()->ChangeState(new CPause);
		return;
	}
}

//=============================================
// 時計回りに配置
//=============================================
void My::CDuel::ArrangePlayerClockwise(const D3DXVECTOR3 center, float radius)
{
	// プレイヤーを中心に配置
	CPlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
	if (player == nullptr)
	{
		return;
	}
	// 敵一覧を取得
	std::list<CEnemy*> enemy = CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();

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
void My::CDuel::Connect(CActiveScene* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//自身のステータスを送る
	CRakNet::GetInstance()->SendMyStatus();

	//全体との更新を合わせる
	while (!CRakNet::GetInstance()->GetIsUpdate())
	{
		//通信処理
		CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
	}
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
void My::CPause::Pause(CActiveScene* game)
{
	//オブジェクトのアップデートを止める
	game->StopObject(true);

	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;

	if (pKeyboard->GetTrigger(DIK_P) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
		return;
	}
}

//=============================================
//通信処理
//=============================================
void My::CPause::Connect(CActiveScene* /*game*/)
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
	CAreaManager* area_manager = CActiveSceneManager::GetInstance()->GetAreaManager();
	area_manager->SetActive(true);
}

//=============================================
// デストラクタ
//=============================================
My::CCardCast::~CCardCast()
{
	CAreaManager* area_manager = CActiveSceneManager::GetInstance()->GetAreaManager();
	if (area_manager == nullptr) { return; }
	area_manager->SetActive(false);
}

//=============================================
// カードキャスト
//=============================================
void My::CCardCast::CardCast(CActiveScene* game)
{
#ifdef _DEBUG
	//入力デバイス取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	if (pKeyboard->GetTrigger(DIK_C) && game->GetPauseKey())
	{
		game->ResetPauseCnt();
		CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
	}
#endif
}

//=============================================
//通信処理
//=============================================
void My::CCardCast::Connect(CActiveScene* /*game*/)
{
	if (!CRakNet::GetInstance()->GetOnline()) return;

	//通信処理
	CRakNet::GetInstance()->Communication(CRakNet::GetInstance()->GetPeer());
}