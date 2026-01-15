//=============================================
//
// リザルトUIマネージャー[result_ui_manager.h]
// Author Ueda Kou
//
//=============================================
#ifndef _RESULT_UI_MANAGER_H_
#define _RESULT_UI_MANAGER_H_
#include "result_ranking.h"	// リザルト用ランキングポリゴン

namespace My {

	namespace {
		constexpr int MAX_RANKING_COUNT = 4;	// ランキングに乗せる人数
		// ランキング
		const std::string RANKING_TEXTURE		[MAX_RANKING_COUNT] = {	// 順位表示テクスチャファイルパス
			"data\\TEXTURE\\rank_1.png",	// 1位の順位 TODO
			"data\\TEXTURE\\rank_2.png",	// 2位の順位 TODO
			"data\\TEXTURE\\rank_3.png",	// 3位の順位 TODO
			"data\\TEXTURE\\rank_4.png"		// 4位の順位 TODO
		};
		const std::string RANKING_NAME_TEXTURE	[MAX_RANKING_COUNT] = {	// プレイヤー表示テクスチャファイルパス
			"data\\TEXTURE\\player_1P.png",	// プレイヤー1
			"data\\TEXTURE\\player_2P.png",	// プレイヤー2
			"data\\TEXTURE\\player_3P.png",	// プレイヤー3
			"data\\TEXTURE\\player_4P.png"	// プレイヤー4
		};
		// 初期位置
		const D3DXVECTOR3 RANKING_START_POS			[MAX_RANKING_COUNT] = {	// 順位の初期位置
			{2000.0f, 100.0f, 0.0f},	// 1位
			{2000.0f, 200.0f, 0.0f},	// 2位
			{2000.0f, 300.0f, 0.0f},	// 3位
			{2000.0f, 400.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR3 RANKING_START_ROT			[MAX_RANKING_COUNT] = {	// 順位の初期向き
			{0.0f, 0.0f, 0.0f},	// 1位
			{0.0f, 0.0f, 0.0f},	// 2位
			{0.0f, 0.0f, 0.0f},	// 3位
			{0.0f, 0.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR2 RANKING_START_SIZE		[MAX_RANKING_COUNT] = {	// 順位の初期スケール
			{50.0f, 50.0f},	// 1位
			{50.0f, 50.0f},	// 2位
			{50.0f, 50.0f},	// 3位
			{50.0f, 50.0f},	// 4位
		};
		const D3DXCOLOR RANKING_START_COL			[MAX_RANKING_COUNT] = {	// 順位の表示位置
			{0.0, 0.0f, 0.0f, 0.0f},	// 1位
			{0.0, 0.0f, 0.0f, 0.0f},	// 2位
			{0.0, 0.0f, 0.0f, 0.0f},	// 3位
			{0.0, 0.0f, 0.0f, 0.0f},	// 4位
		};

		// 表示位置
		const D3DXVECTOR3 RANKING_END_POS			[MAX_RANKING_COUNT] = {	// 順位の表示位置
			{1000.0f, 100.0f, 0.0f},	// 1位
			{1000.0f, 200.0f, 0.0f},	// 2位
			{1000.0f, 300.0f, 0.0f},	// 3位
			{1000.0f, 400.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR3 RANKING_END_ROT			[MAX_RANKING_COUNT] = {	// 順位の表示向き
			{0.0f, 0.0f, 0.0f},	// 1位
			{0.0f, 0.0f, 0.0f},	// 2位
			{0.0f, 0.0f, 0.0f},	// 3位
			{0.0f, 0.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR2 RANKING_END_SIZE			[MAX_RANKING_COUNT] = {	// 順位の表示スケール
			{50.0f, 50.0f},	// 1位
			{50.0f, 50.0f},	// 2位
			{50.0f, 50.0f},	// 3位
			{50.0f, 50.0f},	// 4位
		};
		const D3DXCOLOR RANKING_END_COL			[MAX_RANKING_COUNT] = {	// 順位の表示位置
			{1.0f, 1.0f, 1.0f, 1.0f},	// 1位
			{1.0f, 1.0f, 1.0f, 1.0f},	// 2位
			{1.0f, 1.0f, 1.0f, 1.0f},	// 3位
			{1.0f, 1.0f, 1.0f, 1.0f},	// 4位
		};
		constexpr float RANKING_TO_NAME_GAP_X = 150.0f;	// 順位とプレイヤーの間隔
		constexpr int RANKING_MOVE_DELAY			[MAX_RANKING_COUNT] = {	// 移動開始までの時間
			60,
			90,
			120,
			150
		};
		constexpr int RANKING_MOVE_DURATION			[MAX_RANKING_COUNT] = {// 移動時間
			30,
			30,
			30,
			30
		};
		constexpr int MY_PLAYER_BLINK = 30;

		// 勝敗表示
		const std::string WIN_TEXTURE  = "data\\TEXTURE\\YouWin.png";		// 勝利時テクスチャファイルパス
		const std::string LOSE_TEXTURE = "data\\TEXTURE\\YouLose.png";		// 敗北時テクスチャファイルパス
		const D3DXVECTOR3 WIN_OR_LOSE_START_POS =	{500.0f, 1000.0f, 0.0f};// 勝敗表示初期位置
		const D3DXVECTOR3 WIN_OR_LOSE_START_ROT =	{0.0f, 0.0f, 0.0f};		// 勝敗表示初期向き
		const D3DXVECTOR2 WIN_OR_LOSE_START_SIZE =	{1200.0f, 600.0f};		// 勝敗表示初期大きさ
		const D3DXCOLOR	WIN_OR_LOSE_START_COL =		{0, 0, 0, 0};			// 勝敗表示初期の色

		const D3DXVECTOR3 WIN_OR_LOSE_END_POS =		{500.0f, 500.0f, 0.0f};	// 勝敗表示位置
		const D3DXVECTOR3 WIN_OR_LOSE_END_ROT =		{0.0f, 0.0f, 0.0f};		// 勝敗表示向き
		const D3DXVECTOR2 WIN_OR_LOSE_END_SIZE =	{400.0f, 200.0f};		// 勝敗表示大きさ
		const D3DXCOLOR WIN_OR_LOSE_END_COL =		{1.0f, 1.0f, 1.0f, 1.0f};// 勝敗表示勝敗表示初期大きさの色
		constexpr int WIN_OR_LOSE_MOVE_DELAY	=	10;						// 移動開始までの時間
		constexpr int WIN_OR_LOSE_MOVE_DURATION	=	5;						// 移動時間

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

		void SetRanking(int nRanking[MAX_RANKING_COUNT], int nPlayer);	// ランキング設定

		static CResultUIManager* Create(int nRanking[MAX_RANKING_COUNT], int nPlayer);	// 生成処理

	private:
		int m_nRanking	[MAX_RANKING_COUNT];						// ランキング順位
		int m_nPlayer;												// 自プレイヤーの番号
		CResultRanking* m_pRankingUI	[MAX_RANKING_COUNT];		// 順位の表示用(例１位)
		CResultRanking* m_pRankingNamesUI[MAX_RANKING_COUNT];		// プレイヤーの表示用（例p1）
		int m_nRankingMoveDelay			[MAX_RANKING_COUNT];		// ランキングの移動開始時間
		int m_nRankingMoveDuration		[MAX_RANKING_COUNT];		// ランキングの移動時間
		D3DXVECTOR3 m_nRankingMovePos		[MAX_RANKING_COUNT];	// ランキングの移動分
		D3DXVECTOR3 m_nRankingMoveRot		[MAX_RANKING_COUNT];	// ランキングの回転分
		D3DXVECTOR2 m_nRankingMoveSize		[MAX_RANKING_COUNT];	// ランキングの拡縮分
		D3DXCOLOR m_nRankingMoveCol			[MAX_RANKING_COUNT];	// ランキングの色
		int m_nMyPlayerBlink;	// 自プレイヤー点滅カウント
		bool m_isUpMyPlayerBlink;	// 自プレイヤー点滅カウント


		CResultRanking* m_pWinOrLose;		// 勝敗
		int m_nWinOrLoseMoveDelay;			// 勝敗の移動開始時間
		int m_nWinOrLoseMoveDuration;		// 勝敗の移動時間
		D3DXVECTOR3 m_nWinOrLoseMovePos;	// 勝敗の移動分
		D3DXVECTOR3 m_nWinOrLoseMoveRot;	// 勝敗の回転分
		D3DXVECTOR2 m_nWinOrLoseMoveSize;	// 勝敗の拡縮分
		D3DXCOLOR m_nWinOrLoseMoveCol;		// 勝敗の色
	};
}

#endif // !_RESULT_UI_MANAGER_H_