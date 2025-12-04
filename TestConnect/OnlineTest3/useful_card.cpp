//================================
//
// 実用的なカード処理をまとめたもの[useful_card.cpp]
// Author:shimadu yuto
//
//================================

//ヘッダーのインクルード
#include "useful_card.h"
#include "duel_player_manager.h"
#include<iostream>
#include<algorithm>

//=====================================
//ターゲットプレイヤーのベクターを取得
//=====================================
std::vector<My::CDuel_Player*> GetTargetPlayer(std::vector<int> targetid)
{
	//対象を保管するベクター
	std::vector<My::CDuel_Player*> TargetVector;

	//番号ソート
	std::sort(targetid.begin(), targetid.end());
	
	//周回数
	int nCount = 0;

	//番号を周回
	for (auto index : targetid)
	{
		//周回
		for (auto& iter : My::CDuel_Player_Manager::GetInstance()->GetList())
		{
			//周回数より小さい番号は飛ばす
			if (index < nCount) continue;

			//対象の番号と同じ番号を見つける
			if (index == iter->GetIndex())
			{
				TargetVector.push_back(iter);
				break;
			}

			//カウントを増やす
			nCount++;
		}

		//カウントを増やす
		nCount++;
	}

	return TargetVector;
}