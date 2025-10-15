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

int My::CLobby::m_characterIdx = -1;

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
			ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);
			continue;
		}
		++m_characterIdx;
		CEnemy::Create(VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);
		enemy = CGameManager::GetInstance()->GetEnemyManager()->GetList();
		total = (int)enemy.size() + 1; // プレイヤー含む
									   // 配置を更新
		ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);
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
			// 配置を更新
			ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);
		}
		else
		{
			if (CGameManager::GetInstance()->GetPlayer() != nullptr) { return; }
			++m_characterIdx;

			CPlayer::Create(new CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);

			// プレイヤー生成時にも再配置
			ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);
		}
	}
	else if (pKeyboard->GetTrigger(DIK_2))
	{
		if (CGameManager::GetInstance()->GetPlayer() != nullptr) { return; }
		++m_characterIdx;

		CPlayer::Create(new CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_characterIdx);

		// プレイヤー生成時にも再配置
		ArrangePlayerClockwise(VEC3_RESET_ZERO, 200.0f);
	}
}

//=============================================
// 時計回りに配置
//=============================================
void My::CLobby::ArrangePlayerClockwise(const D3DXVECTOR3 center, float radius)
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
	CArea* area = CGameManager::GetInstance()->GetArea(CInputMouse::DOWN);
	if (area != nullptr)
	{
		area->SetCharacter(player);
	}

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

		CArea* area = CGameManager::GetInstance()->GetArea(CharacterArea(angle));
		if (area != nullptr)
		{
			area->SetCharacter(itr);
		}
		++i;
	}
}

//=============================================
// キャラクターのエリアを判断
//=============================================
My::CInputMouse::AREA My::CLobby::CharacterArea(float angle)
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
// コンストラクタ
//=============================================
My::CDuel::CDuel()
{

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
// コンストラクタ
//=============================================
My::CCardCast::CCardCast()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		CArea* area = CGameManager::GetInstance()->GetArea(i);
		if (area == nullptr) { continue; }
		area->SetActive(true);
	}
}

//=============================================
// デストラクタ
//=============================================
My::CCardCast::~CCardCast()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		CArea* area = CGameManager::GetInstance()->GetArea(i);
		if (area == nullptr) { continue; }
		area->SetActive(false);
	}
}

//=============================================
// カードキャスト
//=============================================
void My::CCardCast::CardCast(CGame* game)
{
	CGameManager::GetInstance()->SelectArea();
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

