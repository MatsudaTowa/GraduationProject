//=============================================
//
//カウントダウンUIの生成[countdown_UI_factory.cpp]
//Author Matsuda Towa
//
//=============================================
#include "countdown_UI_factory.h"

//=============================================
// コンストラクタ
//=============================================
My::CCountdownUIFactory::CCountdownUIFactory()
{
	// 生産ラインを登録
	/*m_CountDownUI[KIND_OF_COUNTDOWN_UI::ONE] = CCountdownNumberUI::Create({ 640.0f, 360.0f, 0.0f }, CCountdownNumberUI::NUMBER::ONE);
	m_CountDownUI[KIND_OF_COUNTDOWN_UI::TWO] = CCountdownNumberUI::Create({ 640.0f, 360.0f, 0.0f }, CCountdownNumberUI::NUMBER::TWO);
	m_CountDownUI[KIND_OF_COUNTDOWN_UI::THREE] = CCountdownNumberUI::Create({ 640.0f, 360.0f, 0.0f }, CCountdownNumberUI::NUMBER::THREE);
	m_CountDownUI[KIND_OF_COUNTDOWN_UI::START] = CCountdownStartUI::Create({ 640.0f, 360.0f, 0.0f });*/
}
//=============================================
// デストラクタ
//=============================================
My::CCountdownUIFactory::~CCountdownUIFactory()
{
}

//=============================================
// 生成
//=============================================
void My::CCountdownUIFactory::Create(D3DXVECTOR3 pos, KIND_OF_COUNTDOWN_UI ui)
{
	switch (ui)
	{
	case KIND_OF_COUNTDOWN_UI::ONE:
		CCountdownNumberUI::Create(pos, CCountdownNumberUI::NUMBER::ONE);
		break;

	case KIND_OF_COUNTDOWN_UI::TWO:
		CCountdownNumberUI::Create(pos, CCountdownNumberUI::NUMBER::TWO);
		break;

	case KIND_OF_COUNTDOWN_UI::THREE:
		CCountdownNumberUI::Create(pos, CCountdownNumberUI::NUMBER::THREE);
		break;

	case KIND_OF_COUNTDOWN_UI::START:
		CCountdownStartUI::Create(pos);
		break;
	}
}
