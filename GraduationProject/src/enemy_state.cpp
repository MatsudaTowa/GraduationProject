//=============================================
//
//エネミーのステートパターン[enemy_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "enemy_state.h"
#include "active_scene_manager.h"
#include "enemy.h"
#include "wait_zone.h"
#include "zone_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CEnemyLobbyState::CEnemyLobbyState(CActiveSceneCharacter* character):CLobbyCharacter(character)
{
}

//=============================================
// デストラクタ
//=============================================
My::CEnemyLobbyState::~CEnemyLobbyState()
{
}

//=============================================
// ロビー処理
//=============================================
void My::CEnemyLobbyState::Lobby(CActiveSceneCharacter* character)
{
	CLobbyCharacter::Lobby(character);
	if (typeid(*character) != typeid(CEnemy))
	{
		return;
	}
	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
	//モーション設定
	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);
}

//デュエルステートで使う定数
namespace
{
	const D3DXVECTOR3 FIRST_CARD_POS{ 1050.0f, 660.0f, 0.0f };
	const float CARD_SPACE{ 40.0f };
	const int WAIT_CHECKER_NUM{ 6 };
	const D3DXVECTOR2 FIRST_CHECKER_POS{ 0.0f, 60.0f};
	const float CHECKER_SPACE{ 20.0f };
}

//=============================================
// コンストラクタ
//=============================================
My::CEnemyDuelState::CEnemyDuelState(CActiveSceneCharacter* character) : CDuelCharacter(character),
	m_WaitCheckerVector()
{
	m_WaitCheckerVector.clear();

	//座標変換しずらす
	D3DXVECTOR3 pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換

	std::list<CCard*> list = GetZoneManager()->GetDeck()->GetList();


//#ifdef _DEBUG
//	for (auto& itr : list)
//	{
//		if (itr == nullptr) { continue; }
//		GetZoneManager()->MoveZone(itr, itr->CastToZone(itr->GetCurrentZone(), this), GetZoneManager()->GetCemetery(), true);
//
//		//TODO:フリッププレビューゾーンの列挙に
//		itr->SetCurrentZone(CCard::CEMETERY);
//	}
//#endif // _DEBUG


	//周回
	/*for (int i = 0; i < WAIT_CHECKER_NUM; i++)
	{
		m_WaitCheckerVector.push_back(CWaitChecker_UI::Create({ pos.x + i * CHECKER_SPACE, pos.y, 0.0f}));
	}*/
}

//=============================================
// デストラクタ
//=============================================
My::CEnemyDuelState::~CEnemyDuelState()
{
	m_WaitCheckerVector.clear();
}

//=============================================
// デュエル処理
//=============================================
void My::CEnemyDuelState::Duel(CActiveSceneCharacter* character)
{
	if (typeid(*character) != typeid(CEnemy))
	{
		return;
	}
	CEnemy* enemy = dynamic_cast<CEnemy*>(character);
	//UIが生成されてなければ生成される
	CreateDuelUI(enemy);
	//モーション設定
	enemy->SetMotion(CPlayer::MOTION_NEUTRAL);

	//親の更新
	CDuelCharacter::Duel(character);

	//周回
	if (!m_WaitCheckerVector.empty())
	{
		D3DXVECTOR3 pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), enemy->GetPos()); //スクリーン座標に変換

		for (int i = 0; i < WAIT_CHECKER_NUM; i++)
		{
			m_WaitCheckerVector[i]->SetPos({ FIRST_CHECKER_POS.x + pos.x + i * CHECKER_SPACE, FIRST_CHECKER_POS.y + pos.y, pos.z});
		}
	}
}

//=============================================
// UI設定
//=============================================
void My::CEnemyDuelState::CreateDuelUI(CEnemy* enemy)
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), enemy->GetPos()); //スクリーン座標に変換

	if (enemy->GetPlayerUI()->GetCemeteryButton() == nullptr)
	{
		CCemeteryButton* pCemeteryButton = CCemeteryButton::Create(screen_pos, enemy);
		enemy->GetPlayerUI()->SetCemeteryButton(pCemeteryButton);
	}
	if (enemy->GetPlayerUI()->GetLifeUI() == nullptr)
	{
		CLife_UI* pLifeUI = CLife_UI::Create(screen_pos);
		enemy->GetPlayerUI()->SetLifeUI(pLifeUI);
	}

	if (enemy->GetPlayerUI()->GetEnergyUI() == nullptr)
	{
		CEnergy_UI* pEnergyUI = CEnergy_UI::Create({ screen_pos.x + 100.0f,screen_pos.y,screen_pos.z });
		enemy->GetPlayerUI()->SetEnergyUI(pEnergyUI);
	}

	//周回
	if (m_WaitCheckerVector.empty())
	{
		for (int i = 0; i < WAIT_CHECKER_NUM; i++)
		{
			m_WaitCheckerVector.push_back(CWaitChecker_UI::Create({ FIRST_CHECKER_POS.x + screen_pos.x + i * CHECKER_SPACE, FIRST_CHECKER_POS.y + screen_pos.y, 0.0f }));
		}
	}
}

//=============================================
//カードを引く処理
//=============================================
void My::CEnemyDuelState::ViewWait(CWaitZone* zone)
{
	//待機ゾーンのカード枚数を取得
	int nWaitNum = zone->GetList().size();

	//周回
	if (!m_WaitCheckerVector.empty())
	{
		for (int i = 0; i < WAIT_CHECKER_NUM; i++)
		{
			//待機数以下の番号のチェッカーはtrue
			if (nWaitNum > i)
			{
				m_WaitCheckerVector[i]->SetIsWaited(true);
			}
			else
			{
				m_WaitCheckerVector[i]->SetIsWaited(false);
			}
			
		}
	}
}