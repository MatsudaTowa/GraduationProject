//================================
//
// デュエルマネージャー用のヘッダー[duel_manager.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_MANAGER_H_
#define _DUEL_MANAGER_H_

//ヘッダーのインクルード
#include "main.h"
#include "duel_timer.h"

//デュエルマネージャークラス
class CDuel_Manager
{
public:

	//対戦時に決められるルールの構造体
	struct DuelRuleParam
	{
		int nDuelTime;			//対戦時間
		int nNeedEnergyUpTime;	//エナジーの増加に必要な時間
		int nCardStayTime;		//カードステイ時間
		int nStartLife;			//開始時の体力
	};

	//関数
	~CDuel_Manager() 
	{
		m_RankMap.clear();
	}	//デストラクタ
	
	//シングルトン
	static CDuel_Manager* GetInstance()
	{
		static CDuel_Manager instance; //静的インスタンス
		return &instance;
	}

	//関数
	void RankReset();

	//設定と取得
	DuelRuleParam& GetDuelRuleParam() { return m_DuelRuleParam; }	//対戦時のルールを取得
	CDuel_Timer& GetDuelTimer() { return m_DuelTimer; }				//対戦時のタイマー
	std::map<int, int>& GetRankMap() { return m_RankMap; }			//順位のマップ取得
	void SetRankMap(std::map<int, int> map) { m_RankMap = map; }	//順位のマップ設定
	void SetIsStratBattle(bool start) { m_isStartBattle = start; }	//対戦開始フラグの設定
	bool GetIsStartBattle() { return m_isStartBattle; }				//対戦開始フラグの取得

private:

	//関数
	CDuel_Manager() : m_DuelRuleParam(), m_DuelTimer(), m_RankMap(), m_isStartBattle(false){}		//コンストラクタ

	//変数
	DuelRuleParam m_DuelRuleParam;				//対戦のルールパラメータ
	CDuel_Timer m_DuelTimer;					//対戦時に扱うタイマー
	std::map<int, int> m_RankMap;				//順位のマップ(番号、順位)
	bool m_isStartBattle;						//対戦開始フラグ

};

#endif