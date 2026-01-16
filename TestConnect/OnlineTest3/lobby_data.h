//================================
//
// ロビーデータ用のヘッダー[lobby_data.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _LOBBY_DATA_H_
#define _LOBBY_DATA_H_

//ヘッダーのインクルード
#include "raknet_data.h"
#include "lobby_player.h"

//ロビーのデータ
class CLobby_Data : public CRakNet_Data
{
public:
	
	//関数
	CLobby_Data() :
		m_nReceiveDeckNum(0){} 	//コンストラクタ
	~CLobby_Data() {}	//デストラクタ

	//送受信の処理
	void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;			//新しく接続する処理
	void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;			//接続切断処理
	void Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;					//準備確認処理
	bool ChangeToDuel(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;				//対戦準備中
	void AddStartMember() override;																	//開始メンバーを追加
	void SendStartMember(RakNet::RakPeerInterface* peer) override {}								//開始メンバーの送信
	bool CheckStartBattle(RakNet::Packet* packet) override { return false; }						//対戦を開始するか
	void StartBattle(RakNet::RakPeerInterface* peer) override {}									//対戦の開始
	void SendStatus(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/) override {}		//ステータスを送る
	void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;					//CPUの追加
	void ReceiveStatus(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/) override {}	//ステータスを受信
	void ReceiveCastCard(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/) override {}//キャストカードの受信
	void ReceiveCastDefCard(RakNet::Packet* /*packet*/) override {}									//キャスト守備カードの受信
	void UpdateScene(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;				//シーンの更新
	bool ReceiveDeck(RakNet::Packet* packet) override;												//デッキの受信
	void ReceiveDrawCard(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/) override {}//カードのドロー処理の受信
	bool ReceiveGameSet(RakNet::Packet* /*packet*/) override { return false; }						//試合終了の受信

	//設定と取得
	bool IsPlayerExist() override;	//プレイヤーが存在するか

	//プレイヤーのデータリスト
	void SetData(std::list<CPlayer::ChangeData> data) override;	//設定
	std::list<CPlayer::ChangeData> GetData() override;			//取得

private:

	//関数
	void SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message);	//プレイヤー数を送信する関数

	//変数
	std::list<CLobby_Player*> m_LobbyPlayerList;	//ロビープレイヤー保管用変数
	int m_nReceiveDeckNum;							//受信したデッキ数
};

#endif