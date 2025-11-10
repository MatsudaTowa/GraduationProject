//================================
//
// 防御カード用のヘッダー[attack_card.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DEFENSE_CARD_H_
#define _DEFENSE_CARD_H_

//ヘッダーのインクルード
#include "card.h"

//防御カードのクラス
class CDefense_Card : public CCard
{
public:

	//ディフェンスカードのパラメータ
	struct DefenseParam
	{
		BaseParam BaseParam = {};		// 基本パラメータ
		int nGuard = 0;					// ガード値
		int nCounter = 0;				// 反撃値
		DefenseType Defensetype;		// 守備の種類
	};

	//関数
	CDefense_Card();		//コンストラクタ
	~CDefense_Card();		//デストラクタ

	//設定と取得
	DefenseParam& GetDefenseParam() { return m_DefenseParam; }	//取得

private:

	//変数
	DefenseParam m_DefenseParam;	//防御パラメータ
};

#endif