//=============================================
//
//何Pかの表示UI[damage_number_UI.cpp]
//Author Matsuda Towa
//
//=============================================
#include "damage_number_UI_factory.h"
#include "damage_number_UI.h"

//=============================================
// コンストラクタ
//=============================================
My::CDamageNumberUIFactory::CDamageNumberUIFactory()
{
}

//=============================================
// デストラクタ
//=============================================
My::CDamageNumberUIFactory::~CDamageNumberUIFactory()
{
}

//=============================================
// 生成
//=============================================
void My::CDamageNumberUIFactory::Create(D3DXVECTOR3 pos, int damage)
{
	//変数
	int nDigitNum = 1;		//桁数
	int nDamage = damage;	//桁を調べる用に数値をコピー
	float fSpace = 0.0f;	//中心値から右に動かす値
	int maxdamage = damage;

	//桁を確認
	while (nDamage >= 10)
	{
		nDigitNum++;
		nDamage /= 10;
	}

	//右に動かす値を算出
	fSpace = (SPACE * 0.5f) * (nDigitNum - 1);

	//ダメージの桁数分ずらす(1桁目から生成)
	for (int i = 0; i < nDigitNum; i++)
	{
		//数値の計算
		int nNum = damage % 10;	//数値を算出
		damage /= 10;			//桁を減らす

		//ダメージ数値UIの生成
		CDamageNumberUI::Create({ pos.x + fSpace, pos.y, pos.z }, nNum, maxdamage);

		//数字の隙間分左に動かす
		fSpace -= SPACE;
	}
}
