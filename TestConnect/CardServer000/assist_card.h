//================================
//
// アシストカード用のヘッダー[attack_card.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _ASSIST_CARD_H_
#define _ASSIST_CARD_H_

//ヘッダーのインクルード
#include "card.h"

//アシストカードのクラス
class CAssist_Card : public CCard
{
public:

	//アシストカードのパラメータ
	struct AttackParam
	{
		BaseParam BaseParam = {};	// 基本パラメータ
		int nDamage = 0;			// ダメージ数
		AttackType Attacktype;		// アシストの種類
	};

	//関数
	CAssist_Card();			//コンストラクタ
	~CAssist_Card();		//デストラクタ

	//設定と取得
	AttackParam& GetAttackParam() { return m_AttackParam; }	//取得

private:

	//変数
	AttackParam m_AttackParam;	//アシストパラメータ
};

#endif