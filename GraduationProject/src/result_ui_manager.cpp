//=============================================
//
// リザルトUIマネージャー[result_ui_manager.cpp]
// Author Ueda Kou
//
//=============================================
#include "result_ui_manager.h"	//リザルトUIマネージャー

#include <algorithm>	// findを使うのに必要

//=============================================
// コンストラクタ
//=============================================
My::CResultUIManager::CResultUIManager():
	m_nRanking{-1,-1,-1,-1},	// ランキング初期化
	m_nPlayer(-1),				// プレイヤー番号初期化
	m_pRankingUI{nullptr, nullptr, nullptr, nullptr},		// ランキングオブジェクト初期化
	m_pRankingNamesUI{nullptr, nullptr, nullptr, nullptr},	// ランキングプレイヤー初期化
	m_nRankingMoveDelay{0, 0, 0, 0},	// 移動開始までの時間初期化
	m_nRankingMoveDuration{0, 0, 0, 0},	// 移動時間初期化
	m_nRankingMovePos{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},	// 移動量初期化
	m_nRankingMoveRot{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}},	// 回転量初期化
	m_nRankingMoveSize{{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}},	// 拡縮量初期化
	m_nMyPlayerBlink(MY_PLAYER_BLINK),
	m_isUpMyPlayerBlink(false),
	m_pWinOrLose(nullptr),	// 勝敗表示
	m_nWinOrLoseMoveDelay(0),	
	m_nWinOrLoseMoveDuration(0),	
	m_nWinOrLoseMovePos{0.0f, 0.0f, 0.0f},
	m_nWinOrLoseMoveRot{0.0f, 0.0f, 0.0f},
	m_nWinOrLoseMoveSize{0.0f, 0.0f}
{
}

//=============================================
// デストラクタ
//=============================================
My::CResultUIManager::~CResultUIManager()
{
}

