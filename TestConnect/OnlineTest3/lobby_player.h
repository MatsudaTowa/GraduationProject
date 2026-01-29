//================================
//
// ロビープレイヤークラス用のヘッダー[lobby_player.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _LOBBY_PLAYER_H_
#define _LOBBY_PLAYER_H_

//ヘッダーのインクルード
#include "player.h"

//ロビープレイヤークラス
class CLobby_Player : public CPlayer
{
public:

	//構造体
	struct LobbyData
	{
		Data BaceData;			//基本のクライアントデータ
		bool isReady = false;	//準備完了したか
	};

	//関数
	CLobby_Player() : m_isReady(false){} 	//コンストラクタ
	~CLobby_Player() override {}			//デストラクタ

	//設定と取得
	void SetReady(bool ready) { m_isReady = ready; }	//設定
	bool& Getready() { return m_isReady; }				//取得

private:

	//変数
	bool m_isReady;	//準備が出来ているか
};

#endif