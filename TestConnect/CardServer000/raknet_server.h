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
#include "card.h"

//RakNetサーバーのクラス
class CRakNet_Server
{
public:

	//関数
	CRakNet_Server();			//コンストラクタ
	~CRakNet_Server();			//デストラクタ
	bool Init(int nPortNum);	//初期化処理
	void Accept(void);			//接続待ち受け処理
	void Uninit();				//終了処理
	void LoadCard();			//カード情報の読み込み

	//静的
	void Communication();		//通信処理
	void Communication0(RakNet::RakPeerInterface* peer, RakNet::Packet* packet);	//特定のクライアントの通信処理
private:

	//定数
	static constexpr int MAX_CLIENTS{ 99 };	//最大クライアント数

	//関数
	static void ChangeData(CRakNet_Data* data);	//データの変更

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	RakNet::RakPeerInterface* m_pPeer;	//ピア
	static CRakNet_Data* m_pRakNetData;	//ラックネットのデータ
	//std::vector<CCard> m_CardVector;	//カードのベクター
};

#endif