//=============================================
// 初期化処理
//=============================================
HRESULT My::CResultUIManager::Init()
{
	CTexture* pTexture = GET_TEXTURE;// テクスチャ取得
	HWND hWnd = GET_MANAGER->GetHwnd();
	// ランキング
	{
		// 順位表示
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			if (CResultRanking* pRanking = CResultRanking::Create(); pRanking != nullptr)
			{
				m_pRankingUI[nCnt] = pRanking;

				m_pRankingUI[nCnt]->SetPos(RANKING_START_POS[nCnt]);		// 位置
				m_pRankingUI[nCnt]->SetRot(RANKING_START_ROT[nCnt]);		// 向き
				m_pRankingUI[nCnt]->SetSize(RANKING_START_SIZE[nCnt]);	// 大きさ
				m_pRankingUI[nCnt]->SetColor(RANKING_START_COL[nCnt]);

				m_pRankingUI[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKING_TEXTURE[nCnt])));	// テクスチャ設定

				m_pRankingUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else
			{
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);
				return E_UNEXPECTED;
			}
		}
		// プレイヤー表示
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			if (CResultRanking* pRanking = CResultRanking::Create(); pRanking != nullptr)
			{
				m_pRankingNamesUI[nCnt] = pRanking;

				m_pRankingNamesUI[nCnt]->SetPos(D3DXVECTOR3(RANKING_START_POS[nCnt].x + RANKING_TO_NAME_GAP_X, RANKING_START_POS[nCnt].y, RANKING_START_POS[nCnt].z));	// 位置
				m_pRankingNamesUI[nCnt]->SetRot(RANKING_START_ROT[nCnt]);		// 向き
				m_pRankingNamesUI[nCnt]->SetSize(RANKING_START_SIZE[nCnt]);	// 大きさ
				m_pRankingNamesUI[nCnt]->SetColor(RANKING_START_COL[nCnt]);

				m_pRankingNamesUI[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKING_NAME_TEXTURE[m_nRanking[nCnt]])));	// テクスチャ

				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else
			{
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);
				return E_UNEXPECTED;
			}
		}
		// 移動
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			m_nRankingMoveDelay[nCnt] = RANKING_MOVE_DELAY[nCnt];		// ランキングの移動開始時間
			m_nRankingMoveDuration[nCnt] = RANKING_MOVE_DURATION[nCnt];	// ランキングの移動時間
			if (m_nRankingMoveDuration[nCnt] != 0)
			{
				m_nRankingMovePos[nCnt] = (RANKING_START_POS[nCnt] - RANKING_END_POS[nCnt] ) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 位置
				m_nRankingMoveRot[nCnt] = (RANKING_START_ROT[nCnt] - RANKING_END_ROT[nCnt] ) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 向き			}
				m_nRankingMoveSize[nCnt]= (RANKING_START_SIZE[nCnt]- RANKING_END_SIZE[nCnt]) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 大きさ
				m_nRankingMoveCol[nCnt] = (RANKING_START_COL[nCnt]- RANKING_END_COL[nCnt]) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);
			}
		}
	}
	// 勝利結果
	{
		if (CResultRanking* pWinOrLose = CResultRanking::Create(); pWinOrLose != nullptr)
		{
			m_pWinOrLose = pWinOrLose;


			m_pWinOrLose->SetPos (WIN_OR_LOSE_START_POS);		// 位置
			m_pWinOrLose->SetRot (WIN_OR_LOSE_START_ROT);		// 向き
			m_pWinOrLose->SetSize(WIN_OR_LOSE_START_SIZE);	// 大きさ
			m_pWinOrLose->SetColor(WIN_OR_LOSE_START_COL);

			// plのランクを探す
			int idx = std::find(m_nRanking, m_nRanking + MAX_RANKING_COUNT, m_nPlayer) - m_nRanking;
			if (idx == MAX_RANKING_COUNT)  // 見つからなかった場合
			{
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 勝敗結果を正しく生成出来ませんでした", "警告！", MB_ICONWARNING);
				return E_UNEXPECTED;
			}

			if (idx == MAX_RANKING_COUNT - 1)
			{
				// 最下位なら
				m_pWinOrLose->BindTexture(pTexture->GetAddress(pTexture->Regist(LOSE_TEXTURE)));
			}
			else
			{
				// 最下位でないなら
				m_pWinOrLose->BindTexture(pTexture->GetAddress(pTexture->Regist(WIN_TEXTURE)));
			}

			m_pWinOrLose->SetVtx();	// 頂点の設定
			m_pWinOrLose->SetVtx(0.0f ,100.0f);

			m_nWinOrLoseMoveDelay = WIN_OR_LOSE_MOVE_DELAY;		// ランキングの移動開始時間
			m_nWinOrLoseMoveDuration = WIN_OR_LOSE_MOVE_DURATION;	// ランキングの移動時間
			if (m_nWinOrLoseMoveDuration != 0)
			{
				m_nWinOrLoseMovePos = (WIN_OR_LOSE_START_POS - WIN_OR_LOSE_END_POS ) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 位置
				m_nWinOrLoseMoveRot = (WIN_OR_LOSE_START_ROT - WIN_OR_LOSE_END_ROT ) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 向き
				m_nWinOrLoseMoveSize= (WIN_OR_LOSE_START_SIZE- WIN_OR_LOSE_END_SIZE) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 大きさ
				m_nWinOrLoseMoveCol = (WIN_OR_LOSE_START_COL- WIN_OR_LOSE_END_COL) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 大きさ
			}
		}
	}

	return S_OK;
}

//=============================================
// 終了処理
//=============================================
void My::CResultUIManager::Uninit()
{
}

