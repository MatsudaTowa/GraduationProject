//================================
//
// TCP接続のサーバー用のcpp[raknet_server.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet_server.h"

//静的変数の宣言
CRakNet_Data* CRakNet_Server::m_pRakNetData = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet_Server::CRakNet_Server() :
	m_pPacket(nullptr),	//パケット
    m_pPeer(nullptr)    //ピア
    //m_CardVector()      //カードのベクター
{
    //動的確保
    //m_pRakNetData = new CLobby_Data;

    m_pRakNetData = new CRakNet_Data;
}

//=====================================
//デストラクタ
//=====================================
CRakNet_Server::~CRakNet_Server()
{
    //メモリ開放
    if (m_pRakNetData != nullptr)
    {
        delete m_pRakNetData;
        m_pRakNetData = nullptr;
    }
}

//=====================================
//初期化処理
//=====================================
bool CRakNet_Server::Init(int nPortNum)
{
    m_pPeer = RakNet::RakPeerInterface::GetInstance();

	//ピアの取得
	RakNet::SocketDescriptor sd(nPortNum, 0);
    m_pPeer->Startup(MAX_CLIENTS, &sd, 1);
	std::cout << "Starting the server.\n";
    m_pPeer->SetMaximumIncomingConnections(MAX_CLIENTS);

    //LAN検出用にアドバタイズ（ブロードキャスト）
    m_pPeer->AdvertiseSystem("255.255.255.255", nPortNum, 0, 0, 0);

    //切断を確認する時間を5秒に変更
    m_pPeer->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	return true;
}

//=====================================
//接続受付の処理
//=====================================
void CRakNet_Server::Accept(void)
{

}

//=====================================
//終了処理
//=====================================
void CRakNet_Server::Uninit()
{
    RakNet::RakPeerInterface::DestroyInstance(m_pPeer);
}

//=====================================
//通信処理
//=====================================
void CRakNet_Server::Communication()
{
    // コンソールの出力コードページを UTF-8 に変更
    SetConsoleOutputCP(CP_UTF8);

    //変数宣言
    RakNet::Packet* packet;

    //常時周回
    while (1) 
    {
        //読み込み処理
        for (packet = m_pPeer->Receive(); packet; m_pPeer->DeallocatePacket(packet), packet = m_pPeer->Receive())
        {
            switch (packet->data[0]) 
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                std::cout << u8"別のクライアントが切断されました\n";
                break;
            case ID_REMOTE_CONNECTION_LOST:
                std::cout << u8"別のクライアントが接続を失いました.\n";
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION:
                std::cout << u8"別のクライアントが接続しました\n";
                break;
            case ID_CONNECTION_REQUEST_ACCEPTED:
                std::cout << u8"接続リクエストが承認されました\n";
                break;
            case ID_NEW_INCOMING_CONNECTION:
                std::cout << u8"新しいクライアントが接続しました\n";
                m_pRakNetData->NewConnection(packet, m_pPeer);
                break;
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                std::cout << u8"サーバーがいっぱいです\n";
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                std::cout << u8"クライアントが切断されました\n";
                m_pRakNetData->DisConnection(packet, m_pPeer);
                break;
            case ID_CONNECTION_LOST:
                std::cout << u8"クライアントの接続が失われました\n";
                m_pRakNetData->DisConnection(packet, m_pPeer);
                break;
            case CRakNet_Data::ID_CARD_MESSAGE_1:
                std::cout << u8"指定番号のカードの情報";
                m_pRakNetData->SendCard(packet, m_pPeer);
                break;
                case CRakNet_Data::ID_ALLCARD_MESSAGE_1:
                std::cout << u8"全カードの情報";
                m_pRakNetData->SendAllCard(packet, m_pPeer);
                break;
           
            default:
                std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
                break;
            }
        }
    }
}

//=====================================
//特定のクライアントとの通信処理
//=====================================
void CRakNet_Server::Communication0(RakNet::RakPeerInterface* peer, RakNet::Packet* packet)
{

}

//=====================================
//データの変更
//=====================================
void CRakNet_Server::ChangeData(CRakNet_Data* data)
{
  
}

//=====================================
//カードの読み込み
//=====================================
void CRakNet_Server::LoadCard()
{
    //カードの読み込み
    m_pRakNetData->LoadCard();
}
