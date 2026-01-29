//=============================================
//
// リザルトUIマネージャー[result_ui_manager.cpp]
// Author Ueda Kou
//
//=============================================
#include "result_ui_manager.h"	//リザルトUIマネージャー
#include "active_scene_manager.h"

#include <algorithm>	// findを使うのに必要


//=============================================
// コンストラクタ
//=============================================
My::CResultUIManager::CResultUIManager():
#if Json
	// 定数
	RANKINGS{},
	PLAYER_IDS{},
	MY_PLAYER_BLINK (0),
	MY_PLAYER_BLINK_LIGHT_INTENSITY (0.0f),
	WIN{},
	LOSE{},
#endif // !Json
	// 試合結果
	m_nRanking{-1,-1,-1,-1},	// ランキング初期化
	m_nPlayer(-1),				// プレイヤー番号初期化
	// ランキング
	m_pRankingsObject{nullptr, nullptr, nullptr, nullptr},		// ランキングオブジェクト初期化
	m_RankingsAnimation {},				// ランキングアニメーション
	m_RankingsDelayTime {0, 0, 0, 0},	// アニメーションするまでの時間
	m_RankingsElapsedTime {0, 0, 0, 0},	// アニメーションに掛かる時間
	// プレイヤーID表示
	m_pPlayerIDsObject {nullptr, nullptr, nullptr, nullptr},	// プレイヤーIDオブジェクト初期化
	m_PlayerIDsAnimation {},				// プレイヤーIDアニメーション
	m_PlayerIDsDelayTime {0, 0, 0, 0},		// アニメーションするまでの時間
	m_PlayerIDsElapsedTime {0, 0, 0, 0},	// アニメーションに掛かる時間
	// 強調表示
	m_nMyPlayerBlink(0),	// 自プレイヤー点滅カウント
	m_isUpMyPlayerBlink(false),			// 自プレイヤー点滅が明るくなっているか
	// 勝敗表示
	m_pWinOrLoseObject(nullptr),	// 勝敗オブジェクト初期化
	m_pWinOrLoseAnimation{},		// 勝敗アニメーション
	m_nWinOrLoseMoveDelay(0),		// アニメーションするまでの時間
	m_nWinOrLoseMoveDuration(0),	// アニメーションに掛かる時間
	m_isWin(false),
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

	json j;
