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
#define SaveJson false	// 現在の直値の情報を外部ファイルの書き出すか

#if !Json
		// ランキング
		const ANIMATION_DATA RANKINGS[MAX_RANKING_COUNT]{
			// 1位
			{
				"data\\TEXTURE\\rank_1.png", // texture
				 // start
				{{
					{2000.0f, 100.0f, 0.0f},// pos 
					{VEC3_RESET_ZERO},// rot 
					{50.0f, 50.0f}, // size 
					},
					{COLOR_WHITE},// col
				}, 
				// end
				{{
					{1000.0f, 100.0f, 0.0f},// pos
					{VEC3_RESET_ZERO},// rot
					{50.0f, 50.0f}, // size ← ここ D3DXVECTOR2 なのに VEC3_RESET_ZERO 使ってない？ 
					},
					{COLOR_WHITE}, // col
				},
				60, // delayTime 
				30, // elapsedTime
			},
			// 2位
			{
				"data\\TEXTURE\\rank_2.png",	// テクスチャファイルパス
				{{
					{2000.0f, 200.0f, 0.0f},		// 開始位置
					{VEC3_RESET_ZERO},				// 開始向き
					{50.0f, 50.0f},					// 開始大きさ
					},
					{COLOR_WHITE},					// 開始カラー
				},
				{{
					{1000.0f, 200.0f, 0.0f},		// 終了位置
					{VEC3_RESET_ZERO},				// 終了向き
					{50.0f, 50.0f},				// 終了大きさ
					},
					{COLOR_WHITE},					// 終了カラー
				},
				90,								// 開始までの時間
				30,								// 終了までの時間
			},
			// 3位
			{
				"data\\TEXTURE\\rank_3.png"	,// テクスチャファイルパス
				{{
					{2000.0f, 300.0f, 0.0f},		// 開始位置
					{VEC3_RESET_ZERO},				// 開始向き
					{50.0f, 50.0f},					// 開始大きさ
					},
					{COLOR_WHITE},					// 開始カラー
				},
				{{
					{1000.0f, 300.0f, 0.0f},		// 終了位置
					{VEC3_RESET_ZERO},				// 終了向き
					{50.0f, 50.0f},					// 終了大きさ
					},
				{COLOR_WHITE},					// 終了カラー
				},
				120,							// 開始までの時間
				30,								// 終了までの時間
			},
			// 4位
			{
				"data\\TEXTURE\\rank_4.png",	// テクスチャファイルパス
				{{
					{2000.0f, 400.0f, 0.0f},		// 開始位置
					{VEC3_RESET_ZERO},				// 開始向き
					{50.0f, 50.0f},					// 開始大きさ
					},
					{COLOR_WHITE},					// 開始カラー
				},
				{{
					{1000.0f, 400.0f, 0.0f},		// 終了位置
					{VEC3_RESET_ZERO},				// 終了向き
					{50.0f, 50.0f},					// 終了大きさ
					},
					{COLOR_WHITE},					// 終了カラー
				},
				150,							// 開始までの時間
				30,								// 終了までの時間
			}
		};
		// プレイヤーID
		const ANIMATION_DATA PLAYER_IDS[MAX_RANKING_COUNT]{
			// 1位
			{
				"data\\TEXTURE\\player_1P.png",	// テクスチャファイルパス
				{{
					{2120.0f, 100.0f, 0.0f},	// 開始位置
					{VEC3_RESET_ZERO},			// 開始向き
					{50.0f, 50.0f},				// 開始大きさ
					},
					{COLOR_WHITE},				// 開始カラー
					},
				{{
					{1120.0f, 100.0f, 0.0f},	// 終了位置
					{VEC3_RESET_ZERO},			// 終了向き
					{50.0f, 50.0f},				// 終了大きさ
					},
					{COLOR_WHITE},				// 終了カラー
				},
				60,								// 開始までの時間
				30,								// 終了までの時間
			},
			// 2位
			{
				"data\\TEXTURE\\player_2P.png",	// テクスチャファイルパス
			{{
				{2120.0f, 200.0f, 0.0f},		// 開始位置
				{VEC3_RESET_ZERO},				// 開始向き
				{50.0f, 50.0f},					// 開始大きさ
				},
				{COLOR_WHITE},					// 開始カラー
				},
			{{
				{1120.0f, 200.0f, 0.0f},		// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{50.0f, 50.0f},					// 終了大きさ
				},
				{COLOR_WHITE},					// 終了カラー
				},
				90,								// 開始までの時間
				30,								// 終了までの時間
			},
			// 3位
			{
				"data\\TEXTURE\\player_3P.png",	// テクスチャファイルパス
			{{
				{2120.0f, 300.0f, 0.0f},		// 開始位置
				{VEC3_RESET_ZERO},				// 開始向き
				{50.0f, 50.0f},					// 開始大きさ
				},
				{COLOR_WHITE},					// 開始カラー
				},
			{{
				{1120.0f, 300.0f, 0.0f},		// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{50.0f, 50.0f},					// 終了大きさ
				},
				{COLOR_WHITE},					// 終了カラー
				},
				120,							// 開始までの時間
				30,								// 終了までの時間
			},
			// 4位
			{
				"data\\TEXTURE\\player_4P.png",	// テクスチャファイルパス
			{{
				{2120.0f, 400.0f, 0.0f},		// 開始位置
				{VEC3_RESET_ZERO},				// 開始向き
				{50.0f, 50.0f},					// 開始大きさ
				},
				{COLOR_WHITE},					// 開始カラー
				},
			{{
				{1120.0f, 400.0f, 0.0f},		// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{50.0f, 50.0f},					// 終了大きさ
				},
				{COLOR_WHITE},					// 終了カラー
				},
				150,							// 開始までの時間
				30,								// 終了までの時間
			}
		};
		constexpr int MY_PLAYER_BLINK = 30;							// 自プレイヤー強調表示間隔
		constexpr float MY_PLAYER_BLINK_LIGHT_INTENSITY = 0.02f;	// 自プレイヤー強調表示光量
		// 勝利出現設定
		const ANIMATION_DATA WIN{
			"data\\TEXTURE\\YouWin.png",	// テクスチャファイルパス
			{{
				{500.0f, 1000.0f, 0.0f},		// 開始位置
				{VEC3_RESET_ZERO},				// 開始向き
				{1200.0f, 600.0f},				// 開始大きさ
				},
				COLOR_NONE,						// 開始カラー
			},
			{{
				{500.0f, 500.0f, 0.0f},			// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{400.0f, 200.0f},				// 終了大きさ
				},
				COLOR_WHITE,					// 終了カラー
			},
			20,								// 開始までの時間
			5,								// 終了までの時間
		};
		// 敗北出現設定
		const ANIMATION_DATA LOSE{
			"data\\TEXTURE\\YouLose.png",	// テクスチャファイルパス
			{{
				{500.0f, 500.0f, 0.0f},			// 開始位置
				{0.0f, 0.0f, 300.0f },			// 開始向き
				{0.0f, 0.0f},					// 開始大きさ
				},
				COLOR_NONE,						// 開始カラー
				},
			{{
				{500.0f, 500.0f, 0.0f},			// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{400.0f, 200.0f},				// 終了大きさ
				},
				COLOR_WHITE,					// 終了カラー
				},
			10,								// 開始までの時間
			50,								// 終了までの時間
		};
		// 敗北出現設定
		const ANIMATION_DATA DRAW{
			"data\\TEXTURE\\YouLose.png",	// テクスチャファイルパス
			{{
				{500.0f, 500.0f, 0.0f},			// 開始位置
				{VEC3_RESET_ZERO},			// 開始向き
				{0.0f, 0.0f},					// 開始大きさ
				},
				COLOR_NONE,						// 開始カラー
				},
			{{
				{500.0f, 500.0f, 0.0f},			// 終了位置
				{VEC3_RESET_ZERO},				// 終了向き
				{400.0f, 200.0f},				// 終了大きさ
				},
				COLOR_WHITE,					// 終了カラー
				},
			10,								// 開始までの時間
			50,								// 終了までの時間
		};


#endif // 
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

		void SetRanking(_In_ int nRanking[MAX_RANKING_COUNT], _In_ int nPlayer);	// ランキング設定

		bool IsEndEffect();	// エフェクトが終わっているか

		json LoadAnimation(const std::string& path);	// 
		void SaveAnimation(const std::string& path, const json& j);	// 

		static CResultUIManager* Create(_In_ int nRanking[MAX_RANKING_COUNT], _In_ int nPlayer);	// 生成処理

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

		// 全体
		int m_nEffectCount;	// 全体のエフェクトカウント
	};
}

#endif // !_RESULT_UI_MANAGER_H_