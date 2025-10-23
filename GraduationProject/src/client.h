//================================
//
// Client用のヘッダー[client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CLIENT_H_
#define _CLIENT_H_

//ヘッダーのインクルード
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

//Clientのクラス
class CClient
{
public:

	//プレイヤーのパラメーター
	struct PlayerParam
	{
		RakNet::RakNetGUID ClientID;	//プレイヤー判別用のID
		int nIndex;						//プレイヤー番号
	};

	//関数
	CClient();					//コンストラクタ
	virtual ~CClient();			//デストラクタ
	virtual bool Init();		//初期化処理
	virtual void Uninit();		//終了処理

	//通信用関数
	virtual void Regist(RakNet::Packet* packet);		//登録処理
	virtual void Delete(RakNet::Packet* packet);		//削除処理
	virtual void SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);		//準備送信処理
	virtual void ReceiveReady(RakNet::Packet* packet);	//準備受信処理
	virtual void CardCast(RakNet::Packet* packet);		//カードのキャスト処理

private:

	//関数
	bool CheckEnemyCreate(int id, int max);	//敵を生成するか

	//変数
	//std::list<PlayerParam> m_PlayerParamList;	//プレイヤーリスト
};

#endif