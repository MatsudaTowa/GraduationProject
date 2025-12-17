//================================
//
//duel_manager.cppに必要な宣言[duel_manager.h]
//Author:松田永久
// 
//================================

#ifndef _DUEL_MANAGER_H_ //このマクロ定義がされていなかったら
#define _DUEL_MANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "duel_timer.h"

namespace My
{
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
		~CDuel_Manager() {}							//デストラクタ

		//シングルトン
		static CDuel_Manager* GetInstance()
		{
			static CDuel_Manager instance; //静的インスタンス
			return &instance;
		}

		//設定と取得
		DuelRuleParam& GetDuelRuleParam() { return m_DuelRuleParam; }	//対戦時のルールを取得
		CDuel_Timer& GetDuelTimer() { return m_DuelTimer; }				//対戦時のタイマー

	private:

		//関数
		CDuel_Manager() : m_DuelRuleParam(), m_DuelTimer() {}		//コンストラクタ

		//変数
		DuelRuleParam m_DuelRuleParam;				//対戦のルールパラメータ
		CDuel_Timer m_DuelTimer;					//対戦時に扱うタイマー
	};
}

#endif