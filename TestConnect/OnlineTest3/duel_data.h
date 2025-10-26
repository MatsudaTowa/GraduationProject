//================================
//
// ロビーデータ用のヘッダー[duel_data.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_DATA_H_
#define _DUEL_DATA_H_

//ヘッダーのインクルード
#include "raknet_data.h"
#include "duel_player.h"

//ロビーのデータ
class CDuel_Data : public CRakNet_Data
{
public:

	//関数
	CDuel_Data() {} 	//コンストラクタ
	~CDuel_Data() {}	//デストラクタ

	//送受信の処理
	void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//新しく接続する処理
	void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//接続切断処理
	void Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;				//準備確認処理
	void SendChangedServer(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//サーバーが変わったことを送信する処理

private:

	//関数
	void SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message);	//プレイヤー数を送信する関数

	//変数
	std::list<CDuel_Player*> m_DuelPlayerList;	//ロビープレイヤー保管用変数
};

#endif