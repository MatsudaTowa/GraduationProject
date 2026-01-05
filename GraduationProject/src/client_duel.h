//================================
//
// Client用のヘッダー[client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _CLIENT_DUEL_H_
#define _CLIENT_DUEL_H_

//ヘッダーのインクルード
#include "client.h"
#include "active_scene_character.h"
#include "card.h" //←バグったらここ

//Clientのクラス
class CClient_Duel : public CClient
{
public:

	//ロビープレイヤーのパラメーター
	struct DuelPlayerParam
	{
		PlayerParam Param = {};						//パラメータ
		My::CActiveSceneCharacter::Status Status = {40, 0, 0, 0, 10};	//ステータス
	};

	//関数
	CClient_Duel();					//コンストラクタ
	~CClient_Duel() override;		//デストラクタ
	bool Init() override;			//初期化処理
	void Uninit() override;			//終了処理

	//通信用関数
	void Regist(RakNet::Packet* packet) override;									//登録処理
	void Delete(RakNet::Packet* packet) override;									//削除処理
	void SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;//準備送信処理
	void ReceiveReady(RakNet::Packet* packet) override;								//準備受信処理
	void CardCast(RakNet::Packet* packet) override;									//カードのキャスト処理
	void SendStartSign(RakNet::RakPeerInterface* /*peer*/) override {}				//開始の合図を送信
	void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//コンピューターの追加
	void SendSuccessDuelLoad(RakNet::RakPeerInterface* peer) override;				//決闘読み込みの成功を送信
	void StartBattle(RakNet::Packet* packet) override;								//対戦の開始
	void SendStatus(RakNet::RakPeerInterface* peer) override;						//ステータスを送る
	void ReceiveStatus(RakNet::Packet* packet) override;							//ステータスの受信
	void SendAddEnemy(RakNet::RakPeerInterface* /*peer*/) override {}				//敵の追加を送信
	void ReceiveAddEnemy(RakNet::Packet* /*packet*/) override {}					//敵の追加を受信
	void ReceiveIsUpdate(RakNet::Packet* packet) override;							//更新可能かを受信
	void SendMyStatus(RakNet::RakPeerInterface* peer) override;						//自分のステータスを送る
	void ReceiveCastCard(RakNet::Packet* packet) override;							//キャストカードの受信
	//void ReceiveCastCard(RakNet::BitStream* bsin);								//キャストカードの受信
	//void ReceiveCastDefCard(RakNet::BitStream* bsin);								//キャスト守備カードの受信
	void ReceiveCastDefCard(RakNet::Packet* /*packet*/) override;					//キャスト守備カードの受信
	void ReceiveDrawCard(RakNet::Packet* packet) override;							//カードドローの受信
	void ReceiveTriggerCard([[maybe_unused]] RakNet::Packet* packet) override;		//トリガーカードの受信
	void ReceiveGameSet(RakNet::Packet* packet) override;							//ゲームセットの受信

	//取得
	//パラメータ
	void SetParam(std::list<PlayerParam> list) override;	//設定
	std::list<PlayerParam> GetParam() override;				//取得
	My::CCard* GetUsedCastCard(int userid, int cardid, int sametypeid);

private:

	//関数
	bool CheckEnemyCreate(int id, int max);	//敵を生成するか

	//変数
	std::list<DuelPlayerParam> m_DuelPlayerList;	//ロビープレイヤーリスト
};

#endif