#if Json
	// 読み込み処理
	j = LoadAnimation(ANIMATION_DATA_FILE_PATH);
	// 順位
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		std::string key = "PL_" + std::to_string(nCnt + 1);	// PL番号
		// テクスチャ
		RANKINGS[nCnt].texture =	 j["Ranking"][key]["Texture"];
		// 開始設定
		RANKINGS[nCnt].start.pos.x = j["Ranking"][key]["Start"]["Pos"][0];	// 位置X
		RANKINGS[nCnt].start.pos.y = j["Ranking"][key]["Start"]["Pos"][1];	// 位置Y
		RANKINGS[nCnt].start.pos.z = j["Ranking"][key]["Start"]["Pos"][2];	// 位置Z
		RANKINGS[nCnt].start.rot.x = j["Ranking"][key]["Start"]["Rot"][0];	// 向きX
		RANKINGS[nCnt].start.rot.y = j["Ranking"][key]["Start"]["Rot"][1];	// 向きY
		RANKINGS[nCnt].start.rot.z = j["Ranking"][key]["Start"]["Rot"][2];	// 向きZ
		RANKINGS[nCnt].start.size.x =j["Ranking"][key]["Start"]["Size"][0];	// 大きさX
		RANKINGS[nCnt].start.size.y =j["Ranking"][key]["Start"]["Size"][1];	// 大きさY
		RANKINGS[nCnt].start.col.r = j["Ranking"][key]["Start"]["Col"][0];	// 色R
		RANKINGS[nCnt].start.col.g = j["Ranking"][key]["Start"]["Col"][1];	// 色G
		RANKINGS[nCnt].start.col.b = j["Ranking"][key]["Start"]["Col"][2];	// 色B
		RANKINGS[nCnt].start.col.a = j["Ranking"][key]["Start"]["Col"][3];	// 色A
		// 終了処理
		RANKINGS[nCnt].end.pos.x = j["Ranking"][key]["End"]["Pos"][0];	// 位置X
		RANKINGS[nCnt].end.pos.y = j["Ranking"][key]["End"]["Pos"][1];	// 位置Y
		RANKINGS[nCnt].end.pos.z = j["Ranking"][key]["End"]["Pos"][2];	// 位置Z
		RANKINGS[nCnt].end.rot.x = j["Ranking"][key]["End"]["Rot"][0];	// 向きX
		RANKINGS[nCnt].end.rot.y = j["Ranking"][key]["End"]["Rot"][1];	// 向きY
		RANKINGS[nCnt].end.rot.z = j["Ranking"][key]["End"]["Rot"][2];	// 向きZ
		RANKINGS[nCnt].end.size.x =j["Ranking"][key]["End"]["Size"][0];	// 大きさ
		RANKINGS[nCnt].end.size.y =j["Ranking"][key]["End"]["Size"][1];	// 大きさ
		RANKINGS[nCnt].end.col.r = j["Ranking"][key]["End"]["Col"][0];	// 色R
		RANKINGS[nCnt].end.col.g = j["Ranking"][key]["End"]["Col"][1];	// 色G
		RANKINGS[nCnt].end.col.b = j["Ranking"][key]["End"]["Col"][2];	// 色B
		RANKINGS[nCnt].end.col.a = j["Ranking"][key]["End"]["Col"][3];	// 色A
		// 時間
		RANKINGS[nCnt].delayTime = j["Ranking"][key]["DelayTime"];		// 開始までの時間
		RANKINGS[nCnt].elapsedTime = j["Ranking"][key]["ElapsedTime"];	// 終了までの時間
	}

	// プレイヤーID
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		std::string key = "PL_" + std::to_string(nCnt + 1);	// PL番号
		// テクスチャ
		PLAYER_IDS[nCnt].texture =	 j["PlayerIDs"][key]["Texture"];
		// 開始設定
		PLAYER_IDS[nCnt].start.pos.x = j["PlayerIDs"][key]["Start"]["Pos"][0];	// 位置X
		PLAYER_IDS[nCnt].start.pos.y = j["PlayerIDs"][key]["Start"]["Pos"][1];	// 位置Y
		PLAYER_IDS[nCnt].start.pos.z = j["PlayerIDs"][key]["Start"]["Pos"][2];	// 位置Z
		PLAYER_IDS[nCnt].start.rot.x = j["PlayerIDs"][key]["Start"]["Rot"][0];	// 向きX
		PLAYER_IDS[nCnt].start.rot.y = j["PlayerIDs"][key]["Start"]["Rot"][1];	// 向きY
		PLAYER_IDS[nCnt].start.rot.z = j["PlayerIDs"][key]["Start"]["Rot"][2];	// 向きZ
		PLAYER_IDS[nCnt].start.size.x =j["PlayerIDs"][key]["Start"]["Size"][0];	// 大きさ
		PLAYER_IDS[nCnt].start.size.y =j["PlayerIDs"][key]["Start"]["Size"][1];	// 大きさ
		PLAYER_IDS[nCnt].start.col.r = j["PlayerIDs"][key]["Start"]["Col"][0];	// 色R
		PLAYER_IDS[nCnt].start.col.g = j["PlayerIDs"][key]["Start"]["Col"][1];	// 色G
		PLAYER_IDS[nCnt].start.col.b = j["PlayerIDs"][key]["Start"]["Col"][2];	// 色B
		PLAYER_IDS[nCnt].start.col.a = j["PlayerIDs"][key]["Start"]["Col"][3];	// 色A
		// 終了処理
		PLAYER_IDS[nCnt].end.pos.x = j["PlayerIDs"][key]["End"]["Pos"][0];	// 位置X
		PLAYER_IDS[nCnt].end.pos.y = j["PlayerIDs"][key]["End"]["Pos"][1];	// 位置Y
		PLAYER_IDS[nCnt].end.pos.z = j["PlayerIDs"][key]["End"]["Pos"][2];	// 位置Z
		PLAYER_IDS[nCnt].end.rot.x = j["PlayerIDs"][key]["End"]["Pos"][0];	// 向きX
		PLAYER_IDS[nCnt].end.rot.y = j["PlayerIDs"][key]["End"]["Pos"][1];	// 向きY
		PLAYER_IDS[nCnt].end.rot.z = j["PlayerIDs"][key]["End"]["Pos"][2];	// 向きZ
		PLAYER_IDS[nCnt].end.size.x =j["PlayerIDs"][key]["End"]["Size"][0];	// 大きさ
		PLAYER_IDS[nCnt].end.size.y =j["PlayerIDs"][key]["End"]["Size"][1];	// 大きさ
		PLAYER_IDS[nCnt].end.col.r = j["PlayerIDs"][key]["End"]["Col"][0];	// 色R
		PLAYER_IDS[nCnt].end.col.g = j["PlayerIDs"][key]["End"]["Col"][1];	// 色G
		PLAYER_IDS[nCnt].end.col.b = j["PlayerIDs"][key]["End"]["Col"][2];	// 色B
		PLAYER_IDS[nCnt].end.col.a = j["PlayerIDs"][key]["End"]["Col"][3];	// 色A
		// 時間
		PLAYER_IDS[nCnt].delayTime = j["PlayerIDs"][key]["DelayTime"];		// 開始までの時間
		PLAYER_IDS[nCnt].elapsedTime = j["PlayerIDs"][key]["ElapsedTime"];	// 終了までの時間
	}
	// 勝敗
	{
		// テクスチャ
		WIN.texture =	 j["Win"]["Texture"];
		// 開始設定
		WIN.start.pos.x = j["Win"]["Start"]["Pos"][0];	// 位置X
		WIN.start.pos.y = j["Win"]["Start"]["Pos"][1];	// 位置Y
		WIN.start.pos.z = j["Win"]["Start"]["Pos"][2];	// 位置Z
		WIN.start.rot.x = j["Win"]["Start"]["Rot"][0];	// 向きX
		WIN.start.rot.y = j["Win"]["Start"]["Rot"][1];	// 向きY
		WIN.start.rot.z = j["Win"]["Start"]["Rot"][2];	// 向きZ
		WIN.start.size.x =j["Win"]["Start"]["Size"][0];	// 大きさ
		WIN.start.size.y =j["Win"]["Start"]["Size"][1];	// 大きさ
		WIN.start.col.r = j["Win"]["Start"]["Col"][0];	// 色R
		WIN.start.col.g = j["Win"]["Start"]["Col"][1];	// 色G
		WIN.start.col.b = j["Win"]["Start"]["Col"][2];	// 色B
		WIN.start.col.a = j["Win"]["Start"]["Col"][3];	// 色A
		// 終了処理
		WIN.end.pos.x = j["Win"]["End"]["Pos"][0];	// 位置X
		WIN.end.pos.y = j["Win"]["End"]["Pos"][1];	// 位置Y
		WIN.end.pos.z = j["Win"]["End"]["Pos"][2];	// 位置Z
		WIN.end.rot.x = j["Win"]["End"]["Rot"][0];	// 向きX
		WIN.end.rot.y = j["Win"]["End"]["Rot"][1];	// 向きY
		WIN.end.rot.z = j["Win"]["End"]["Rot"][2];	// 向きZ
		WIN.end.size.x =j["Win"]["End"]["Size"][0];	// 大きさ
		WIN.end.size.y =j["Win"]["End"]["Size"][1];	// 大きさ
		WIN.end.col.r = j["Win"]["End"]["Col"][0];	// 色R
		WIN.end.col.g = j["Win"]["End"]["Col"][1];	// 色G
		WIN.end.col.b = j["Win"]["End"]["Col"][2];	// 色B
		WIN.end.col.a = j["Win"]["End"]["Col"][3];	// 色A
		// 時間
		WIN.delayTime = j["Win"]["DelayTime"];		// 開始までの時間
		WIN.elapsedTime = j["Win"]["ElapsedTime"];	// 終了までの時間

		// テクスチャ
		LOSE.texture =	 j["Lose"] ["Texture"];
		// 開始設定
		LOSE.start.pos.x = j["Lose"]["Start"]["Pos"][0];	// 位置X
		LOSE.start.pos.y = j["Lose"]["Start"]["Pos"][1];	// 位置Y
		LOSE.start.pos.z = j["Lose"]["Start"]["Pos"][2];	// 位置Z
		LOSE.start.rot.x = j["Lose"]["Start"]["Rot"][0];	// 向きX
		LOSE.start.rot.y = j["Lose"]["Start"]["Rot"][1];	// 向きY
		LOSE.start.rot.z = j["Lose"]["Start"]["Rot"][2];	// 向きZ
		LOSE.start.size.x =j["Lose"]["Start"]["Size"][0];	// 大きさ
		LOSE.start.size.y =j["Lose"]["Start"]["Size"][1];	// 大きさ
		LOSE.start.col.r = j["Lose"]["Start"]["Col"][0];	// 色R
		LOSE.start.col.g = j["Lose"]["Start"]["Col"][1];	// 色G
		LOSE.start.col.b = j["Lose"]["Start"]["Col"][2];	// 色B
		LOSE.start.col.a = j["Lose"]["Start"]["Col"][3];	// 色A
		// 終了処理
		LOSE.end.pos.x = j["Lose"]["End"]["Pos"][0];	// 位置X
		LOSE.end.pos.y = j["Lose"]["End"]["Pos"][1];	// 位置Y
		LOSE.end.pos.z = j["Lose"]["End"]["Pos"][2];	// 位置Z
		LOSE.end.rot.x = j["Lose"]["End"]["Rot"][0];	// 向きX
		LOSE.end.rot.y = j["Lose"]["End"]["Rot"][1];	// 向きY
		LOSE.end.rot.z = j["Lose"]["End"]["Rot"][2];	// 向きZ
		LOSE.end.size.x =j["Lose"]["End"]["Size"][0];	// 大きさ
		LOSE.end.size.y =j["Lose"]["End"]["Size"][1];	// 大きさ
		LOSE.end.col.r = j["Lose"]["End"]["Col"][0];	// 色R
		LOSE.end.col.g = j["Lose"]["End"]["Col"][1];	// 色G
		LOSE.end.col.b = j["Lose"]["End"]["Col"][2];	// 色B
		LOSE.end.col.a = j["Lose"]["End"]["Col"][3];	// 色A
		// 時間
		LOSE.delayTime = j["Lose"]["DelayTime"];		// 開始までの時間
		LOSE.elapsedTime = j["Lose"]["ElapsedTime"];	// 終了までの時間
	}
