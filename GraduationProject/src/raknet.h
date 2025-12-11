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

	//更新の許可
	void SetIsUpdate(bool update) { m_isUpdate = update; }	//設定
	bool GetIsUpdate() { return m_isUpdate; }				//取得

	//クライアントクラスの取得
	CClient* GetClient() { return m_Client; }

	//クライアントの変更
	static void ChangeClient(CClient* client);

	//自身のステータスを送信
	void SendMyStatus();

	//リクエスト処理
	void RequestDrawCard();												//カードを引く
	void RequestCastCard(int usecardid, std::vector<int> target);		//通常カードのキャスト
	void RequestDefCastCard(int usecardid, int targetid, int target);	//守備カードのキャスト

	//キャストカードの送信
	template<typename... Args>
	void SendCastCard(int cardid, int playerid, Args... args)
	{
		//データの作成
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_CAST_CARD);    //メッセージ
		bsOut.Write(cardid);														//カード番号
		bsOut.Write(playerid);														//使用者番号

		//対象者の数を確認し書き出し
		int nLength = sizeof...(args);
		bsOut.Write(nLength);

		//攻撃対象の書き出し
		for (int Target : std::initializer_list<int>{ args... }) 
		{
			bsOut.Write(Target);													//対象者者番号
		}

		//サーバーに送信
		RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

		//サーバーの確認
		if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
		{
			//サーバーにブロードキャスト
			m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
		}
	}

	//キャスト守備カードの送信
	void SendCastDefCard(int cardid, int playerid, std::vector<My::CCardDeffence::DiffenceTarget> target)
	{
		//データの作成
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_CAST_DEFCARD);    //メッセージ
		bsOut.Write(cardid);														//カード番号
		bsOut.Write(playerid);														//使用者番号

		//対象者の数を確認し書き出し
		int nLength = (int)target.size();
		bsOut.Write(nLength);

		//攻撃対象の書き出し
		for (auto Target : target)
		{
			bsOut.Write(Target.nAttackCardUserId);	//対象者番号
			bsOut.Write(Target.nTargetCard);		//カード番号
		}

		//サーバーに送信
		RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

		//サーバーの確認
		if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
		{
			//サーバーにブロードキャスト
			m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
		}
	}

private:

	//定数
	static constexpr int PORT{ 22333 };	//ポート番号

	//関数
	CRakNet();			//コンストラクタ

	//メンバ変数
	RakNet::Packet* m_pPacket;			//パケット
	RakNet::RakPeerInterface* m_pPeer;	//ピア(接続用)
	bool m_isOnline;					//オンラインか
	bool m_isUpdate;					//更新しても良いか

	//静的変数
	static CClient* m_Client;			//クライアントクラス
};

#endif