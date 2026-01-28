//=============================================
//
//開始時のカウントダウンを実行するクラス[countdown_start.h]
//Author Shimadu Yuto
//
//=============================================

//インクルード
#include "countdown_start.h"
#include "duel_manager.h"
#include "countdown_UI_factory.h"

//=============================================
//経過時間の更新
//=============================================
void My::CCountdown_Start::Update()
{
	//合図があったら更新
	if (My::CDuel_Manager::GetInstance()->GetIsStartBattle()) return;

	//開始の合図が出ていたら更新
	if (!m_isStartCountdown) return;

	//経過時間に加算
	float fElapsedTime = m_fElapsedTime;

	if (My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime() <= 3.0f)
	{
		m_fElapsedTime += My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();
	}

	//UIの生成
	CreateUI(m_fElapsedTime, fElapsedTime);
}

//=============================================
//UIの生成
//=============================================
void My::CCountdown_Start::CreateUI(float elapsedtime, float oldelapsedtime)
{
	//現在と過去の経過時間によってカウントダウンのUIを生成
	if (!m_isCreateFirst)
	{
		//フラグを立てる
		m_isCreateFirst = true;

		//3を生成
		My::CCountdownUIFactory::GetInstance()->Create({640.0f, 360.0f, 0.0f}, CCountdownUIFactory::KIND_OF_COUNTDOWN_UI::THREE, 1.0f - m_fCountStartTime);
	}
	else if (elapsedtime >= 1.0f && oldelapsedtime <= 1.0f)	//現在の更新で1秒以上経過したか
	{
		//2を生成
		My::CCountdownUIFactory::GetInstance()->Create({ 640.0f, 360.0f, 0.0f }, CCountdownUIFactory::KIND_OF_COUNTDOWN_UI::TWO, 2.0f - elapsedtime);
	}
	else if (elapsedtime >= 2.0f && oldelapsedtime <= 2.0f)	//現在の更新で2秒以上経過したか
	{
		//1を生成
		My::CCountdownUIFactory::GetInstance()->Create({ 640.0f, 360.0f, 0.0f }, CCountdownUIFactory::KIND_OF_COUNTDOWN_UI::ONE, 3.0f - elapsedtime);
	}
	else if (elapsedtime >= 3.0f && oldelapsedtime <= 3.0f)	//現在の更新で3秒以上経過したか
	{
		//スタートUIを生成
		m_isEndCountDown = true;

		My::CCountdownUIFactory::GetInstance()->Create({ 640.0f, 360.0f, 0.0f }, CCountdownUIFactory::KIND_OF_COUNTDOWN_UI::START, 4.0f - elapsedtime);

		//対戦時のタイマーを開始
		My::CDuel_Manager::GetInstance()->GetDuelTimer().Start();
		My::CDuel_Manager::GetInstance()->SetIsStratBattle(true);
	}
}