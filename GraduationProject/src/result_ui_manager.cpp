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
	// ランキング変数
	m_nRanking{-1,-1,-1,-1},	// ランキング初期化
	m_nPlayer(-1),				// プレイヤー番号初期化
	m_pRankingUI{nullptr, nullptr, nullptr, nullptr},		// ランキングオブジェクト初期化
	m_pRankingNamesUI{nullptr, nullptr, nullptr, nullptr},	// ランキングプレイヤー初期化
	m_nRankingMoveDelay{INT_ZERO, INT_ZERO, INT_ZERO, INT_ZERO},	// ランキング出現開始までの時間初期化
	m_nRankingMoveDuration{INT_ZERO, INT_ZERO, INT_ZERO, INT_ZERO},	// ランキング出現時間初期化
	m_nRankingMovePos{{VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}},	// ランキング移動量初期化
	m_nRankingMoveRot{{VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}, {VEC3_RESET_ZERO}},	// ランキング回転量初期化
	m_nRankingMoveSize{{VEC2_RESET_ZERO}, {VEC2_RESET_ZERO}, {VEC2_RESET_ZERO}, {VEC2_RESET_ZERO}},							// ランキング拡縮量初期化
	m_nMyPlayerBlink(MY_PLAYER_BLINK),	// 自プレイヤー点滅カウント
	m_isUpMyPlayerBlink(false),			// 自プレイヤー点滅が明るくなっているか
	// 勝敗変数
	m_pWinOrLose(nullptr),	// 勝敗オブジェクト初期化
	m_nWinOrLoseMoveDelay(INT_ZERO),	// 勝敗出現開始までの時間初期化
	m_nWinOrLoseMoveDuration(INT_ZERO),// 勝敗出現時間初期化
	m_nWinOrLoseMovePos{VEC3_RESET_ZERO},	// 勝敗移動量初期化
	m_nWinOrLoseMoveRot{VEC3_RESET_ZERO},	// 勝敗回転量初期化
	m_nWinOrLoseMoveSize{VEC2_RESET_ZERO},		// 勝敗拡縮量初期化
	// 全体
	m_nEffectCount(INT_ZERO)
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
				// 生成正常に生成出来たら格納
				m_pRankingUI[nCnt] = pRanking;

				m_pRankingUI[nCnt]->SetPos(RANKING_START_POS[nCnt]);	// 位置
				m_pRankingUI[nCnt]->SetRot(RANKING_START_ROT[nCnt]);	// 向き
				m_pRankingUI[nCnt]->SetSize(RANKING_START_SIZE[nCnt]);	// 大きさ
				m_pRankingUI[nCnt]->SetColor(RANKING_START_COL[nCnt]);	// 色

				m_pRankingUI[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKING_TEXTURE[nCnt])));	// テクスチャ設定

				m_pRankingUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else
			{
				// 生成正常に生成出来無かったら
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);	// 警告表示
				return E_UNEXPECTED;	// エラーを返す
			}
		}
		// プレイヤー表示
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			if (CResultRanking* pRanking = CResultRanking::Create(); pRanking != nullptr)
			{
				// 生成正常に生成出来たら格納
				m_pRankingNamesUI[nCnt] = pRanking;

				m_pRankingNamesUI[nCnt]->SetPos(D3DXVECTOR3(RANKING_START_POS[nCnt].x + RANKING_TO_NAME_GAP_X, RANKING_START_POS[nCnt].y, RANKING_START_POS[nCnt].z));	// 位置
				m_pRankingNamesUI[nCnt]->SetRot(RANKING_START_ROT[nCnt]);	// 向き
				m_pRankingNamesUI[nCnt]->SetSize(RANKING_START_SIZE[nCnt]);	// 大きさ
				m_pRankingNamesUI[nCnt]->SetColor(RANKING_START_COL[nCnt]);	// 色

				m_pRankingNamesUI[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKING_NAME_TEXTURE[m_nRanking[nCnt]])));	// テクスチャ

				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
			else
			{
				// 生成正常に生成出来無かったら
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);	// 警告表示
				return E_UNEXPECTED;	// エラーを返す
			}
		}
		// 出現
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			m_nRankingMoveDelay[nCnt] = RANKING_MOVE_DELAY[nCnt];		// ランキングの出現開始時間
			m_nRankingMoveDuration[nCnt] = RANKING_MOVE_DURATION[nCnt];	// ランキングの出現時間
			// 出現時間が0でなければ
			if (m_nRankingMoveDuration[nCnt] != 0)
			{
				m_nRankingMovePos[nCnt] = (RANKING_START_POS[nCnt] - RANKING_END_POS[nCnt] ) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 位置
				m_nRankingMoveRot[nCnt] = (RANKING_START_ROT[nCnt] - RANKING_END_ROT[nCnt] ) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 向き			}
				m_nRankingMoveSize[nCnt]= (RANKING_START_SIZE[nCnt]- RANKING_END_SIZE[nCnt]) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 大きさ
				m_nRankingMoveCol[nCnt] = (RANKING_START_COL[nCnt]- RANKING_END_COL[nCnt]) / static_cast<float>(RANKING_MOVE_DURATION[nCnt]);	// 色
			}
		}
	}
	// 勝利結果
	{
		if (CResultRanking* pWinOrLose = CResultRanking::Create(); pWinOrLose != nullptr)
		{
			m_pWinOrLose = pWinOrLose;


			m_pWinOrLose->SetPos (WIN_OR_LOSE_START_POS);	// 位置
			m_pWinOrLose->SetRot (WIN_OR_LOSE_START_ROT);	// 向き
			m_pWinOrLose->SetSize(WIN_OR_LOSE_START_SIZE);	// 大きさ
			m_pWinOrLose->SetColor(WIN_OR_LOSE_START_COL);	// 色

			// plのランクを探す
			int idx = std::find(m_nRanking, m_nRanking + MAX_RANKING_COUNT, m_nPlayer) - m_nRanking;
			if (idx == MAX_RANKING_COUNT)  // 見つからなかった場合
			{
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 勝敗結果を正しく生成出来ませんでした", "警告！", MB_ICONWARNING);
				return E_UNEXPECTED;
			}

			// 最下位なら
			if (idx == MAX_RANKING_COUNT - 1)
			{
				// 最下位なら
				m_pWinOrLose->BindTexture(pTexture->GetAddress(pTexture->Regist(LOSE_TEXTURE)));
			}
			// 最下位でなければ
			else
			{
				// 最下位でないなら
				m_pWinOrLose->BindTexture(pTexture->GetAddress(pTexture->Regist(WIN_TEXTURE)));
			}

			m_pWinOrLose->SetVtx();	// 頂点の設定

			m_nWinOrLoseMoveDelay = WIN_OR_LOSE_MOVE_DELAY;			// ランキングの出現開始時間
			m_nWinOrLoseMoveDuration = WIN_OR_LOSE_MOVE_DURATION;	// ランキングの出現時間
			if (m_nWinOrLoseMoveDuration != 0)
			{
				m_nWinOrLoseMovePos = (WIN_OR_LOSE_START_POS - WIN_OR_LOSE_END_POS ) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 位置
				m_nWinOrLoseMoveRot = (WIN_OR_LOSE_START_ROT - WIN_OR_LOSE_END_ROT ) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 向き
				m_nWinOrLoseMoveSize= (WIN_OR_LOSE_START_SIZE- WIN_OR_LOSE_END_SIZE) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);	// 大きさ
				m_nWinOrLoseMoveCol = (WIN_OR_LOSE_START_COL- WIN_OR_LOSE_END_COL) / static_cast<float>(WIN_OR_LOSE_MOVE_DURATION);		// 色
			}
		}
	}
	// 全体
	{
		m_nEffectCount;
		// エフェクトで一番時間の掛かる物を探す
		// ランキング
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			int nTime = m_nRankingMoveDelay[nCnt] + m_nRankingMoveDuration[nCnt]; // 総合時間格納
			if (nTime > m_nEffectCount)
			{
				m_nEffectCount = nTime;
			}
		}
		// 勝敗
		if (int nTime = m_nWinOrLoseMoveDelay + m_nWinOrLoseMoveDuration; nTime > m_nEffectCount)
		{
			m_nEffectCount = nTime;
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
		// 出現開始時間が経過していなかったらカウントする
		if (m_nRankingMoveDelay[nCnt] > 0){m_nRankingMoveDelay[nCnt]--;}
		// 出現開始時間が経過していたら
		else
		{
			// 出現時間が経過していなかったら
			if (m_nRankingMoveDuration[nCnt] > 0)
			{
				m_nRankingMoveDuration[nCnt]--;
				// 出現処理
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
			// 出現時間が経過していたら自プレイヤーを強調表示する
			else if (nCnt == m_nPlayer)
			{
				// 強調表示が明るくなっているかどうか
				if (m_isUpMyPlayerBlink == true)
				{
					m_nMyPlayerBlink--;	// カウントする
					// カウントが０になっていたら
					if (m_nMyPlayerBlink <= 0)
					{
						m_nMyPlayerBlink = MY_PLAYER_BLINK;
						m_isUpMyPlayerBlink = false;
					}
					m_pRankingNamesUI[nCnt]->AddColor	(D3DXCOLOR(MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));	// 色
				}
				else
				{// 暗くなっている
					m_nMyPlayerBlink--;	// カウントする
										// カウントが０になっていたら
					if (m_nMyPlayerBlink <= 0)
					{
						m_nMyPlayerBlink = MY_PLAYER_BLINK;
						m_isUpMyPlayerBlink = true;
					}
					m_pRankingNamesUI[nCnt]->AddColor	(D3DXCOLOR(-MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));	// 色
				}

				m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
			}
		}
	}
	// 勝敗
	// 出現開始時間が経過していなかったらカウントする
	if (m_nWinOrLoseMoveDelay > 0){m_nWinOrLoseMoveDelay--;}
	// 出現開始時間が経過していたら
	else
	{
		// 出現時間が経過していなかったら
		if (m_nWinOrLoseMoveDuration > 0)
		{
			m_nWinOrLoseMoveDuration--;
			// 出現処理

			m_pWinOrLose->AddPos	(-m_nWinOrLoseMovePos );	// 位置
			m_pWinOrLose->AddRot	(-m_nWinOrLoseMoveRot );	// 向き
			m_pWinOrLose->AddSize	(-m_nWinOrLoseMoveSize);	// 大きさ
			m_pWinOrLose->AddColor	(-m_nWinOrLoseMoveCol);		// 色
			m_pWinOrLose->SetVtx();	// 頂点の設定
		}
	}
	// 全体経過時間
	if (m_nEffectCount > 0){ m_nEffectCount--; }

	//各インプット取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	if ((pKeyboard->GetTrigger(DIK_RETURN)
		|| pMouse->GetTrigger(0))
		&& m_nEffectCount > 0)
	{
		// ランキング
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			// カウントを０にする
			m_nRankingMoveDelay[nCnt] = 0;
			m_nRankingMoveDuration[nCnt] = 0;
			
			// 出現処理
			m_pRankingUI[nCnt]->SetPos	(RANKING_END_POS[nCnt] );	// 位置
			m_pRankingUI[nCnt]->SetRot	(RANKING_END_ROT[nCnt] );	// 向き
			m_pRankingUI[nCnt]->SetSize	(RANKING_END_SIZE[nCnt]);	// 大きさ
			m_pRankingUI[nCnt]->SetColor(RANKING_END_COL[nCnt]);	// 色
			m_pRankingUI[nCnt]->SetVtx();	// 頂点の設定

			m_pRankingNamesUI[nCnt]->SetPos		(D3DXVECTOR3(RANKING_END_POS[nCnt].x + RANKING_TO_NAME_GAP_X, RANKING_END_POS[nCnt].y, RANKING_END_POS[nCnt].z));
			m_pRankingNamesUI[nCnt]->SetSize	(RANKING_END_SIZE[nCnt]);	// 大きさ
			m_pRankingNamesUI[nCnt]->SetColor	(RANKING_END_COL[nCnt]);		// 色
			m_pRankingNamesUI[nCnt]->SetVtx();	// 頂点の設定
		}
		// カウントを０にする
		m_nWinOrLoseMoveDelay = 0;
		m_nWinOrLoseMoveDuration = 0;

		// 出現処理
		m_pWinOrLose->SetPos	(WIN_OR_LOSE_END_POS );	// 位置
		m_pWinOrLose->SetRot	(WIN_OR_LOSE_END_ROT );	// 向き
		m_pWinOrLose->SetSize	(WIN_OR_LOSE_END_SIZE);	// 大きさ
		m_pWinOrLose->SetColor	(WIN_OR_LOSE_END_COL );	// 色
		m_pWinOrLose->SetVtx();	// 頂点の設定
	}
}
//=============================================
// エフェクトが終わっているか
//=============================================
bool My::CResultUIManager::IsEndEffect()
{
	return m_nEffectCount <= 0;
}
//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
void My::CResultUIManager::SetRanking(_In_ int nRanking[MAX_RANKING_COUNT],_In_ int nPlayer)
{
	// 順位をコピー
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		m_nRanking[nCnt] = nRanking[nCnt];
	}
	// プレイヤー番号をコピー
	m_nPlayer = nPlayer;
}

//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
/// <returns>生成に成功したらポインターを返す</returns>
My::CResultUIManager* My::CResultUIManager::Create(_In_ int nRanking[MAX_RANKING_COUNT],_In_ int nPlayer)
{
	CResultUIManager* pResultUIManager= new CResultUIManager();
	pResultUIManager->SetRanking(nRanking, nPlayer);

	// 初期化
	if (FAILED(pResultUIManager->Init()))
	{
		// 初期化に失敗したらnullptrを返す
		delete pResultUIManager;
		pResultUIManager = nullptr;
		return nullptr;
	}

	return pResultUIManager;
}
