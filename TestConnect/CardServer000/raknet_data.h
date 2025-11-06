//================================
//
// 送受信データクラス用のヘッダー[taknet_data.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _RAKNET_DATA_H_
#define _RAKNET_DATA_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

//ラックネットのデータ
class CRakNet_Data
{
public:
	
	//列挙
	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CARD_MESSAGE_1,			//自分が接続時
		ID_ALLCARD_MESSAGE_1,		//他人が切断した時
	};

	//関数
	CRakNet_Data() {} 	//コンストラクタ
	~CRakNet_Data() {}	//デストラクタ

	//送受信の処理
	virtual void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);					//新しく接続する処理
	virtual void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);					//接続切断処理

	//設定と取得

private:


};

#endif