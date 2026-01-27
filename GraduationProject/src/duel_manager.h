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
#include "card_info_UI.h"
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

		//デストラクタ
		~CDuel_Manager()
		{ 
			//カード情報UI
			if (m_CardInfoUI != nullptr) 
			{ 
				m_CardInfoUI->Uninit(); 
				m_CardInfoUI = nullptr;
			} 

			//順位のクリア
			m_RankMap.clear();
		}							//デストラクタ

		//シングルトン
		static CDuel_Manager* GetInstance()
		{
			static CDuel_Manager instance; //静的インスタンス
			return &instance;
		}

		//関数
		void Init();	//初期化

		//設定と取得
		DuelRuleParam& GetDuelRuleParam() { return m_DuelRuleParam; }		//対戦時のルールを取得
		CDuel_Timer& GetDuelTimer() { return m_DuelTimer; }					//対戦時のタイマー
		CCardInfoUI* GetCardInfoUI() { return m_CardInfoUI; }				//カード情報UIの取得
		void SetCardInfoUI(CCardInfoUI* infoUI) { m_CardInfoUI = infoUI; }	//カード情報UIの設定
		void SetRankMap(std::map<int, int> map) { m_RankMap = map; }		//順位マップの設定
		std::map<int, int>& GetRankMap() { return m_RankMap; }				//順位マップの取得
		void SetIsStratBattle(bool start) { m_isStartBattle = start; }		//対戦開始フラグの設定
		bool GetIsStartBattle() { return m_isStartBattle; }					//対戦開始フラグの取得

	private:

		//関数
		CDuel_Manager() : m_DuelRuleParam(), m_DuelTimer(), m_CardInfoUI(nullptr), m_RankMap(), m_isStartBattle(false){}		//コンストラクタ

		//変数
		DuelRuleParam m_DuelRuleParam;				//対戦のルールパラメータ
		CDuel_Timer m_DuelTimer;					//対戦時に扱うタイマー
		CCardInfoUI* m_CardInfoUI = nullptr;		//!<UI
		std::map<int, int> m_RankMap;				//順位のマップ(番号、順位)
		bool m_isStartBattle;						//対戦開始フラグ
	};
}

#endif