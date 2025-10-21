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
	CClient();			//コンストラクタ
	~CClient();			//デストラクタ
	bool Init();		//初期化処理
	void Uninit();		//終了処理

	//通信用関数
	void Regist(RakNet::Packet* packet);	//登録処理
	void Delete(RakNet::Packet* packet);	//削除処理

private:

	//関数
	bool CheckEnemyCreate(int id, int max);	//敵を生成するか

	//変数
	std::list<PlayerParam> m_PlayerParamList;	//プレイヤーリスト
	int m_nMyID;								//自身の番号
};

#endif