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
#include "player.h"

//ラックネットのデータ
class CRakNet_Data
{
public:

	//シーンの状態
	enum SCENE_STATE
	{
		SCENE_NONE = 0,
		SCENE_LOBBY,
		SCENE_DUEL,
		SCENE_MAX
	};
	
	//列挙
	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CONNECT_MESSAGE_1,			//自分が接続時
		ID_DISCONNECT_MESSAGE_1,		//他人が切断した時
		ID_LOBY_MESSAGE_ADD_ENEMY,		//ロビーでの敵を追加を送信
		ID_LOBY_MESSAGE_SEND_READY,		//ロビーでの準備OKを送信
		ID_LOBY_MESSAGE_RECEIVE_READY,  //ロビーでの準備OKを送信
		ID_LOBY_MESSAGE_SEND_START,		//ロビーでの開始合図を送信
		ID_LOBY_MESSAGE_RECEIVE_START,	//ロビーでの開始合図を受信
		ID_LOBY_MESSAGE_REQUEST_DECK,	//デッキ要求のメッセージ
		ID_DUEL_MESSAGE_CLIENT_START,	//クライアントが対戦用に変わったことを通知
		ID_DUEL_MESSAGE_START,			//対戦の開始を通知
		ID_DUEL_MESSAGE_DECK,			//デッキのメッセージ
		//ID_DUEL_MESSAGE_SEND_STATUS,	//ステータスを送る
		ID_DUEL_MESSAGE_STATUS,			//ステータスの通知
		ID_DUEL_MESSAGE_CAST_CARD,		//キャストカードを通知
		ID_DUEL_MESSAGE_CAST_DEFCARD,	//キャスト守備カードを通知
		ID_DUEL_MESSAGE_1,				//対戦時のメッセージ
	};

	//関数
	CRakNet_Data()/* : m_PlayerParamList()*/ { /*m_PlayerParamList.clear();*/ } 		//コンストラクタ
	~CRakNet_Data() {}	//デストラクタ

	//送受信の処理
	virtual void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);					//新しく接続する処理
	virtual void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);					//接続切断処理
	virtual void Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer);							//準備確認処理
	virtual bool ChangeToDuel(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) { return false; }	//対戦に変更
	virtual void SendChangedServer(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) {}			//サーバーが変わったことを送信
	virtual void AddStartMember() = 0;																	//開始メンバーの追加
	virtual void SendStartMember(RakNet::RakPeerInterface* peer) = 0;									//開始メンバーの送信
	virtual bool CheckStartBattle(RakNet::Packet* packet) = 0;											//対戦を開始するか
	virtual void StartBattle(RakNet::RakPeerInterface* peer) = 0;										//対戦の開始
	virtual void SendStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;				//ステータスを送る
	virtual void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;					//CPUの追加
	virtual void ReceiveStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;				//ステータスを受信
	virtual void ReceiveCastCard(RakNet::Packet* packet) = 0;											//キャストカードの受信
	virtual void ReceiveCastDefCard(RakNet::Packet* packet) = 0;										//キャスト守備カードの受信
	virtual void UpdateScene(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;				//シーンの更新
	void SendRequestDeck(RakNet::RakPeerInterface* peer);												//デッキの要求

	//設定と取得

	//プレイヤーのデータリスト
	virtual void SetData(std::list<CPlayer::ChangeData> data) = 0;	//設定
	virtual std::list<CPlayer::ChangeData> GetData() = 0;			//取得
	virtual bool ReceiveDeck(RakNet::Packet* packet);				//デッキの受信

private:

	//関数
	void SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message);	//プレイヤー数を送信する関数
};

#endif