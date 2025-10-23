//================================
//
// RakNet用のヘッダー[raknet.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _RAKNET_H_
#define _RAKNET_H_

//ヘッダーのインクルード
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "client.h"

//RakNetサーバーのクラス
class CRakNet
{
public:

	//列挙
	//enum GameMessages
	//{
	//	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
	//	ID_CONNECT_MESSAGE_1,   //自分が接続時
	//	ID_CONNECT_MESSAGE_2,   //他人が接続時
	//};

	//列挙
	enum GameMessages
	{
		ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1,
		ID_CONNECT_MESSAGE_1,			//自分が接続時
		ID_DISCONNECT_MESSAGE_1,		//他人が接続切断
		ID_LOBY_MESSAGE_SEND_READY,		//ロビーでの準備OKを送信
		ID_LOBY_MESSAGE_RECEIVE_READY,  //ロビーでの準備OKを受信
		ID_LOBY_MESSAGE_SEND_START,		//ロビーでの開始合図を送信
		ID_LOBY_MESSAGE_RECEIVE_START,	//ロビーでの開始合図を受信
		ID_DUEL_MESSAGE_1,				//対戦時のメッセージ
	};

	//シングルトン
	static CRakNet* GetInstance()
	{
		static CRakNet instance; //静的インスタンス
		return &instance;
	}

	//関数
	~CRakNet();			//デストラクタ
	bool Init();		//初期化処理
	void Accept();		//接続待ち受け処理
	void Uninit();		//終了処理
	static void Communication(RakNet::RakPeerInterface* peer);		//通信処理
	static void DuelComunication(RakNet::RakPeerInterface* peer);	//対戦中の通信処理

	//設定と取得
	RakNet::RakPeerInterface* GetPeer() { return m_pPeer; }	//ピアの取得

	//オンラインか
	void SetOnline(bool online) { m_isOnline = online; }	//設定
	bool GetOnline() { return m_isOnline; }					//取得

	//クライアントクラスの取得
	CClient* GetClient() { return m_Client; }

private:

	//定数
	static constexpr int PORT{ 22333 };	//ポート番号

	//関数
	CRakNet();			//コンストラクタ

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	RakNet::RakPeerInterface* m_pPeer;	//ピア(接続用)
	bool m_isOnline;					//オンラインか

	//静的変数
	static CClient* m_Client;			//クライアントクラス
};

#endif