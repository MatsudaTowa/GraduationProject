//=============================================
//
//オブジェクト[object.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include  "manager.h"

//総数初期化
int My::CObject::m_nNumAll = 0;

//オブジェクトポインタ初期化
My::CObject* My::CObject::m_pCur[My::CObject::PRI_MAX] = { nullptr };			//最後尾のポインタの初期化
My::CObject* My::CObject::m_pTop[My::CObject::PRI_MAX] = { nullptr };			//最初のポインタの初期化
/**
 * @brief コンストラクタ
 * @param [in]プライオリティ
 */
My::CObject::CObject(int nPriority):
	m_isDelete(false),
	m_isStop(false),
	m_isDraw(true)
{
	m_nPriority = nPriority; //描画優先度設定
	CObject* pObj = this;				//自身のポインタを取得
	m_nNumAll++;						//総数をカウント

	pObj->m_pPrev = m_pCur[nPriority];	//自分の前のオブジェクトポインタを取得

	//初めてのオブジェクトなら静的メンバに代入
	if (m_pTop[nPriority] == nullptr)
	{
		m_pTop[nPriority] = pObj;	//最初のオブジェクトのポインタ
		m_pCur[nPriority] = pObj;	//最後尾のオブジェクトのポインタ
		m_pCur[nPriority]->m_pNext = nullptr;	//次のポインタを初期化
	}
	else
	{
		m_pCur[nPriority]->m_pNext = pObj;	//現在の最後尾に次のポインタを設定
	}

	pObj->m_pNext = nullptr;			//次のポインタを初期化
	m_pCur[nPriority] = pObj;			//最後尾を更新
}

/**
 * @brief デストラクタ
 */
My::CObject::~CObject()
{
	m_nNumAll--;
}

/**
 * @brief 全て削除
 */
void My::CObject::ReleaseAll()
{
	//保険として先に終了処理を実行
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = m_pTop[i];	//先頭取得

		//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//次のポインタを取得

			if (pObj->m_type != CObject::OBJECT_TYPE::OBJECT_TYPE_FADE)
			{
				pObj->Uninit();					//終了処理
			}

			pObj = pNext;					//ポインタを進める
		}
	}
}

/**
 * @brief 全て更新
 */
void My::CObject::UpdateAll()
{
	//変数の宣言
	CObject* pObj = nullptr;

	//プライオリティの数だけ周回
	for (int i = 0; i < PRI_MAX; i++)
	{
		pObj = m_pTop[i];	//先頭取得

		//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//次のポインタを取得

			if (pObj->m_isStop)
			{
				continue;
			}

			pObj->Update();				//更新処理
			
			pObj = pNext;					//ポインタを進める
		}
	}

	//プライオリティの数だけ周回
	for (int i = 0; i < PRI_MAX; i++)
	{
		pObj = m_pTop[i];	//先頭取得

	//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//次のポインタを取得

			//死亡フラグが立っていたらRelease
			if (pObj->m_isDelete)
			{
				pObj->Release();
			}

			pObj = pNext;	//ポインタを進める
		}
	}
}

/**
 * @brief 全て描画
 */
void My::CObject::DrawAll()
{
	//プライオリティの数だけ周回
	for (int i = 0; i < PRI_MAX; i++)
	{
		CObject* pObj = m_pTop[i];	//先頭取得

		//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->m_pNext;	//次のポインタを取得
			pObj->Draw();					//更新処理
			pObj = pNext;					//ポインタを進める
		}
	}
}


My::CObject* My::CObject::GetTopObject(int priority)
{
	//プライオリティの範囲内なら返す
	if (priority >= 0 && priority < PRI_MAX)
	{
		return m_pTop[priority];
	}

	//範囲外ならnullptr
	return nullptr;
}

/**
 * @brief 自分自身の削除
 */
void My::CObject::Release()
{
	//変数に代入
	int nPriority = m_nPriority;

	//nullチェック
	if (this != nullptr)
	{
		CObject* pObj = this;					//自身のポインタを取得

		if (pObj->m_pPrev != nullptr)
		{
			pObj->m_pPrev->m_pNext = pObj->m_pNext;	//自身の前後を繋ぐ
		}

		if (pObj->m_pNext != nullptr)
		{
			pObj->m_pNext->m_pPrev = pObj->m_pPrev;	//自身の前後を繋ぐ
		}

		//最後尾が現在のポインタなら前のポインタに更新
		if (pObj == m_pCur[nPriority])
		{
			m_pCur[nPriority] = pObj->m_pPrev;	//ポインタの変更
		}

		//最初が現在のポインタなら次のポインタに更新
		if (pObj == m_pTop[nPriority])
		{
			m_pTop[nPriority] = pObj->m_pNext;	//ポインタの変更
		}

		//リストから削除
		delete pObj;
		pObj = nullptr;
	}
}
