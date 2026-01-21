//================================
//
// デュエルタイマー用のヘッダー[duel_timer.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_TIMER_H_
#define _DUEL_TIMER_H_

//ヘッダーのインクルード
#include "main.h"
#include "GetTime.h"
#include <chrono>

//デュエルマネージャークラス
class CDuel_Timer
{
public:

	//関数
	~CDuel_Timer();			//デストラクタ
	CDuel_Timer();			//コンストラクタ
	void Reset();			//タイマーのリセット
	void Update();			//更新
	float GetdeltaTime() { return m_deltaTime; }		//デルタタイムの取得
	std::chrono::high_resolution_clock::time_point GetStartTime() { return m_StartTime; }		//対戦開始時間の取得
	std::chrono::high_resolution_clock::time_point GetElapsedTime() { return m_ElapsedTime; }	//経過時間の取得
	void Start();			//開始
	void Stop();			//停止
	void SetStartTime(std::chrono::high_resolution_clock::time_point time) { m_StartTime = time; }	//開始時間の設定

	//デルタタイムを反映した値を返す
	template<typename T>
	float GetDeltaTimeValue(T value)
	{
		return dynamic_cast<float>(value * dynamic_cast<float>((m_deltaTime * 0.001f)));
	}

private:

	//変数
	std::chrono::high_resolution_clock::time_point m_StartTime;		//開始時間
	std::chrono::high_resolution_clock::time_point m_ElapsedTime;	//経過時間
	std::chrono::high_resolution_clock::time_point m_OldElapsedTime;//前回の経過時間
	float m_deltaTime;				//デルタタイム
	bool m_isUpdate;				//更新フラグ
};

#endif