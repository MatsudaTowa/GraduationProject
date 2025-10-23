//================================
//
// RakNet接続のサーバー用のcpp[raknet.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet.h"
#include "client_lobby.h"

//静的変数宣言
CClient* CRakNet::m_Client = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet::CRakNet() :
    m_pPacket(nullptr),	 //パケット
    m_pPeer(nullptr),    //ピア
    m_isOnline(false)    //オンラインか
{
   
}

//=====================================
//デストラクタ
//=====================================
CRakNet::~CRakNet()
{

}

//=====================================
//初期化処理
//=====================================
bool CRakNet::Init()
{
    if (!m_isOnline) return false;

    m_pPeer = RakNet::RakPeerInterface::GetInstance();

    //ピアの取得
    RakNet::SocketDescriptor sd;
    m_pPeer->Startup(1, &sd, 1);
    // ブロードキャストでサーバーを探す
    m_pPeer->Ping("255.255.255.255", PORT, false);
    
    //クライアントクラスの作成
    if (m_Client == nullptr)
    {
        m_Client = new CClient_Lobby;
    }

    return true;
}

//=====================================
//接続受付の処理
//=====================================
void CRakNet::Accept(void)
{

}

//=====================================
//終了処理
//=====================================
void CRakNet::Uninit()
{
    //接続の破棄
    if (m_pPeer != nullptr)
    {
        m_pPeer->Shutdown(300); //300ミリ秒後破棄
        RakNet::RakPeerInterface::DestroyInstance(m_pPeer);
    }

    //クラスの破棄
    if (m_Client != nullptr)
    {
        delete m_Client;
        m_Client = nullptr;
    }
}

//=====================================
//通信処理
//=====================================
void CRakNet::Communication(RakNet::RakPeerInterface* peer)
{
    //変数宣言
    RakNet::Packet* packet;

    //受信処理
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION:
            std::cout << "Another client has disconnected.\n";
            break;
        case ID_REMOTE_CONNECTION_LOST:
            std::cout << "Another client has lost the connection.\n";
            break;
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            std::cout << "Another client has connected.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";
            break;
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "A new client is connecting.\n";
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "The server is full.\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "We have been disconnected.\n";
            break;
        case ID_CONNECTION_LOST:
            std::cout << "Connection lost.\n";
            break;
        case ID_GAME_MESSAGE_1:

        break;
        case ID_CONNECT_MESSAGE_1:
        
            //登録処理
            m_Client->Regist(packet);
            break;

        case ID_DISCONNECT_MESSAGE_1:
        
            //削除処理
            m_Client->Delete(packet);
            break;

        case ID_LOBY_MESSAGE_SEND_READY:
        
            //準備OKの合図を送る処理
            m_Client->SendReady(packet, peer);
            break;

        case ID_LOBY_MESSAGE_RECEIVE_READY:
        
            //準備OKの合図を受信する処理処理
            m_Client->ReceiveReady(packet);
        

        break;
        case ID_UNCONNECTED_PONG:
        {
            RakNet::SystemAddress serverAddress = packet->systemAddress;
            std::cout << "Server found at: " << serverAddress.ToString() << std::endl;

            // サーバーに接続
            peer->Connect(serverAddress.ToString(false), CRakNet::PORT, 0, 0);
            std::cout << "Connecting to " << serverAddress.ToString() << "...\n";
            break;
        }

        default:
            std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

//=====================================
//対戦中の通信処理
//=====================================
void CRakNet::DuelComunication(RakNet::RakPeerInterface* peer)
{
    //変数宣言
    RakNet::Packet* packet;

    //受信処理
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION:
            std::cout << "Another client has disconnected.\n";
            break;
        case ID_REMOTE_CONNECTION_LOST:
            std::cout << "Another client has lost the connection.\n";
            break;
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            std::cout << "Another client has connected.\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "Our connection request has been accepted.\n";

            {
                // 送信側
                //RakNet::BitStream bsOut;
                // 送信
                //peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
            }


            break;
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "A new client is connecting.\n";
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "The server is full.\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "We have been disconnected.\n";
            break;
        case ID_CONNECTION_LOST:
            std::cout << "Connection lost.\n";
            break;
        case ID_GAME_MESSAGE_1:

        break;
        case ID_CONNECT_MESSAGE_1:
        {
            //登録処理
            m_Client->Regist(packet);
        }

        break;
        case ID_UNCONNECTED_PONG:
        {
            RakNet::SystemAddress serverAddress = packet->systemAddress;
            std::cout << "Server found at: " << serverAddress.ToString() << std::endl;

            // サーバーに接続
            peer->Connect(serverAddress.ToString(false), CRakNet::PORT, 0, 0);
            std::cout << "Connecting to " << serverAddress.ToString() << "...\n";
            break;
        }

        default:
            std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
            break;
        }
    }
}
