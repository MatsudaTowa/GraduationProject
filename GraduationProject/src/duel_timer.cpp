//================================
//
// 対戦時のタイマー用のcpp[duel_timer.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "duel_timer.h"

//=====================================
//コンストラクタ
//=====================================
CDuel_Timer::CDuel_Timer() :
	m_StartTime(),			//開始時間
	m_ElapsedTime(),		//経過時間
	m_OldElapsedTime(),		//前回の経過時間
	m_deltaTime(0.0f),		//デルタタイム
	m_isUpdate(false)		//更新フラグ
{
	//現在の時間を開始時間として保存
	m_StartTime = std::chrono::high_resolution_clock::now();
}

//=====================================
//デストラクタ
//=====================================
CDuel_Timer::~CDuel_Timer()
{

}

//=====================================
//リセット処理
//=====================================
void CDuel_Timer::Reset()
{
	//経過時間のリセット
	//m_ElapsedTime. = 0;
	//m_OldElapsedTime = 0;
}

//=====================================
//更新処理
//=====================================
void CDuel_Timer::Update()
{
	//更新フラグが無ければ返す
	if (!m_isUpdate) return;

	//経過時間を取得
	//std::chrono::high_resolution_clock::time_point CurrentTime = std::chrono::high_resolution_clock::now();;	//現在の時間を取得
	//m_OldElapsedTime = m_ElapsedTime;				//前回の経過時間を保存
	//m_ElapsedTime = CurrentTime - m_StartTime;		//経過時間を算出
	//m_deltaTime = m_ElapsedTime - m_OldElapsedTime;	//デルタタイムを算出

	//auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - m_ElapsedTime).count();
	// 現在の時間を次のループの基準時間に更新
	//m_OldElapsedTime = CurrentTime;

	auto currentTime = std::chrono::high_resolution_clock::now();
	// デルタタイムを計算 (ミリ秒単位に変換)
	auto deltaTime = currentTime - m_OldElapsedTime;
	// 現在の時間を次のループの基準時間に更新
	m_OldElapsedTime = currentTime;
	//デルタタイムをタイマーに加算
	m_deltaTime = std::chrono::duration<float>(deltaTime).count();
	// デルタタイムを利用して処理を行う
	//std::cout << "経過秒数" << timer_count << std::endl;
}

//=====================================
//開始処理
//=====================================
void CDuel_Timer::Start()
{
	//更新中なら抜ける
	if (m_isUpdate) return;

	//更新フラグを立てる
	m_isUpdate = true;

	//現在の時間を開始時間として保存
	m_StartTime = std::chrono::high_resolution_clock::now();
}

//=====================================
//停止処理
//=====================================
void CDuel_Timer::Stop()
{
	//更新されていないなら抜ける
	if (!m_isUpdate) return;

	//更新フラグを下ろす
	m_isUpdate = false;
}
