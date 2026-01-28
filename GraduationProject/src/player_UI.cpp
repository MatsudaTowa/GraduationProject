//=============================================
//
//プレイヤーのUI TODO:プレイヤー関連全てのUIをここにまとめて「player_UI.cpp」
// Author松田永久
//
//=============================================
#include "player_UI.h"
#include "active_scene_player_state.h"
#include "enemy_state.h"
#include "active_scene_manager.h"
#include "zone_manager.h"
//=============================================
// コンストラクタ
//=============================================
My::CPlayerUI::CPlayerUI():m_pNumberUI(nullptr),
m_pCemeteryButton(nullptr),
m_pEneryUI(nullptr),
m_pLifeUI(nullptr),
m_pDeckDrawButton(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CPlayerUI::~CPlayerUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CPlayerUI::Init(CActiveSceneCharacter* character)
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換

	if (m_pNumberUI == nullptr)
	{
		//D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換
		m_pNumberUI = CPlayerNumberUI::Create(screen_pos, character->GetPlayerIdx());
	}

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CPlayerUI::Uninit()
{
	if (m_pCemeteryButton != nullptr)
	{
		m_pCemeteryButton->Uninit();
		m_pCemeteryButton = nullptr;
	}
	if (m_pCemeteryNum != nullptr)
	{
		m_pCemeteryNum->Uninit();
		m_pCemeteryNum = nullptr;
	}
	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->Uninit();
		m_pNumberUI = nullptr;
	}
	if (m_pLifeUI != nullptr)
	{
		m_pLifeUI->Uninit();
		m_pLifeUI = nullptr;
	}
	if (m_pEneryUI != nullptr)
	{
		m_pEneryUI->Uninit();
		m_pEneryUI = nullptr;
	}
}

//=============================================
// 更新
//=============================================
void My::CPlayerUI::Update()
{
}

//=============================================
// 現在のUI設定
//=============================================
void My::CPlayerUI::SetCurrentCharacter_UI(D3DXVECTOR3 screen_pos, CActiveSceneCharacter* character)
{
	//-Author.Umeda begin--

	// プレイヤーを取得
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();

	//-Author.Umeda --end

	if (m_pNumberUI != nullptr)
	{
		m_pNumberUI->SetPos({screen_pos.x,screen_pos.y - 100.0f,screen_pos.z});
	}
	if (m_pLifeUI != nullptr)
	{
		int life = character->GetLife();
		if (life > CActiveSceneCharacter::MAX_LIFE)
		{
			character->SetLife(CActiveSceneCharacter::MAX_LIFE);
		}
		if (life < INT_ZERO)
		{
			character->SetLife(INT_ZERO);
		}
		m_pLifeUI->SetLifeNumber(life);

		// スクリーン座標に数字を描画
		int i = INT_ZERO;

		for (auto& itr : m_pLifeUI->GetNumVector())
		{
			if (itr == nullptr) { continue; }

			if (player == character) 
			{
				D3DXVECTOR3 life_pos = { 145.0f,600.0f,0.0f };

				// TODO: 30.0fは桁ずらし値 取得できるように変更予定
				itr->SetPos({ life_pos.x - (i * 30.0f),life_pos.y,life_pos.z });
			}
			else
			{
				// TODO: 30.0fは桁ずらし値 取得できるように変更予定
				itr->SetPos({ screen_pos.x - (i * 30.0f),screen_pos.y,screen_pos.z });
			}

			//桁ずらす
			++i;
		}
	}
	if (m_pEneryUI != nullptr)
	{
		int energy = character->GetEnergy();
		if (energy > CActiveSceneCharacter::MAX_ENERGY)
		{
			character->SetEnergy(CActiveSceneCharacter::MAX_ENERGY);
		}
		if (energy < INT_ZERO)
		{
			character->SetEnergy(INT_ZERO);
		}
		m_pEneryUI->SetEnergyNumber(energy);

		// スクリーン座標に数字を描画
		int i = INT_ZERO;

		for (auto& itr : m_pEneryUI->GetNumVector())
		{
			// いくつずらすか
			D3DXVECTOR2 ShiftPos = VEC2_RESET_ZERO;

			// プレイヤーと他の敵でずらす位置を変える
			if (player == character) 
			{
				ShiftPos = { -530.0f,-220.0f };
			}
			else 
			{
				ShiftPos = { 100.0f,0.0f };
			}

			if (itr == nullptr) { continue; }

			// TODO: 30.0fは桁ずらし値 取得できるように変更予定
			// Author.Umeda |-> TODO : 2025/11/27 直値でずらしたよ(*/ω＼*)
			itr->SetPos({ screen_pos.x + ShiftPos.x - (i * 30.0f),screen_pos.y + ShiftPos.y,screen_pos.z });

			//桁ずらす
			++i;
		}
	}
	if (m_pCemeteryButton != nullptr)
	{
		// いくつずらすか
		D3DXVECTOR2 ShiftPos = VEC2_RESET_ZERO;
		// プレイヤーと他の敵でずらす位置を変える TODO:いるエリアに応じてずらしたほうが綺麗
		if (player == character)
		{
			D3DXVECTOR2 ShiftSize{ 38.0f,38.0f };
			m_pCemeteryButton->SetSize(ShiftSize);
			ShiftPos = { -400.0f,-100.0f };
		}
		else
		{
			ShiftPos = { -75.0f,-80.0f };
		}

		m_pCemeteryButton->SetPos({ screen_pos.x + ShiftPos.x,screen_pos.y + ShiftPos.y,screen_pos.z });
		m_pCemeteryButton->ProcessMouseEvent();
	}
	if (m_pCemeteryNum == nullptr)
	{
		return;
	}

	int cemetery_size = 0;
	D3DXVECTOR3 basePos = VEC3_RESET_ZERO;

	bool isDraw;
	if (player == character)
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(player->GetState());
		cemetery_size = duel_state->GetZoneManager()->GetCemetery()->GetList().size();

		basePos = { 248.0f, 580.0f, 0.0f };

		//墓地を見てないときに数字を表示する
		isDraw = !duel_state->GetIsCemeteryView();
	}
	else
	{
		CEnemyDuelState* duel_state =dynamic_cast<CEnemyDuelState*>(character->GetState());
		cemetery_size = duel_state->GetZoneManager()->GetCemetery()->GetList().size();

		// いくつずらすか
		D3DXVECTOR2 ShiftPos = VEC2_RESET_ZERO;
		ShiftPos = { -70.0f,-80.0f };

		basePos = { screen_pos.x + ShiftPos.x,screen_pos.y + ShiftPos.y,screen_pos.z };

		//墓地を見てないときに数字を表示する
		isDraw = !duel_state->GetIsCemeteryView();
	}

	// 数字設定
	m_pCemeteryNum->SetNumber(cemetery_size);
	m_pCemeteryNum->SetNumPos(basePos);

	// 桁ずらし描画
	int i = 0;
	for (auto& itr : m_pCemeteryNum->GetNumVector())
	{
		itr->SetPos({basePos.x - (i * 20.0f),basePos.y,basePos.z});
		itr->SetisDraw(isDraw);
		++i;
	}
}

//=============================================
// リセット
//=============================================
void My::CPlayerUI::Reset()
{
}
