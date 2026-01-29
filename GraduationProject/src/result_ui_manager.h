//=============================================
//
// リザルトUIマネージャー[result_ui_manager.h]
// Author Ueda Kou
//
//=============================================
#ifndef _RESULT_UI_MANAGER_H_
#define _RESULT_UI_MANAGER_H_
#include "result_ranking.h"	// リザルト用ランキングポリゴン

// ジェーソンに必要
#include <fstream>
#include "nlohmann/json.hpp"
using json = nlohmann::json;
namespace My {

	// 定数直値
	namespace {
		// トランスフォーム
		struct Transform
		{
			D3DXVECTOR3 pos;	// 位置
			D3DXVECTOR3 rot;	// 向き
			D3DXVECTOR2 size;	// 大きさ
		};
		// レンダートランスフォーム
		struct RenderTransform : public Transform	
		{
			D3DXCOLOR col;	//　色
		};
		// アニメーション
		struct ANIMATION_DATA
		{
			std::string	texture;	// テクスチャファイルパス
			RenderTransform start;	// 開始
			RenderTransform end;	// 終了
			int delayTime;			// 開始までの時間
			int elapsedTime;		// 終了までの時間
		};

		constexpr int MAX_RANKING_COUNT = 4;	// ランキングに乗せる人数
		const std::string ANIMATION_DATA_FILE_PATH = "data/JSON/result_ui.json";
#define Json true	// 外部ファイルで初期化をする

	}
	// リザルトUIマネージャークラス
	class CResultUIManager
	{
	public:
		CResultUIManager();
		~CResultUIManager();
		HRESULT Init ();	// 初期化処理
		void Uninit();		// 終了処理
		void Update();		// 更新処理

		void SetRanking(_In_ int nRanking[MAX_RANKING_COUNT]);	// ランキング設定

		bool IsEndEffect();	// エフェクトが終わっているか

		json LoadAnimation(const std::string& path);	// 
		void SaveAnimation(const std::string& path, const json& j);	// 

		static CResultUIManager* Create(_In_ int nRanking[MAX_RANKING_COUNT]);	// 生成処理

	private:
#if Json

		ANIMATION_DATA RANKINGS		[MAX_RANKING_COUNT];	// ランキング
		ANIMATION_DATA PLAYER_IDS	[MAX_RANKING_COUNT];		// プレイヤーID
		int MY_PLAYER_BLINK;									// 自プレイヤー強調表示間隔
		float MY_PLAYER_BLINK_LIGHT_INTENSITY ;				// 自プレイヤー強調表示光量
		ANIMATION_DATA WIN;	// 勝利出現設定
		ANIMATION_DATA LOSE;	// 敗北出現設定
#endif // !Json
		// ランキング変数
		int m_nRanking	[MAX_RANKING_COUNT];						// ランキング順位
		int m_nPlayer;												// 自プレイヤーの番号

		// 順位
		CResultRanking* m_pRankingsObject	[MAX_RANKING_COUNT];	// 順位の表示用(例１位)
		RenderTransform m_RankingsAnimation	[MAX_RANKING_COUNT];	// 順位のアニメーション情報格納
		int m_RankingsDelayTime				[MAX_RANKING_COUNT];	// 順位の出現開始時間
		int m_RankingsElapsedTime			[MAX_RANKING_COUNT];	// 順位の出現時間

		// プレイヤーID
		CResultRanking* m_pPlayerIDsObject	[MAX_RANKING_COUNT];	// プレイヤーIDの表示用(例１位)
		RenderTransform m_PlayerIDsAnimation[MAX_RANKING_COUNT];	// プレイヤーIDのアニメーション情報格納
		int m_PlayerIDsDelayTime			[MAX_RANKING_COUNT];	// プレイヤーIDの出現開始時間
		int m_PlayerIDsElapsedTime			[MAX_RANKING_COUNT];	// プレイヤーIDの出現時間

		int m_nMyPlayerBlink;		// 自プレイヤー点滅カウント
		bool m_isUpMyPlayerBlink;	// 自プレイヤー点滅カウント

		// 勝敗変数
		CResultRanking* m_pWinOrLoseObject;		// 勝敗オブジェクト
		RenderTransform m_pWinOrLoseAnimation;	// 勝敗のアニメーション情報格納
		int m_nWinOrLoseMoveDelay;				// 勝敗の出現開始時間
		int m_nWinOrLoseMoveDuration;			// 勝敗の出現時間
		bool m_isWin;							// 勝利しているか

		// 全体
		int m_nEffectCount;	// 全体のエフェクトカウント
	};
}

#endif // !_RESULT_UI_MANAGER_H_