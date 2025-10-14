//=============================================
//
//アクティブシーンキャラクター処理[active_scene_character.cpp]
//Author Matsuda Towa
//
//=============================================
#include "active_scene_character.h"

//=============================================
// コンストラクタ
//=============================================
My::CActiveSceneCharacter::CActiveSceneCharacter(int nPriority):CCharacter(nPriority),
m_status(),
m_pEneryUI(nullptr),
m_pLifeUI(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CActiveSceneCharacter::~CActiveSceneCharacter()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CActiveSceneCharacter::Init()
{
	//ステータス設定
	m_status.deckSize = START_DECK;
	m_status.hand = START_HAND;
	m_status.life = START_LIFE;
	m_status.energy = START_ENERGY;
	m_status.trash = INT_ZERO;

	CCharacter::Init();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CActiveSceneCharacter::Uninit()
{
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

	CCharacter::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CActiveSceneCharacter::Update()
{
	if (m_pLifeUI != nullptr)
	{
		if (m_status.life > MAX_LIFE)
		{
			SetLife(MAX_LIFE);
		}
		if (m_status.life < INT_ZERO)
		{
			SetLife(INT_ZERO);
		}
		m_pLifeUI->SetLifeNumber(m_status.life);
	}
	if (m_pEneryUI != nullptr)
	{
		if (m_status.energy > MAX_ENERGY)
		{
			SetEnergy(MAX_LIFE);
		}
		if (m_status.energy < INT_ZERO)
		{
			SetEnergy(INT_ZERO);
		}
		m_pEneryUI->SetEnergyNumber(m_status.energy);
	}
	CCharacter::Update();
}

//=============================================
// 描画
//=============================================
void My::CActiveSceneCharacter::Draw()
{
	CCharacter::Draw();
}
