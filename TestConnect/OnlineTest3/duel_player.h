//================================
//
// デュエルプレイヤークラス用のヘッダー[duel_player.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_PLAYER_H_
#define _DUEL_PLAYER_H_

//ヘッダーのインクルード
#include "player.h"

//デュエルプレイヤークラス
class CDuel_Player : public CPlayer
{
public:

	//構造体
	struct DuelData
	{
		Data BaceData;			//基本のクライアントデータ
		//bool isReady = false;	//準備完了したか
	};

	//関数
	CDuel_Player() {} 	//コンストラクタ
	~CDuel_Player() {}	//デストラクタ

private:

	//変数
	//TODOここに対戦中に受け取る変数を追加
};

#endif