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
#include "card_deffence.h"

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
		ID_LOBY_MESSAGE_ADD_ENEMY,		//ロビーでの敵を追加を送信
		ID_LOBY_MESSAGE_SEND_READY,		//ロビーでの準備OKを送信
		ID_LOBY_MESSAGE_RECEIVE_READY,  //ロビーでの準備OKを受信
		ID_LOBY_MESSAGE_SEND_START,		//ロビーでの開始合図を送信
		ID_LOBY_MESSAGE_RECEIVE_START,	//ロビーでの開始合図を受信
		ID_LOBY_MESSAGE_REQUEST_DECK,	//デッキ要求のメッセージ
		ID_DUEL_MESSAGE_CLIENT_START,	//クライアントが対戦用に変わったことを通知
		ID_DUEL_MESSAGE_START,			//対戦の開始を通知
		ID_DUEL_MESSAGE_DECK,			//デッキのメッセージ
		ID_DUEL_MESSAGE_DRAW,			//カードを引くメッセージ
		//ID_DUEL_MESSAGE_SEND_STATUS,	//ステータスを送る
		ID_DUEL_MESSAGE_STATUS,			//ステータスの通知
		ID_DUEL_MESSAGE_CAST_CARD,		//キャストカードを通知
		ID_DUEL_MESSAGE_CAST_DEFCARD,	//キャスト守備カードを通知
		ID_DUEL_MESSAGE_TRIGGER,		//カードのトリガーを通知
		ID_DUEL_MESSAGE_GAMESET,		//ゲームの終了の通知
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
	void Communication(RakNet::RakPeerInterface* peer);		//通信処理
	void SendStartSign();									//開始の合図を送信
	void SendDeck();										//デッキの内容を送信
	void SendStatus();										//ステータスの送信
	void SendAddEnemy();									//敵の追加を送信

	//設定と取得
	RakNet::RakPeerInterface* GetPeer() { return m_pPeer; }	//ピアの取得

	//オンラインか
	void SetOnline(bool online) { m_isOnline = online; }	//設定
	bool GetOnline() { return m_isOnline; }					//取得

	//クライアントクラスの取得
	CClient* GetClient() { return m_Client; }

	//クライアントの変更
	static void ChangeClient(CClient* client);

	//自身のステータスを送信
	void SendMyStatus();

	//リクエスト処理
	void RequestDrawCard();														//カードを引く
	void RequestCastCard(My::CCard* castcard);									//通常カードのキャスト
	void RequestDefCastCard(My::CCardDeffence* castcard);						//守備カードのキャスト

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