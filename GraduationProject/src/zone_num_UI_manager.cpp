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
My::CZoneNumUIManager::CZoneNumUIManager():
	m_pCemeteryUI(nullptr),
	m_pZoneNumBG(nullptr)
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
	if (m_pZoneNumBG == nullptr)
	{
		m_pZoneNumBG = CZoneNumUI_BG::Create(VEC3_RESET_ZERO, { 80.0f,40.0f });
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CZoneNumUIManager::Uninit()
{
	if (m_pZoneNumBG != nullptr)
	{
		m_pZoneNumBG->Uninit();
		m_pZoneNumBG = nullptr;
	}
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
		D3DXVECTOR3 life_pos = { 1170.0f,630.0f,0.0f };
		m_pCemeteryUI->SetNumPos(life_pos);
		// スクリーン座標に数字を描画
		int i = INT_ZERO;
		for (auto& itr : m_pCemeteryUI->GetNumVector())
		{
			if (itr == nullptr) { continue; }

			if (player == character)
			{
				// TODO: 30.0fは桁ずらし値 取得できるように変更予定
				itr->SetPos({ life_pos.x - (i * 12.0f),life_pos.y,life_pos.z });
			}
			else
			{
				// TODO: 30.0fは桁ずらし値 取得できるように変更予定
				itr->SetPos({ screen_pos.x - (i * 12.0f),screen_pos.y,screen_pos.z });
			}

			//桁ずらす
			++i;
		}
	}
	if (m_pZoneNumBG != nullptr)
	{
		m_pZoneNumBG->SetPos({ 1190.0f,650.0f,0.0f });
	}
}
