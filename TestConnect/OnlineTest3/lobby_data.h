//================================
//
// ロビーデータ用のヘッダー[lobby_data.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _LOBBY_DATA_H_
#define _LOBBY_DATA_H_

//ヘッダーのインクルード
#include "raknet_data.h"
#include "lobby_player.h"

//ロビーのデータ
class CLobby_Data : public CRakNet_Data
{
public:
	
	//関数
	CLobby_Data() {} 	//コンストラクタ
	~CLobby_Data() {}	//デストラクタ

	//送受信の処理
	void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//新しく接続する処理
	void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//接続切断処理
	void Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;			//準備確認処理
	bool ChangeToDuel(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//対戦準備中

private:

	//関数
	void SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message);	//プレイヤー数を送信する関数

	//変数
	std::list<CLobby_Player*> m_LobbyPlayerList;	//ロビープレイヤー保管用変数
};

#endif