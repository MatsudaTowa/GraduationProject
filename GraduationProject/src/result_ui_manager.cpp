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
	m_nRanking{},
	m_nPlayer(),
	m_pRankingUI{},
	m_pRankingNamesUI{},
	m_pWinOrLose{}
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

				m_pRankingUI[nCnt]->SetPos(RANKING_POS[nCnt]);		// 位置
				m_pRankingUI[nCnt]->SetRot(RANKING_ROT[nCnt]);		// 向き
				m_pRankingUI[nCnt]->SetSize(RANKING_SIZE[nCnt]);	// 大きさ

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

				m_pRankingNamesUI[nCnt]->SetPos(D3DXVECTOR3(RANKING_POS[nCnt].x + RANKING_TO_NAME_GAP_X, RANKING_POS[nCnt].y, RANKING_POS[nCnt].z));	// 位置
				m_pRankingNamesUI[nCnt]->SetRot(RANKING_ROT[nCnt]);		// 向き
				m_pRankingNamesUI[nCnt]->SetSize(RANKING_SIZE[nCnt]);	// 大きさ

				m_pRankingNamesUI[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKING_NAME_TEXTURE[m_nRanking[nCnt]])));	// テクスチャ

				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else
			{
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);
				return E_UNEXPECTED;
			}
		}
	}
	// 勝利結果
	{
		if (CResultRanking* pWinOrLose = CResultRanking::Create(); pWinOrLose != nullptr)
		{
			m_pWinOrLose = pWinOrLose;


			m_pWinOrLose->SetPos (WIN_OR_LOSE_POS);	// 位置
			m_pWinOrLose->SetRot (WIN_OR_LOSE_ROT);							// 向き
			m_pWinOrLose->SetSize(WIN_OR_LOSE_SIZE);												// 大きさ

			// plのランクを


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
