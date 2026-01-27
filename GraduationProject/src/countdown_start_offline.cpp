//=============================================
//
//オフラインで開始時のカウントダウンを実行するクラス[countdown_start_offline.h]
//Author Shimadu Yuto
//
//=============================================

//インクルード
#include "countdown_start_offline.h"

//=============================================
//コンストラクタ
//=============================================
My::CCountdown_Start_Offline::CCountdown_Start_Offline()
{
	
}

//=============================================
//初期化
//=============================================
void My::CCountdown_Start_Offline::Init()
{
	//開始時間と開始設定
	SetCountStartTime(0);
	SetIsStartCountdown(true);
}