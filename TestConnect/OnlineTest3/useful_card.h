//=============================================
//
//実用的なカード処理をまとめたもの[useful_card.h]
//Author shimadu yuto
//
//============================================= 

//２重インクルード防止
#ifndef _USEFUL_CARD_H_ //これが定義されてないとき
#define _USEFUL_CARD_H_

//インクルード
#include "main.h"
#include "card.h"
#include "duel_player.h"

//ターゲットプレイヤーのベクターを取得
std::vector<My::CDuel_Player*> GetTargetPlayer(std::vector<int> targetid);

#endif