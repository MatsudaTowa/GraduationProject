//================================
//
// 攻撃カード用のヘッダー[attack_card.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _ATTACK_CARD_H_
#define _ATTACK_CARD_H_

//ヘッダーのインクルード
#include "card.h"

//攻撃カードのクラス
class CAttack_Card : public CCard
{
public:

	//アタックカードのパラメータ
	struct AttackParam
	{
		BaseParam BaseParam = {};	// 基本パラメータ
		int nDamage = 0;			// ダメージ数
		AttackType Attacktype;		// 攻撃の種類
	};

	//関数
	CAttack_Card();			//コンストラクタ
	~CAttack_Card();		//デストラクタ

	//設定と取得
	AttackParam& GetAttackParam() { return m_AttackParam; }	//取得

private:

	//変数
	AttackParam m_AttackParam;	//攻撃パラメータ
};

#endif