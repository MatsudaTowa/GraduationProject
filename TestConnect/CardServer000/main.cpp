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

#define MAX_CLIENTS (10)
#define SERVER_PORT (22334)

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
    CRakNet_Server a;
    a.LoadCard();
    a.Init(SERVER_PORT);
    a.Communication();

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