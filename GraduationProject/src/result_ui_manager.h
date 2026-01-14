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
		const D3DXVECTOR3 RANKING_POS			[MAX_RANKING_COUNT] = {	// 順位の位置
			{1000.0f, 100.0f, 0.0f},	// 1位
			{1000.0f, 200.0f, 0.0f},	// 2位
			{1000.0f, 300.0f, 0.0f},	// 3位
			{1000.0f, 400.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR3 RANKING_ROT			[MAX_RANKING_COUNT] = {	// 順位の向き
			{0.0f, 0.0f, 0.0f},	// 1位
			{0.0f, 0.0f, 0.0f},	// 2位
			{0.0f, 0.0f, 0.0f},	// 3位
			{0.0f, 0.0f, 0.0f},	// 4位
		};
		const D3DXVECTOR2 RANKING_SIZE			[MAX_RANKING_COUNT] = {	// 順位のスケール
			{50.0f, 50.0f},	// 1位
			{50.0f, 50.0f},	// 2位
			{50.0f, 50.0f},	// 3位
			{50.0f, 50.0f},	// 4位
		};
		constexpr float RANKING_TO_NAME_GAP_X = 150.0f;	// 間隔

		const std::string WIN_TEXTURE  = "data\\TEXTURE\\YouWin.png";	// 勝利時テクスチャファイルパス
		const std::string LOSE_TEXTURE = "data\\TEXTURE\\Youlose.png";	// 敗北時テクスチャファイルパス
		const  D3DXVECTOR3 WIN_OR_LOSE_POS =  {500.0f, 500.0f, 0.0f};	// 勝敗表示位置
		const  D3DXVECTOR3 WIN_OR_LOSE_ROT =  {0.0f, 0.0f, 0.0f};		// 勝敗表示向き
		const  D3DXVECTOR2 WIN_OR_LOSE_SIZE = {400.0f, 200.0f};			// 勝敗表示大きさ

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
		int m_nRanking[MAX_RANKING_COUNT];	// ランキング順位
		int m_nPlayer;						// 自プレイヤーの番号
		CResultRanking* m_pRankingUI[MAX_RANKING_COUNT];		// 順位の表示用(例１位)
		CResultRanking* m_pRankingNamesUI[MAX_RANKING_COUNT];	// プレイヤーの表示用（例p1）
		CResultRanking* m_pWinOrLose;							// 勝敗
	};
}

#endif // !_RESULT_UI_MANAGER_H_