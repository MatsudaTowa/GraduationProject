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
#include "character.h"

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
	virtual void Regist(RakNet::Packet* packet);										//登録処理
	virtual void Delete(RakNet::Packet* packet);										//削除処理
	virtual void SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);		//準備送信処理
	virtual void ReceiveReady(RakNet::Packet* packet);									//準備受信処理
	virtual void CardCast(RakNet::Packet* packet);										//カードのキャスト処理
	virtual void SendStartSign(RakNet::RakPeerInterface* peer) = 0;						//開始の合図を送信
	virtual void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;	//コンピューターの追加
	virtual void SendSuccessDuelLoad(RakNet::RakPeerInterface* peer) = 0;				//決闘読み込みの成功を送信
	virtual void StartBattle() = 0;														//対戦の開始
	virtual void SendStatus(RakNet::RakPeerInterface* peer) = 0;						//ステータスを送る
	virtual void ReceiveStatus(RakNet::Packet* packet) = 0;								//ステータスを受信
	virtual void SendAddEnemy(RakNet::RakPeerInterface* peer) = 0;						//敵の追加を送信

	//設定と取得

	//パラメータ
	virtual void SetParam(std::list<PlayerParam> list) = 0;	//設定
	virtual std::list<PlayerParam> GetParam() = 0;			//取得

private:

	//関数
	bool CheckEnemyCreate(int id, int max);	//敵を生成するか

	//変数
	//std::list<PlayerParam> m_PlayerParamList;	//プレイヤーリスト
};

#endif