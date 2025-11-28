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
My::CTargetArrowManager::CTargetArrowManager():
	m_IsOverlapped(false)
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
		SetOverlappedPos(itr,pTA);

		// 攻撃者と標的が同じだったら
		if (itr->GetAttacker() == pTA->GetAttacker()
			&& itr->GetTarget() == pTA->GetTarget())
		{
			// 寿命を再設定する
			itr->ResetLife();

			// 矢印を生成したくないので登録もせず、ここで消す
			pTA->SetisDelete(true);
			return;

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


	//if (pTA->GetArrowType() == CTargetArrow::ARROWTYPE_PLAYER)
	//{
	//	D3DXVECTOR3 pos = pTA->GetPos();
	//	pTA->SetPos(pTA->GetPos());
	//}

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

void My::CTargetArrowManager::SetOverlappedPos(CTargetArrow* itr, CTargetArrow* ptr)
{
	if (!itr->GetIsOverlapped())
	{
		// 互いに同じ攻撃者、標的だったら(逆方向)
		if (itr->GetAttacker() == ptr->GetTarget()
			&& itr->GetTarget() == ptr->GetAttacker())
		{
			// 真ん中の位置
			D3DXVECTOR2 centerpos = VEC2_RESET_ZERO;
			D3DXVECTOR2 shiftpos = VEC2_RESET_ZERO;

			// 攻撃者と標的の間の真ん中の位置を求める
			centerpos.x = std::lerp(itr->GetTarget().x, itr->GetAttacker().x, 0.5f);
			centerpos.y = std::lerp(itr->GetTarget().y, itr->GetAttacker().y, 0.5f);

			float rot = 0.0f;

			// 角度を求める
			rot = atan2f(itr->GetAttacker().y - itr->GetTarget().y, itr->GetAttacker().x - itr->GetTarget().x);	// 設定
			float a = 1.0f;

			// ずらす位置を求める
			shiftpos.x = sinf(rot - (D3DX_PI * 0.5f)) * 50.0f;
			shiftpos.y = cosf(rot - (D3DX_PI * 0.5f)) * 50.0f;

			// 
			if (itr->GetPos().x == ptr->GetPos().x)
				shiftpos.x += 50.0f;
			else if(itr->GetPos().y == ptr->GetPos().y)
				shiftpos.y += 50.0f;

			itr->SetShiftPos({ shiftpos.x,shiftpos.y });
			ptr->SetShiftPos({ -shiftpos.x,-shiftpos.y });
		}
	}
}