//=============================================
//更新処理
//=============================================
void My::CResultUIManager::Update()
{
	// ランキング
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		// 移動開始時間が経過していなかったらカウントする
		if (m_nRankingMoveDelay[nCnt] > 0){m_nRankingMoveDelay[nCnt]--;}
		// 移動開始時間が経過していたら
		else
		{
			// 移動時間が経過していなかったら
			if (m_nRankingMoveDuration[nCnt] > 0)
			{
				m_nRankingMoveDuration[nCnt]--;
				// 移動処理

				m_pRankingUI[nCnt]->AddPos	(-m_nRankingMovePos[nCnt] );	// 位置
				m_pRankingUI[nCnt]->AddRot	(-m_nRankingMoveRot[nCnt] );	// 向き
				m_pRankingUI[nCnt]->AddSize	(-m_nRankingMoveSize[nCnt]);	// 大きさ
				m_pRankingUI[nCnt]->AddColor(-m_nRankingMoveCol[nCnt]);		// 色
				m_pRankingUI[nCnt]->SetVtx();	// 頂点の設定

				m_pRankingNamesUI[nCnt]->AddPos		(-m_nRankingMovePos[nCnt] );	// 位置
				m_pRankingNamesUI[nCnt]->AddRot		(-m_nRankingMoveRot[nCnt] );	// 向き
				m_pRankingNamesUI[nCnt]->AddSize	(-m_nRankingMoveSize[nCnt]);	// 大きさ
				m_pRankingNamesUI[nCnt]->AddColor	(-m_nRankingMoveCol[nCnt]);		// 色
				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else if (nCnt == m_nPlayer)
			{
				if (m_isUpMyPlayerBlink == true)
				{
					m_nMyPlayerBlink--;
					if (m_nMyPlayerBlink <= 0)
					{
						m_nMyPlayerBlink = MY_PLAYER_BLINK;
						m_isUpMyPlayerBlink = false;
					}
					m_pRankingNamesUI[nCnt]->AddColor	(D3DXCOLOR(0.01f, 0.01f, 0.01f, 0.0f));		// 色
				}
				else
				{
					m_nMyPlayerBlink--;
					if (m_nMyPlayerBlink <= 0)
					{
						m_nMyPlayerBlink = MY_PLAYER_BLINK;
						m_isUpMyPlayerBlink = true;
					}
					m_pRankingNamesUI[nCnt]->AddColor	(D3DXCOLOR(-0.01f, -0.01f, -0.01f, 0.0f));		// 色
				}

				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
		}
	}
	// 勝敗
	// 移動開始時間が経過していなかったらカウントする
	if (m_nWinOrLoseMoveDelay > 0){m_nWinOrLoseMoveDelay--;}
	// 移動開始時間が経過していたら
	else
	{
		// 移動時間が経過していなかったら
		if (m_nWinOrLoseMoveDuration > 0)
		{
			m_nWinOrLoseMoveDuration--;
			// 移動処理

			m_pWinOrLose->AddPos	(-m_nWinOrLoseMovePos );	// 位置
			m_pWinOrLose->AddRot	(-m_nWinOrLoseMoveRot );	// 向き
			m_pWinOrLose->AddSize	(-m_nWinOrLoseMoveSize);	// 大きさ
			m_pWinOrLose->AddColor	(-m_nWinOrLoseMoveCol);	// 大きさ
			m_pWinOrLose->SetVtx();	// 頂点の設定
		}
	}

}
//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
void My::CResultUIManager::SetRanking(int nRanking[MAX_RANKING_COUNT], int nPlayer)
{
	// 順位をコピー
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		m_nRanking[nCnt] = nRanking[nCnt];
	}
	m_nPlayer = nPlayer;
}

//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
/// <returns>生成に成功したらポインターを返す</returns>
My::CResultUIManager* My::CResultUIManager::Create(int nRanking[MAX_RANKING_COUNT], int nPlayer)
{
	CResultUIManager* pResultUIManager= new CResultUIManager();
	pResultUIManager->SetRanking(nRanking, nPlayer);

	if (FAILED(pResultUIManager->Init()))
	{
		delete pResultUIManager;
		pResultUIManager = nullptr;
		return nullptr;
	}

	return pResultUIManager;
}
