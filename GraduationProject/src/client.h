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
		My::CCharacter::TAG Tag;		//タグ
	};

	//関数
	CClient();					//コンストラクタ
	virtual ~CClient();			//デストラクタ
	virtual bool Init();		//初期化処理
	virtual void Uninit();		//終了処理

	//通信用関数
	virtual void Regist([[maybe_unused]] RakNet::Packet* packet);										//登録処理
	virtual void Delete([[maybe_unused]] RakNet::Packet* packet);										//削除処理
	virtual void SendReady([[maybe_unused]] RakNet::Packet* packet, RakNet::RakPeerInterface* peer);	//準備送信処理
	virtual void ReceiveReady([[maybe_unused]] RakNet::Packet* packet);									//準備受信処理
	virtual void CardCast([[maybe_unused]] RakNet::Packet* packet);										//カードのキャスト処理
	virtual void SendStartSign([[maybe_unused]] RakNet::RakPeerInterface* peer) = 0;					//開始の合図を送信
	virtual void AddCPU([[maybe_unused]] RakNet::Packet* packet, RakNet::RakPeerInterface* peer) = 0;	//コンピューターの追加
	virtual void SendSuccessDuelLoad([[maybe_unused]] RakNet::RakPeerInterface* peer) = 0;				//決闘読み込みの成功を送信
	virtual void StartBattle() = 0;																		//対戦の開始
	virtual void SendStatus([[maybe_unused]] RakNet::RakPeerInterface* peer) = 0;						//ステータスを送る
	virtual void ReceiveStatus([[maybe_unused]] RakNet::Packet* packet) = 0;							//ステータスを受信
	virtual void SendAddEnemy([[maybe_unused]] RakNet::RakPeerInterface* peer) = 0;						//敵の追加を送信
	virtual void ReceiveAddEnemy([[maybe_unused]] RakNet::Packet* packet) = 0;							//敵の追加を受信
	virtual void ReceiveIsUpdate([[maybe_unused]] RakNet::Packet* packet) = 0;							//更新可能かを受信する
	virtual void SendMyStatus([[maybe_unused]] RakNet::RakPeerInterface* peer) = 0;						//自分のステータスを送る
	virtual void ReceiveCastCard([[maybe_unused]] RakNet::Packet* packet) = 0;							//キャストカードの受信
	virtual void ReceiveCastDefCard([[maybe_unused]] RakNet::Packet* packet) = 0;						//キャスト守備カードの受信
	void ReceiveDeck(RakNet::Packet* packet);															//デッキの受信
	virtual void ReceiveDrawCard([[maybe_unused]] RakNet::Packet* packet) = 0;							//カードドローの受信

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