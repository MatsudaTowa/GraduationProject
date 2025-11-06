//================================
//
// Server用のcpp[main.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
//#include "RakNetworkFactory.h" // RakNetでは必要

#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include "raknet_server.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#define MAX_CLIENTS (4)
#define SERVER_PORT (22333)

// 構造体の定義（送信側と受信側で共有）
struct PlayerData {
    unsigned char messageId;
    float positionX;
    float positionY;
    int nPlayerID;                  //自分の番号
    RakNet::RakNetGUID RakNetID;    //接続時の番号
};

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};

//プロトタイプ宣言
//void networkThread(RakNet::RakPeerInterface* peer); //別スレッドのネットワーク更新

//=====================================
//メイン関数
//=====================================
int main(void)
{
    //RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
    //CRakNet_Server* pServer = new CRakNet_Server();
    //pServer->Init(22333, peer);

    ////終わるまで周回
    ///*while (1)
    //{
    //    
    //}*/

    //// 通信スレッドを起動
    //std::thread netThread(CRakNet_Server::Communication, peer);

    ////スレッドの切り離す
    //netThread.detach();

    //ファイルを開く
    //std::ifstream ifs("json\\cards.json");

    ////開けたかを確認
    //if (!ifs.is_open())
    //{
    //    assert(false && "ファイルを開けませんでした");
    //    return -1;
    //}

    ////文字列から直接パース
    //ordered_json Json;
    //Json = ordered_json::parse(ifs);
   
    //std::cout << Json << std::endl;  // coutに渡せば出力できる。

    CRakNet_Server a;
    a.LoadCard();

    getchar();
    rewind(stdin);

    //終了処理
    /*if (pServer != nullptr)
    {
        pServer->Uninit(peer);
        delete pServer;
        pServer = nullptr;
    }*/

	return 0;
}