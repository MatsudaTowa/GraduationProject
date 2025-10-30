//=============================================
//
//プレイヤーのUI TODO:プレイヤー関連全てのUIをここにまとめて「player_UI.cpp」
// Author松田永久
//
//=============================================
#include "player_UI.h"

//=============================================
// コンストラクタ
//=============================================
My::CPlayerUI::CPlayerUI():m_pNumberUI(nullptr)
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
		D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換
		m_pNumberUI = CPlayerNumberUI::Create(screen_pos, character->GetPlayerIdx());
	}

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CPlayerUI::Uninit()
{
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
void My::CPlayerUI::SetCurrentPlayer_UI(D3DXVECTOR3 screen_pos, CActiveSceneCharacter* character)
{
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

			// TODO: 30.0fは桁ずらし値 取得できるように変更予定
			itr->SetPos({ screen_pos.x - (i * 30.0f),screen_pos.y,screen_pos.z });

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
			if (itr == nullptr) { continue; }

			// TODO: 30.0fは桁ずらし値 取得できるように変更予定
			itr->SetPos({ screen_pos.x + 100.0f - (i * 30.0f),screen_pos.y,screen_pos.z });

			//桁ずらす
			++i;
		}
	}

}

//=============================================
// リセット
//=============================================
void My::CPlayerUI::Reset()
{
}
