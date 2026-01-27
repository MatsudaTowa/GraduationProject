//=============================================
//
//オフラインで開始時のカウントダウンを実行するクラス[countdown_start_offline.h]
//Author Shimadu Yuto
//
//=============================================

//インクルード
#include "countdown_start_offline.h"
#include "duel_manager.h"

//=============================================
//コンストラクタ
//=============================================
My::CCountdown_Start_Offline::CCountdown_Start_Offline()
{
	//開始時間と開始設定
	SetCountStartTime(0);
	SetIsStartCountdown(true);

	//対戦時のタイマーを開始
	My::CDuel_Manager::GetInstance()->GetDuelTimer().Start();
}

//=============================================
//初期化
//=============================================
void My::CCountdown_Start_Offline::Init()
{
	
}