#endif // !Json
	// ランキング
	{
		// 順位表示
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			
			if (CResultRanking* pRanking = CResultRanking::Create(); pRanking != nullptr)
			{
				// 生成正常に生成出来たら格納
				m_pRankingsObject[nCnt] = pRanking;

				m_pRankingsObject[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(RANKINGS[nCnt].texture)));	// テクスチャ設定
				m_pRankingsObject[nCnt]->SetPos(RANKINGS[nCnt].start.pos);	// 位置
				m_pRankingsObject[nCnt]->SetRot(RANKINGS[nCnt].start.rot);	// 向き
				m_pRankingsObject[nCnt]->SetSize(RANKINGS[nCnt].start.size);	// 大きさ
				m_pRankingsObject[nCnt]->SetColor(RANKINGS[nCnt].start.col);	// 色
				m_pRankingsObject[nCnt]->SetVtx();	// 頂点の設定

				// アニメーション設定(0 = アニメーションしない)
				if (RANKINGS[nCnt].elapsedTime > 0)
				{// アニメーションをしなかったら
					m_RankingsDelayTime[nCnt] = RANKINGS[nCnt].delayTime;	// アニメーション開始までの時間
					m_RankingsElapsedTime[nCnt] = RANKINGS[nCnt].elapsedTime;// アニメーションの時間

					m_RankingsAnimation[nCnt].pos = (RANKINGS[nCnt].start.pos - RANKINGS[nCnt].end.pos ) / static_cast<float>(RANKINGS[nCnt].elapsedTime);	// 位置
					m_RankingsAnimation[nCnt].rot = (RANKINGS[nCnt].start.rot - RANKINGS[nCnt].end.rot ) / static_cast<float>(RANKINGS[nCnt].elapsedTime);	// 向き
					m_RankingsAnimation[nCnt].size= (RANKINGS[nCnt].start.size- RANKINGS[nCnt].end.size) / static_cast<float>(RANKINGS[nCnt].elapsedTime);	// 大きさ
					m_RankingsAnimation[nCnt].col = (RANKINGS[nCnt].start.col - RANKINGS[nCnt].end.col ) / static_cast<float>(RANKINGS[nCnt].elapsedTime);	// 色
				}
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
				m_pPlayerIDsObject[nCnt] = pRanking;

				m_pPlayerIDsObject[nCnt]->BindTexture(pTexture->GetAddress(pTexture->Regist(PLAYER_IDS[m_nRanking[nCnt]].texture)));	// テクスチャ
				m_pPlayerIDsObject[nCnt]->SetPos(PLAYER_IDS[nCnt].start.pos);	// 位置
				m_pPlayerIDsObject[nCnt]->SetRot(PLAYER_IDS[nCnt].start.rot);	// 向き
				m_pPlayerIDsObject[nCnt]->SetSize(PLAYER_IDS[nCnt].start.size);	// 大きさ
				m_pPlayerIDsObject[nCnt]->SetColor(PLAYER_IDS[nCnt].start.col);	// 色
				m_pPlayerIDsObject[nCnt]->SetVtx();	// 頂点の設定

				if (PLAYER_IDS[nCnt].elapsedTime > 0)
				{
					m_PlayerIDsDelayTime[nCnt] = PLAYER_IDS[nCnt].delayTime;	// アニメーション開始までの時間
					m_PlayerIDsElapsedTime[nCnt] = PLAYER_IDS[nCnt].elapsedTime;// アニメーションの時間

					m_PlayerIDsAnimation[nCnt].pos = (PLAYER_IDS[nCnt].start.pos - PLAYER_IDS[nCnt].end.pos ) / static_cast<float>(PLAYER_IDS[nCnt].elapsedTime);	// 位置
					m_PlayerIDsAnimation[nCnt].rot = (PLAYER_IDS[nCnt].start.rot - PLAYER_IDS[nCnt].end.rot ) / static_cast<float>(PLAYER_IDS[nCnt].elapsedTime);	// 向き			}
					m_PlayerIDsAnimation[nCnt].size= (PLAYER_IDS[nCnt].start.size- PLAYER_IDS[nCnt].end.size) / static_cast<float>(PLAYER_IDS[nCnt].elapsedTime);	// 大きさ
					m_PlayerIDsAnimation[nCnt].col = (PLAYER_IDS[nCnt].start.col - PLAYER_IDS[nCnt].end.col ) / static_cast<float>(PLAYER_IDS[nCnt].elapsedTime);	// 色
				}
			}
			else
			{
				// 生成正常に生成出来無かったら
				MessageBox(hWnd, "Error : result_ui_manager.cpp \n 正常なランキングの生成を出来ませんでした", "警告！", MB_ICONWARNING);	// 警告表示
				return E_UNEXPECTED;	// エラーを返す
			}
		}
	}
	// 勝利結果
	{
		// plのランクを探す
		int idx = std::find(m_nRanking, m_nRanking + MAX_RANKING_COUNT, m_nPlayer) - m_nRanking;
		if (idx == MAX_RANKING_COUNT)  // 見つからなかった場合
		{
			MessageBox(hWnd, "Error : result_ui_manager.cpp \n 勝敗結果を正しく生成出来ませんでした", "警告！", MB_ICONWARNING);
			return E_UNEXPECTED;
		}
		if (CResultRanking* pWinOrLose = CResultRanking::Create(); pWinOrLose != nullptr)
		{
			m_pWinOrLoseObject = pWinOrLose;

			// 最下位なら
			if (idx == MAX_RANKING_COUNT - 1)
			{
				// 敗北者のBGMを設定
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_LOSER);
				
				// 最下位なら
				m_pWinOrLoseObject->BindTexture(pTexture->GetAddress(pTexture->Regist(LOSE.texture)));
				m_pWinOrLoseObject->SetPos		(LOSE.start.pos);	// 位置
				m_pWinOrLoseObject->SetRot		(LOSE.start.rot);	// 向き
				m_pWinOrLoseObject->SetSize		(LOSE.start.size);	// 大きさ
				m_pWinOrLoseObject->SetColor	(LOSE.start.col);	// 色

				// 演出０秒じゃないなら
				if (WIN.elapsedTime != 0)
				{
					m_nWinOrLoseMoveDelay =		LOSE.delayTime;			// ランキングの出現開始時間
					m_nWinOrLoseMoveDuration =	LOSE.elapsedTime;	// ランキングの出現時間

					m_pWinOrLoseAnimation.pos = (LOSE.start.pos - LOSE.end.pos ) / static_cast<float>(LOSE.elapsedTime);	// 位置
					m_pWinOrLoseAnimation.rot = (LOSE.start.rot - LOSE.end.rot ) / static_cast<float>(LOSE.elapsedTime);	// 向き
					m_pWinOrLoseAnimation.size= (LOSE.start.size- LOSE.end.size) / static_cast<float>(LOSE.elapsedTime);	// 大きさ
					m_pWinOrLoseAnimation.col = (LOSE.start.col - LOSE.end.col ) / static_cast<float>(LOSE.elapsedTime);	// 色
				}
				// 敗者であることを保存
				m_isWin = false;
			}
			// 最下位でなければ
			else
			{
				// 勝者のBGMを設定
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_WINNER);

				// 最下位なら
				m_pWinOrLoseObject->BindTexture(pTexture->GetAddress(pTexture->Regist(WIN.texture)));
				m_pWinOrLoseObject->SetPos		(WIN.start.pos);	// 位置
				m_pWinOrLoseObject->SetRot		(WIN.start.rot);	// 向き
				m_pWinOrLoseObject->SetSize		(WIN.start.size);	// 大きさ
				m_pWinOrLoseObject->SetColor	(WIN.start.col);	// 色

																	// 演出０秒じゃないなら
				if (WIN.elapsedTime != 0)
				{
					m_nWinOrLoseMoveDelay =		WIN.delayTime;			// ランキングの出現開始時間
					m_nWinOrLoseMoveDuration =	WIN.elapsedTime;	// ランキングの出現時間

					m_pWinOrLoseAnimation.pos = (WIN.start.pos - WIN.end.pos ) / static_cast<float>(WIN.elapsedTime);	// 位置
					m_pWinOrLoseAnimation.rot = (WIN.start.rot - WIN.end.rot ) / static_cast<float>(WIN.elapsedTime);	// 向き
					m_pWinOrLoseAnimation.size= (WIN.start.size- WIN.end.size) / static_cast<float>(WIN.elapsedTime);	// 大きさ
					m_pWinOrLoseAnimation.col = (WIN.start.col - WIN.end.col ) / static_cast<float>(WIN.elapsedTime);	// 色
				}
				// 勝者であることを保存
				m_isWin = true;
			}

			m_pWinOrLoseObject->SetVtx();	// 頂点の設定
		}
	}
	// 全体
	{
		m_nEffectCount;
		// エフェクトで一番時間の掛かる物を探す
		// ランキング
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			int nTime = m_RankingsDelayTime[nCnt] + m_RankingsElapsedTime[nCnt]; // 総合時間格納
			if (nTime > m_nEffectCount)
			{
				m_nEffectCount = nTime;
			}
		}

		// プレイヤーID
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			int nTime = m_PlayerIDsDelayTime[nCnt] + m_PlayerIDsElapsedTime[nCnt]; // 総合時間格納
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
	//BGMを止める
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_LOSER);

	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL_BGM_WINNER);
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
		if (m_RankingsDelayTime[nCnt] > 0){m_RankingsDelayTime[nCnt]--;}
		// 出現開始時間が経過していたら
		else
		{

			// 出現時間が経過していなかったら
			if (m_RankingsElapsedTime[nCnt] > 0)
			{
				m_RankingsElapsedTime[nCnt]--;
				// 出現処理
				m_pRankingsObject[nCnt]->AddPos		(-m_RankingsAnimation[nCnt].pos );	// 位置
				m_pRankingsObject[nCnt]->AddRot		(-m_RankingsAnimation[nCnt].rot );	// 向き
				m_pRankingsObject[nCnt]->AddSize	(-m_RankingsAnimation[nCnt].size);	// 大きさ
				m_pRankingsObject[nCnt]->AddColor	(-m_RankingsAnimation[nCnt].col );	// 色
				m_pRankingsObject[nCnt]->SetVtx();	// 頂点の設定

				if (m_RankingsElapsedTime[nCnt] == 0)
				{
					// UIの移動が終わってからSEを鳴らす
					// SEの設定
					CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_RANKING);
				}

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
						m_nMyPlayerBlink = MY_PLAYER_BLINK;	// カウントリセット
						m_isUpMyPlayerBlink = false;		// 暗くするに設定
					}
					// 色
					m_pPlayerIDsObject[nCnt]->AddColor	(D3DXCOLOR(MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));
				}
				else
				{// 暗くなっている
					m_nMyPlayerBlink--;	// カウントする
					// カウントが０になっていたら
					if (m_nMyPlayerBlink <= 0)
					{
						m_nMyPlayerBlink = MY_PLAYER_BLINK;	// カウントリセット
						m_isUpMyPlayerBlink = true;			// 明るくするに設定
					}
					// 色
					m_pPlayerIDsObject[nCnt]->AddColor	(D3DXCOLOR(-MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));
				}
				m_pPlayerIDsObject[nCnt]->SetVtx();	// 頂点の設定
			}
		}
	}

	// プレイヤーID
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		// 出現開始時間が経過していなかったらカウントする
		if (m_PlayerIDsDelayTime[nCnt] > 0){m_PlayerIDsDelayTime[nCnt]--;}
		// 出現開始時間が経過していたら
		else
		{
			// 出現時間が経過していなかったら
			if (m_PlayerIDsElapsedTime[nCnt] > 0)
			{
				m_PlayerIDsElapsedTime[nCnt]--;
				// 出現処理
				m_pPlayerIDsObject[nCnt]->AddPos	(-m_PlayerIDsAnimation[nCnt].pos );	// 位置
				m_pPlayerIDsObject[nCnt]->AddRot	(-m_PlayerIDsAnimation[nCnt].rot );	// 向き
				m_pPlayerIDsObject[nCnt]->AddSize	(-m_PlayerIDsAnimation[nCnt].size);	// 大きさ
				m_pPlayerIDsObject[nCnt]->AddColor	(-m_PlayerIDsAnimation[nCnt].col );	// 色
				m_pPlayerIDsObject[nCnt]->SetVtx();	// 頂点の設定
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
					m_pPlayerIDsObject[nCnt]->AddColor	(D3DXCOLOR(MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));	// 色
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
					m_pPlayerIDsObject[nCnt]->AddColor	(D3DXCOLOR(-MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, -MY_PLAYER_BLINK_LIGHT_INTENSITY, 0.0f));	// 色
				}

				m_pPlayerIDsObject[nCnt]->SetVtx();	// 頂点の設定
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
			// カウントを進める
			m_nWinOrLoseMoveDuration--;

			// 出現処理
			m_pWinOrLoseObject->AddPos		(-m_pWinOrLoseAnimation.pos );	// 位置
			m_pWinOrLoseObject->AddRot		(-m_pWinOrLoseAnimation.rot );	// 向き
			m_pWinOrLoseObject->AddSize		(-m_pWinOrLoseAnimation.size);	// 大きさ
			m_pWinOrLoseObject->AddColor	(-m_pWinOrLoseAnimation.col );		// 色
			m_pWinOrLoseObject->SetVtx();	// 頂点の設定
			// 勝利していたらアニメーション後にSEを鳴らす
			if (m_isWin == true && m_nWinOrLoseMoveDuration == 0)
			{
				// TODO : 勝利の表示時にSEを鳴らす
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WIN);
			}

			// 敗北していたらアニメーション中にSEを鳴らす
			if (m_isWin == false && m_nWinOrLoseMoveDuration == 0)
			{
				// TODO : 敗北の表示時にSEを鳴らす 
				CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_LOSE);
			}

		}
	}
	// 全体経過時間
	if (m_nEffectCount > 0){ m_nEffectCount--; }

	//各インプット取得
	CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	// 演出スキップ
	if ((pKeyboard->GetTrigger(DIK_RETURN)
		|| pMouse->GetTrigger(0))
		&& m_nEffectCount > 0)
	{
		m_nEffectCount = 1;
		// ランキング
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			// カウントを０にする
			m_RankingsDelayTime[nCnt] = 0;
			m_RankingsElapsedTime[nCnt] = 0;
			
			// 出現処理
			m_pRankingsObject[nCnt]->SetPos	 (RANKINGS[nCnt].end.pos);	// 位置
			m_pRankingsObject[nCnt]->SetRot	 (RANKINGS[nCnt].end.rot);	// 向き
			m_pRankingsObject[nCnt]->SetSize (RANKINGS[nCnt].end.size);	// 大きさ
			m_pRankingsObject[nCnt]->SetColor(RANKINGS[nCnt].end.col);	// 色
			m_pRankingsObject[nCnt]->SetVtx();	// 頂点の設定
		}
		// ランキング
		for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
		{
			// カウントを０にする
			m_PlayerIDsDelayTime[nCnt] = 0;
			m_PlayerIDsElapsedTime[nCnt] = 0;

			// 出現処理
			m_pPlayerIDsObject[nCnt]->SetPos	(PLAYER_IDS[nCnt].end.pos );
			m_pRankingsObject[nCnt]->SetRot		(PLAYER_IDS[nCnt].end.rot );	// 向き
			m_pPlayerIDsObject[nCnt]->SetSize	(PLAYER_IDS[nCnt].end.size);	// 大きさ
			m_pPlayerIDsObject[nCnt]->SetColor	(PLAYER_IDS[nCnt].end.col );	// 色
			m_pPlayerIDsObject[nCnt]->SetVtx();	// 頂点の設定
		}
		// カウントを０にする
		m_nWinOrLoseMoveDelay = 0;
		m_nWinOrLoseMoveDuration = 0;

		if (std::find(m_nRanking, m_nRanking + MAX_RANKING_COUNT, m_nPlayer) - m_nRanking == MAX_RANKING_COUNT - 1)
		{
			// 出現処理
			m_pWinOrLoseObject->SetPos		(LOSE.end.pos );	// 位置
			m_pWinOrLoseObject->SetRot		(LOSE.end.rot );	// 向き
			m_pWinOrLoseObject->SetSize		(LOSE.end.size);	// 大きさ
			m_pWinOrLoseObject->SetColor	(LOSE.end.col );	// 色
		}
		else
		{
			// 出現処理
			m_pWinOrLoseObject->SetPos		(WIN.end.pos );	// 位置
			m_pWinOrLoseObject->SetRot		(WIN.end.rot );	// 向き
			m_pWinOrLoseObject->SetSize		(WIN.end.size);	// 大きさ
			m_pWinOrLoseObject->SetColor	(WIN.end.col );	// 色
		}
		m_pWinOrLoseObject->SetVtx();	// 頂点の設定
	}
}
//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
void My::CResultUIManager::SetRanking(_In_ int nRanking[MAX_RANKING_COUNT])
{
	// 順位をコピー
	for (int nCnt = 0; nCnt < MAX_RANKING_COUNT; nCnt++)
	{
		m_nRanking[nCnt] = nRanking[nCnt];
	}
	// プレイヤー番号をコピー
	m_nPlayer = My::CActiveSceneManager::GetInstance()->GetPlayerIndex();
}

