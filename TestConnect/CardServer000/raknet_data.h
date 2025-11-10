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
#include "card.h"

//ラックネットのデータ
class CRakNet_Data
{
public:
	
	//列挙
	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CARD_MESSAGE_1,			//カード情報の送信
		ID_ALLCARD_MESSAGE_1,		//全カードの情報を送信
	};

	//関数
	CRakNet_Data(); 	//コンストラクタ
	~CRakNet_Data() {}	//デストラクタ

	//カードの読み込み
	void LoadCard();

	//送受信の処理
	virtual void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);	//新しく接続する処理
	virtual void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);	//接続切断処理
	void SendCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);				//カードの送信処理
	void SendAllCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);			//全てカードの送信処理

	//設定と取得

private:

	//変数
	std::vector<CCard> m_CardVector;	//カードのベクター
};

#endif