//================================
//
// カードクライアント用のヘッダー[card_client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CARD_CLIENT_H_
#define _CARD_CLIENT_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "raknet_data.h"

//カードクライアントのクラス
class CCard_Client
{
public:

	//列挙
	enum GAME_MESSAGE
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CARD_MESSAGE_1,			//カード情報の送信
		ID_ALLCARD_MESSAGE_1,		//全カードの情報を送信
	};

	//関数
	CCard_Client();												//コンストラクタ
	~CCard_Client();											//デストラクタ
	bool Init(int nPortNum, RakNet::RakPeerInterface* peer);	//初期化処理
	void Uninit(RakNet::RakPeerInterface* peer);				//終了処理

	//通信
	void Communication(RakNet::RakPeerInterface* peer);			//通信処理
	void RequestCard(RakNet::RakPeerInterface* peer, int id);	//カードリクエスト
	void RequestAllCard(RakNet::RakPeerInterface* peer);		//全カードリクエスト
	void ReceiveCardInfo(RakNet::Packet* packet);				//カード情報の受信
	
private:

	//定数
	static constexpr int PORT{ 22334 };	//ポート番号

	//関数
	void Send(RakNet::RakPeerInterface* peer, RakNet::BitStream* out);	//送信処理

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	RakNet::RakPeerInterface* m_pPeer;	//ピア(接続用)
	static CRakNet_Data* m_pRakNetData;	//ラックネットのデータ
};

#endif