//=============================================
// エフェクトが終わっているか
//=============================================
bool My::CResultUIManager::IsEndEffect()
{
	return m_nEffectCount <= 0;
}

//=============================================
// エフェクト情報取得
//=============================================
json My::CResultUIManager::LoadAnimation(const std::string& path)
{
	std::ifstream ifs(path);
	if (!ifs) {
		return json::object();
	}
	json j;
	try{
		ifs >> j;
	}
	catch (...)
	{
		// JSON が壊れている → 空の json を返す
		return json::object();
	}
	return j;
}

//=============================================
// エフェクト情報保存
//=============================================
void My::CResultUIManager::SaveAnimation(const std::string& path, const json& j)
{
	std::ofstream ofs(path);
	if (!ofs) {
		throw std::runtime_error("Failed to open file: " + path);
	}

	ofs << j.dump(4);  // 4 はインデント幅
}

//=============================================
// 生成処理
//=============================================
/// <param name="nRanking">順位</param>
/// <param name="nPlayer">プレイヤーの順位</param>
/// <returns>生成に成功したらポインターを返す</returns>
My::CResultUIManager* My::CResultUIManager::Create(_In_ int nRanking[MAX_RANKING_COUNT])
{
	CResultUIManager* pResultUIManager= new CResultUIManager();
	pResultUIManager->SetRanking(nRanking);

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
