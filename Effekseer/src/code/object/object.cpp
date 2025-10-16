//============================================
//
// オブジェクト[object.cpp]
// Author:Uedakou
// 
//============================================
#include "object.h"	// オブジェクト
#include "../system/manager.h"	// 全体マネージャー
#include "3D/object_billboard.h"	// ビルボード
namespace My
{

	// 静的メンバ変数
	CObject* CObject::m_pTop[MAX_PRIORITY] = {};	// オブジェクトデータ
	CObject* CObject::m_pCur[MAX_PRIORITY] = {};	// オブジェクトデータ
	int CObject::m_nNumObject[MAX_PRIORITY] = {};	// 階層ごとオブジェクト数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CObject::CObject()
	{
		m_pNext = nullptr;		// 次オブジェクト
		m_pPrev = nullptr;		// 前オブジェクト
		m_nNumObject[s_nDefalut_Priority]++;	// オブジェクト数加算
		m_ID = m_nNumObject[s_nDefalut_Priority];	// IDを記録

		m_nPriority = s_nDefalut_Priority;		// オブジェクトプライオリティ

		// トランスフォーム設定
		m_x = X(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_fDistance = 0.0f;	// カメラからの距離

		m_bAllUpdate = true;	// 全体で更新するか
		m_bPoseUpdate = false;	// ポーズ中更新するか
		m_bAllDraw = true;		// 全体で描画するか
		m_bPoseDraw = true;		// ポーズ中描画するか
		m_bReleaseScene = true;	// シーンでリリースするか

		m_bDeath = false;		// デスフラグ

		// 先頭がなかったら先頭設定
		if (m_pTop[s_nDefalut_Priority] != nullptr)
		{
			m_pCur[s_nDefalut_Priority]->m_pNext = this;
			m_pPrev = m_pCur[s_nDefalut_Priority];
			m_pCur[s_nDefalut_Priority] = this;
		}
		else
		{
			m_pTop[s_nDefalut_Priority] = this;
			m_pCur[s_nDefalut_Priority] = this;
		}
	}
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="nPriority">優先度</param>
	CObject::CObject(const int nPriority)
	{
		m_pNext = nullptr;		// 次オブジェクト
		m_pPrev = nullptr;		// 前オブジェクト
		m_nNumObject[nPriority]++;	// オブジェクト数加算
		m_ID = m_nNumObject[nPriority];	// IDを記録

		m_nPriority = nPriority;	// オブジェクトプライオリティ

		// トランスフォーム
		m_x = X(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f));
		m_fDistance = 0.0f;		// カメラからの距離

		m_bAllUpdate = true;		// 全体で更新するか
		m_bPoseUpdate = false;	// ポーズ中更新するか
		m_bAllDraw = true;			// 全体で描画するか
		m_bPoseDraw = true;		// ポーズ中描画するか
		m_bReleaseScene = true;	// シーンでリリースするか

		m_bDeath = false;		// デスフラグ

		// 先頭が有ったら末尾に追加
		if (m_pTop[nPriority] != nullptr)
		{
			m_pCur[nPriority]->m_pNext = this;
			m_pPrev = m_pCur[nPriority];
			m_pCur[nPriority] = this;
		}
		// 先頭がなかったら先頭設定
		else
		{
			m_pTop[nPriority] = this;
			m_pCur[nPriority] = this;
		}
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CObject::~CObject()
	{
		// オブジェクト数を減らす設定
		m_nNumObject[m_nPriority]--;
	}
	/// <summary>
	/// 死亡フラグ
	/// </summary>
	void CObject::Release()
	{
		m_bDeath = true;
	}
	/// <summary>
	/// シーンチェンジ時に解放
	/// </summary>
	void CObject::ReleaseScene()
	{
		// 優先度分繰り返す
		for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			// トップをオブジェクトに入れる
			CObject* pObjact = m_pTop[nCntPriority];
			// オブジェクトが無くなるまで続ける
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->GetNext();	//	次保管
				// シーンリリースで解放するかどうか
				if (pObjact->m_bReleaseScene)
				{
					CObject* pNext = pObjact->m_pNext;	// 次保管
					CObject* pPrev = pObjact->m_pPrev;	// 前保管
					// 次が有れば
					if (pNext != nullptr)
					{// 次に前を入れる
						pNext->SetPrev(pPrev);
					}
					// 前が有れば
					if (pPrev != nullptr)
					{// 前に次を入れる
						pPrev->SetNext(pNext);
					}

					// トップがこのオブジェクトなら
					if (m_pTop[nCntPriority] == pObjact)
					{
						// 次を先頭に入れる
						m_pTop[nCntPriority] = pNext;
					}
					// 末端がこのオブジェクトなら
					if (m_pCur[nCntPriority] == pObjact)
					{
						// 前を末端に入れる
						m_pCur[nCntPriority] = pPrev;
					}
					// オブジェクトの終了処理
					pObjact->Uninit();

					// 解放
					delete pObjact;
				}
				// 次に移行
				pObjact = pNext;

			}
		}
	}
	/// <summary>
	/// 全オブジェクト解放
	/// </summary>
	void CObject::ReleaseAll()
	{
		// 優先度分繰り返す
		for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			// トップをオブジェクトに入れる
			CObject* pObjact = m_pTop[nCntPriority];
			// オブジェクトが無くなるまで続ける
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->m_pNext;	// 次保管
				CObject* pPrev = pObjact->m_pPrev;	// 前保管
				// 次が有れば
				if (pNext != nullptr)
				{// 次に前を入れる
					pNext->SetPrev(pPrev);
				}
				// 前が有れば
				if (pPrev != nullptr)
				{// 前に次を入れる
					pPrev->SetNext(pNext);
				}

				// トップがこのオブジェクトなら
				if (m_pTop[nCntPriority] == pObjact)
				{
					// 次を先頭に入れる
					m_pTop[nCntPriority] = pNext;
				}
				// 末端がこのオブジェクトなら
				if (m_pCur[nCntPriority] == pObjact)
				{
					// 前を末端に入れる
					m_pCur[nCntPriority] = pPrev;
				}
				// オブジェクトの終了処理
				pObjact->Uninit();
				// 解放
				delete pObjact;
				// 次に移行
				pObjact = pNext;
			}
		}
	}
	/// <summary>
	/// 死亡フラグが立っていたら解放
	/// </summary>
	void CObject::ReleaseDeathFlag()
	{
		// 優先度分繰り返す
		for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			// トップをオブジェクトに入れる
			CObject* pObjact = m_pTop[nCntPriority];
			// オブジェクトが無くなるまで続ける
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->m_pNext;	//	次保管
				CObject* pPrev = pObjact->m_pPrev;	// 前保管
				// 死亡フラグが立っていたら
				if (pObjact->m_bDeath == true)
				{
					// 次が有れば
					if (pNext != nullptr)
					{// 次に前を入れる
						pNext->SetPrev(pPrev);
					}
					// 前が有れば
					if (pPrev != nullptr)
					{// 前に次を入れる
						pPrev->SetNext(pNext);
					}

					// トップがこのオブジェクトなら
					if (m_pTop[nCntPriority] == pObjact)
					{
						// 次を先頭に入れる
						m_pTop[nCntPriority] = pNext;
					}
					// 末端がこのオブジェクトなら
					if (m_pCur[nCntPriority] == pObjact)
					{
						// 前を末端に入れる
						m_pCur[nCntPriority] = pPrev;
					}
					// オブジェクトの終了処理
					pObjact->Uninit();
					// 解放
					delete pObjact;
				}
				// 次に移行
				pObjact = pNext;
			}
		}
	}
	/// <summary>
	/// オブジェクト更新
	/// </summary>
	void CObject::UpdateAll()
	{
		// ポーズ状態取得
		const bool bPose = CManager::GetInstance()->GetSceneManager()->GetPose();
		// プライオリティ分繰り返す
		for (int nCntPriority = MIN_PRIORITY; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			// トップをオブジェクトに入れる
			CObject* pObjact = m_pTop[nCntPriority];
			// オブジェクトが無くなるまで続ける
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->m_pNext; //	次保管
				// ポーズ状態なら
				if (bPose)
				{
					// ポーズ中更新するなら
					if (pObjact->IsPoseUpdate())
					{
						pObjact->Update();
					}
				}
				// 通常時
				else
				{
					// 通常時更新するなら
					if (pObjact->IsAllUpdate())
					{
						pObjact->Update();
					}
				}
				// 次に移行
				pObjact = pNext;
			}
		}

	}
	/// <summary>
	/// オブジェクト描画
	/// </summary>
	void CObject::DrawAll()
	{
		// ポーズ状態取得
		const bool bPose = CManager::GetInstance()->GetSceneManager()->GetPose();
		// プライオリティ分繰り返す
		for (int nCntPriority = MIN_PRIORITY; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			// トップをオブジェクトに入れる
			CObject* pObjact = m_pTop[nCntPriority];
			// オブジェクトが無くなるまで続ける
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->m_pNext; //	次保管
				// ポーズ状態なら
				if (bPose)
				{
					// ポーズ中更新するなら
					if (pObjact->IsPoseDraw())
					{
						pObjact->Draw();
					}
				}
				// 通常時
				else
				{
					// 通常時更新するなら
					if (pObjact->IsAllDraw())
					{
						pObjact->Draw();
					}
				}
				// 次に移行
				pObjact = pNext;
			}
		}

	}
	/// <summary>
	/// オブジェクト取得
	/// </summary>
	/// <param name="object">先頭を入れるオブジェクトリスト</param>
	void CObject::GetAllObject(CObject* object[MAX_PRIORITY])
	{

		for (int nCnt = 0; nCnt < MAX_PRIORITY; nCnt++)
		{
			object[nCnt] = m_pTop[nCnt];
		}
	}
	/// <summary>
	/// カメラからの距離でソート
	/// </summary>
	void CObject::Sort()
	{
		for (int nCnt = 0; nCnt < MAX_PRIORITY; nCnt++)
		{
			CObject* headUnsorted;	// 前
			CObject* headSorted;	// 先頭
			CObject* max;			// 次に大きい
			CObject* prevMax;		// 前の最大
			CObject* prevComp;		// 
			//printf("リストを昇順ソートします\n");
			headUnsorted = m_pTop[nCnt];    // 未ソートリスト
			headSorted = nullptr;			// ソート済リスト 

			//------------------------------------------------------------
			// 選択ソート
			//------------------------------------------------------------
			// 
			while (headUnsorted != nullptr)
			{
				max = headUnsorted;				// 最大値要素を初期化 
				prevMax = nullptr;			// 最大値要素の前の要素を初期化 
				prevComp = headUnsorted;	// 
				//------------------------------------------------------------
				// 未ソートリストから条件を満たす最大値を探す
				//------------------------------------------------------------
				// 次がからでなければ
				while (prevComp->m_pNext != nullptr) {
					// 条件を満たす場合のみ比較

					// ビルボードなら
					if (dynamic_cast<CObjectBillbord*>(prevComp->m_pNext)) {
						// ソート条件
						//
						if ((prevComp->m_pNext)->m_fDistance < max->m_fDistance) {
							max = prevComp->m_pNext;			// 最大値を更新
							prevMax = prevComp;				// 最大値の前の要素を記録
						}
					}
					prevComp = prevComp->m_pNext;		// 次の要素に進む
				}
				// 最大値が見つからない場合、残りのリストはソート対象外
				if (max == nullptr) {//maxがNULLにならない
					break;
				}

				// 最大値を未ソートリストから削除
				if (prevMax == nullptr) {
					// 最大値がリストの先頭の場合
					headUnsorted = max->m_pNext;
				}
				else {
					// 最大値をリストから除外
					prevMax->m_pNext = max->m_pNext;
				}

				// 最大値要素をソート済リストの先頭に追加
				if (headSorted == NULL) {
					// ソート済リストが空の場合
					headSorted = max;
					m_pCur[nCnt] = max;
					max->m_pNext = NULL;
					max->m_pPrev = NULL;
				}
				else {
					// ソート済リストの先頭に追加
					max->m_pNext = headSorted;
					headSorted->m_pPrev = max;
					max->m_pPrev = NULL;
					headSorted = max;

				}
			}
			m_pTop[nCnt] = headSorted;
		}
	}
	/// <summary>
	/// カメラからの距離でソート
	/// </summary>
	void CObject::CalculateDistanceToCamera()
	{
		CManager* instance = CManager::GetInstance();
		CCamera* pCamera = instance->GetCamera();
		D3DXVECTOR3 pos = pCamera->GetPosR();

		for (int nCntPriority = MIN_PRIORITY; nCntPriority < MAX_PRIORITY; nCntPriority++)
		{
			CObject* pObjact = m_pTop[nCntPriority];
			while (pObjact != nullptr)
			{
				CObject* pNext = pObjact->m_pNext; //	次保管

				pObjact->CalculateDistance(pos);

				pObjact = pNext;
			}
		}
	}
	/// <summary>
	/// カメラからの距離を計算
	/// </summary>
	/// <param name="pos"></param>
	void CObject::CalculateDistance(D3DXVECTOR3 pos)
	{
		// カメラとの差を計算（Vec3）
		D3DXVECTOR3 diff = m_x.pos - pos;
		// ベクターから距離(float)
		m_fDistance = D3DXVec3LengthSq(&diff); // 距離の二乗を計算
	}
}