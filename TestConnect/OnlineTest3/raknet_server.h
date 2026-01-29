//================================
//
// TCP接続のサーバー用のヘッダー[tcp_client.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _RAKNET_SERVER_H_
#define _RAKNET_SERVER_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "raknet_data.h"
#include "card.h"

//RakNetサーバーのクラス
class CRakNet_Server
{
public:

	//関数
	~CRakNet_Server();											//デストラクタ
	bool Init(int nPortNum, RakNet::RakPeerInterface* peer);	//初期化処理
	void Accept(void);											//接続待ち受け処理
	void Uninit(RakNet::RakPeerInterface* peer);				//終了処理

	//静的
	void Communication(RakNet::RakPeerInterface* peer);								//通信処理
	
	//シングルトン
	static CRakNet_Server* GetInstance()
	{
		static CRakNet_Server instance; //静的インスタンス
		return &instance;
	}

	//情報の送信
	void SendStatus();						//ステータスの送信
	void SendCastCard(My::CCard* Card);		//キャストカードの送信
	void SendTriggerCard(My::CCard* Card);	//トリガーカードの送信
	void SendGameSet();						//ゲーム終了の送信

private:

	//定数
	static constexpr int MAX_CLIENTS{ 4 };	//最大クライアント数

	//関数
	CRakNet_Server();							//コンストラクタ
	static void ChangeData(CRakNet_Data* data);	//データの変更
	bool IsSendGameSet();						//終了の合図を送るか
	void Reset();								//サーバーの状態をリセット
	void ReceiveTime(RakNet::Packet* packet);	//時間の受信

	//メンバ変数
	RakNet::Packet* m_pPacket;				//パケット
	RakNet::RakPeerInterface* m_pPeer;		//ピア
	static CRakNet_Data* m_pRakNetData;		//ラックネットのデータ
	bool m_isUpdate;						//更新フラグ
	CRakNet_Data::SCENE_STATE m_SceneState;	//シーンの状態
	RakNet::Time m_deltaTime;				//前回の更新から次の更新までにかかった時間
};

#endif