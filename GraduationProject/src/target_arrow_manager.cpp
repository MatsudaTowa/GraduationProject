//===========================================================================================================================================================
// 
// ターゲットアローのリスト [target_arrow_manager.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "target_arrow_manager.h"
#include "target_arrow.h"

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CTargetArrowManager::CTargetArrowManager()
{
	m_pTargetArrowList.clear();
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CTargetArrowManager::~CTargetArrowManager()
{
	m_pTargetArrowList.clear();
}

//===========================================================================================================
// 登録
//===========================================================================================================
void My::CTargetArrowManager::Regist(CTargetArrow* pTA)
{
	/** 処理メモ
	* 
	* 同じターゲット、攻撃者のポインタは登録しない
	* ターゲットか攻撃者どちらかが違えば登録する
	* 上書きはできない(nullptrでなければならない)
	*/

	for (auto& itr : m_pTargetArrowList)
	{
		// 攻撃者と標的が同じだったら
		if (itr->GetAttacker() == pTA->GetAttacker()
			&& itr->GetTarget() == pTA->GetTarget())
		{
			// 寿命を再設定する
			itr->ResetLife();

			// イテレーターが最後まで回ったら
			if (itr == m_pTargetArrowList.back())
			{
				pTA->SetisDelete(true);
				return;
			}
			continue;
		}
		else
		{
			// イテレーターが最後まで回ったら
			if (itr == m_pTargetArrowList.back())
			{
				// リストに登録まで飛ぶ
				break;
			}

			// まだイテレーターを回す
			continue;
		}		
	}

	if (pTA->GetArrowType() == CTargetArrow::ARROWTYPE_PLAYER)
	{
		D3DXVECTOR3 pos = pTA->GetPos();
		pTA->SetPos(pTA->GetPos());
	}

	// リストに登録
	m_pTargetArrowList.push_back(pTA);
	
}

//===========================================================================================================
// 削除
//===========================================================================================================
void My::CTargetArrowManager::Remove()
{
	unsigned int i = 0;
	for (auto& itr : m_pTargetArrowList)
	{
		// nullptr じゃなかったら
		if (itr != nullptr)
		{
			// 寿命が０になったら
			if (itr->GetLife() <= 0)
			{
				// 矢印の削除
				itr->SetisDelete(true);

				// リストから削除
				m_pTargetArrowList.erase(std::next(m_pTargetArrowList.begin(), i));

				return;
			}
			else
			{// イテレーターの番号を進める
				i++;
			}
		}
	}
}

//===========================================================================================================
// ターゲットアローリストの取得
//===========================================================================================================
std::list < My::CTargetArrow* > My::CTargetArrowManager::GetTargetArrow()
{
	return m_pTargetArrowList;
}
