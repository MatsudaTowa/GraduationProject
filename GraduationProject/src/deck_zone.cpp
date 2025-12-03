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
My::CDeckZone::CDeckZone()
{

}

/**
* @brief デストラクタ
*/
My::CDeckZone::~CDeckZone()
{

}

/**
* @brief めくる処理
* @param [in]枚数
*/
std::list<My::CCard*> My::CDeckZone::Flip(int sheet)
{
    std::list<CCard*> card_list;
    card_list.clear();

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
        card_list.push_back(card);
    }

    //TODO:のちのちめくったカードを選択する処理を追加
    return card_list;
}

/**
* @brief シャッフル処理
*/
void My::CDeckZone::Shuffle()
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

/**
* @brief 山札切れになるか
*/
bool My::CDeckZone::CheckDeckOut()
{
    return GetList().empty();
}
