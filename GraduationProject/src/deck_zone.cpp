//===========================================================================================================================================================
// 
// ゾーンの処理 [deck.cpp]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

// include
#include "deck_zone.h"

/**
* @brief コンストラクタ
*/
My::CDeck::CDeck()
{

}

/**
* @brief デストラクタ
*/
My::CDeck::~CDeck()
{

}

/**
* @brief めくる処理
* @param [in]枚数
*/
void My::CDeck::Flip(int sheet)
{
    // 変数の初期化
    int count = 0;

    for (CCard* card : GetList())
    {
        if (count >= sheet)
        {// めくり切ったら
            break;
        }

        if (card)
        {
            SetCardFaceUp(card);

            ++count;
        }
    }

    //TODO:のちのちめくったカードを選択する処理を追加

}

/**
* @brief シャッフル処理
*/
void My::CDeck::Shuffle()
{
    std::list<CCard*>& list = GetListRef();

    std::vector<CCard*> temp(list.begin(), list.end());

    // シードの生成
    std::random_device rd;
    std::mt19937 g(rd());

    // シャッフルさせる
    std::shuffle(temp.begin(), temp.end(), g);

    // シャッフル結果を代入
    list.assign(temp.begin(), temp.end());

}
