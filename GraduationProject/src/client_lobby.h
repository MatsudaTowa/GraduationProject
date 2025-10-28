//================================
//
// Client用のヘッダー[client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CLIENT_LOBBY_H_
#define _CLIENT_LOBBY_H_

//ヘッダーのインクルード
#include "client.h"

//Clientのクラス
class CClient_Lobby : public CClient
{
public:

	//ロビープレイヤーのパラメーター
	struct LobbyPlayerParam
	{
		PlayerParam Param = {};		//パラメータ
		bool isReady = false;		//準備できているか
	};

	//関数
	CClient_Lobby();					//コンストラクタ
	~CClient_Lobby() override;			//デストラクタ
	bool Init() override;				//初期化処理
	void Uninit() override;				//終了処理

	//通信用関数
	void Regist(RakNet::Packet* packet) override;										//登録処理
	void Delete(RakNet::Packet* packet) override;										//削除処理
	void SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//準備送信処理
	void ReceiveReady(RakNet::Packet* packet) override;									//準備受信処理
	void CardCast(RakNet::Packet* packet) override;										//カードのキャスト処理
	void SendStartSign(RakNet::RakPeerInterface* peer) override;						//開始の合図を送信
	void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* /*peer*/) {}			//コンピューターの追加
	void SendSuccessDuelLoad(RakNet::RakPeerInterface* /*peer*/) override {}			//決闘読み込みの成功を送信
	void StartBattle() override {}														//対戦の開始
	void SendStatus(RakNet::RakPeerInterface* peer) override {}							//ステータスを送る
	void ReceiveStatus(RakNet::Packet* packet) override {}								//ステータスの受信

	//取得
	void SetParam(std::list<PlayerParam> list) override;	//設定
	std::list<PlayerParam> GetParam() override;				//取得

private:

	//関数
	bool CheckEnemyCreate(int id, int max);	//敵を生成するか

	//変数
	std::list<LobbyPlayerParam> m_LobbyPlayerList;	//ロビープレイヤーリスト
};

#endif