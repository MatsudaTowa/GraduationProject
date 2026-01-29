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

//=====================================
//メイン関数
//=====================================
int main(void)
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

    RakNet::RakPeerInterface* peer = RakNet::RakPeerInterface::GetInstance();
    //CRakNet_Server* pServer = new CRakNet_Server();

    //サーバーの初期化と通信
    CRakNet_Server::GetInstance()->Init(22333, peer);
    CRakNet_Server::GetInstance()->Communication(peer);
   
    //終了処理
    CRakNet_Server::GetInstance()->Uninit(peer);
    peer = nullptr;
	return 0;
}