//================================
//
// ロビーデータ用のヘッダー[duel_data.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_DATA_H_
#define _DUEL_DATA_H_

//ヘッダーのインクルード
#include "raknet_data.h"
#include "duel_player.h"

//ロビーのデータ
class CDuel_Data : public CRakNet_Data
{
public:

	//関数
	CDuel_Data() : m_DuelPlayerList(), m_isCheckStart{ false,false,false,false }, m_nReceiveNum(0), m_CastCardList(), m_CastDiffenceCardVector(){} 	//コンストラクタ
	~CDuel_Data() {}										//デストラクタ

	//送受信の処理
	void NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//新しく接続する処理
	void DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//接続切断処理
	void Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;				//準備確認処理
	void SendChangedServer(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;	//サーバーが変わったことを送信する処理
	void AddStartMember() override {}															//開始メンバーの追加
	void SendStartMember(RakNet::RakPeerInterface* peer) override;								//開始メンバーの送信
	bool CheckStartBattle(RakNet::Packet* packet) override;										//対戦を開始するか
	void StartBattle(RakNet::RakPeerInterface* peer) override;									//対戦の開始
	void SendStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;			//ステータスを送る
	void AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override {}				//CPUの追加
	void ReceiveStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer) override;		//ステータスを受信
	void ReceiveCastCard(RakNet::Packet* packet) override;										//キャストカードの受信
	void ReceiveCastDefCard(RakNet::Packet* packet) override;									//キャスト守備カードの受信

	//プレイヤーのデータリスト
	void SetData(std::list<CPlayer::Data> data) override;	//設定
	std::list<CPlayer::Data> GetData() override;			//取得

private:

	//キャストカード情報
	struct CastCardInfo
	{
		int nPlayerID;					//使用者ID
		std::list<int> m_TargetIDList;	//ターゲットIDリスト
		int nCardID;					//カードID
	};

	//守備対象
	struct DiffenceTarget
	{
		int nAttackCardUserId = 0;	//攻撃カードの使用者番号
		int nTargetCard = 0;		//ターゲットのカード番号(現在のリストの中の番号)
	};

	//キャスト守備カード情報
	struct CastDiffenceCardInfo
	{
		int nCardID = 0;				//カードID
		int nUsePlayer = -1;			//使用者
		std::vector<DiffenceTarget> DiffenceTarget;	//守備対象
	};

	//定数
	static constexpr int MAX_CLIENT{ 4 };	//最大人数

	//関数
	void SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message);	//プレイヤー数を送信する関数
	bool IsSendUpdate(RakNet::Packet* packet);									//更新の許可を出すか
	void SendCastCard(RakNet::BitStream* bsout);								//キャストカードの送信
	void SendCastDeffenceCard(RakNet::BitStream* bsout);						//キャスト守備カードの送信
	void SendUpdateSign(RakNet::RakPeerInterface* peer);						//更新の合図を送る
	bool IsDisconnectionSendUpdate();											//クライアントが切断時に更新の合図を送信するか

	//変数
	std::list<My::CDuel_Player*> m_DuelPlayerList;	//ロビープレイヤー保管用変数
	bool m_isCheckStart[MAX_CLIENT];				//開始するかのフラグ
	int m_nReceiveNum;								//ステータスを受信した数
	std::list<CastCardInfo> m_CastCardList;			//キャストカードのリスト
	std::vector<CastDiffenceCardInfo> m_CastDiffenceCardVector;	//キャスト守備カードのリスト
};

#endif