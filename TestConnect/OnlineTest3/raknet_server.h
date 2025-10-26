//================================
//
// TCP接続のサーバー用のヘッダー[tcp_client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _RAKNET_SERVER_H_
#define _RAKNET_SERVER_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "raknet_data.h"

//RakNetサーバーのクラス
class CRakNet_Server
{
public:

	//関数
	CRakNet_Server();											//コンストラクタ
	~CRakNet_Server();											//デストラクタ
	bool Init(int nPortNum, RakNet::RakPeerInterface* peer);	//初期化処理
	void Accept(void);											//接続待ち受け処理
	void Uninit(RakNet::RakPeerInterface* peer);				//終了処理

	//静的
	static void Communication(RakNet::RakPeerInterface* peer);						//通信処理
	void Communication0(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);	//特定のクライアントの通信処理
private:

	//定数
	static constexpr int MAX_CLIENTS{ 4 };	//最大クライアント数

	//関数
	static void ChangeData(CRakNet_Data* data);	//データの変更

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	static CRakNet_Data* m_pRakNetData;	//ラックネットのデータ
};

#endif