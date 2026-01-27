//=============================================
//
//zoneのUIマネージャー
// Author松田永久
//
//=============================================
#include "zone_num_UI_manager.h"
#include "active_scene_manager.h"
#include "zone_manager.h"
//=============================================
// コンストラクタ
//=============================================
My::CZoneNumUIManager::CZoneNumUIManager():m_pCemeteryUI(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CZoneNumUIManager::~CZoneNumUIManager()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CZoneNumUIManager::Init(CActiveSceneCharacter* character)
{
	if (m_pCemeteryUI == nullptr)
	{
		m_pCemeteryUI = new CCemeteryNumUI;
		m_pCemeteryUI->Init();
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CZoneNumUIManager::Uninit()
{
	if (m_pCemeteryUI != nullptr)
	{
		m_pCemeteryUI->Uninit();
		m_pCemeteryUI = nullptr;
	}
}

//=============================================
// 
//=============================================
void My::CZoneNumUIManager::SetCurrentCharacter_UI(D3DXVECTOR3 screen_pos, CActiveSceneCharacter* character)
{
	// プレイヤーを取得
	CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();

	if (m_pCemeteryUI != nullptr)
	{
		CActiveSceneCharacterState* state = player->GetState();
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		int cemetery_size = duel_state->GetZoneManager()->GetCemetery()->GetList().size();
		m_pCemeteryUI->SetNumber(cemetery_size);

		// スクリーン座標に数字を描画
		int i = INT_ZERO;
		for (auto& itr : m_pCemeteryUI->GetNumVector())
		{
			if (itr == nullptr) { continue; }

			if (player == character)
			{
				D3DXVECTOR3 life_pos = { 300.0f,600.0f,0.0f };

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
}
