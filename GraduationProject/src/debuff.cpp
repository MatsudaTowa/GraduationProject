//=============================================
//
//デバフ[debuff.cpp]
//Author Matsuda Towa
//
//=============================================
#include "debuff.h"

//=============================================
// コンストラクタ
//=============================================
My::CDeath::CDeath():m_pDeathIcon(nullptr)
{
	SetType(DEBUFF_TYPE::DEATH);
}

//=============================================
// デストラクタ
//=============================================
My::CDeath::~CDeath()
{
	if (m_pDeathIcon != nullptr)
	{
		m_pDeathIcon->Uninit();
		m_pDeathIcon = nullptr;
	}
}

//=============================================
// デバフ
//=============================================
void My::CDeath::Debuff(CActiveSceneCharacter* character)
{
	if (m_pDeathIcon == nullptr)
	{
		D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換
		m_pDeathIcon = CDeathIcon::Create(screen_pos);
	}
}
