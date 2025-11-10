//================================
//
// TCP接続のサーバー用のcpp[raknet_server.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "card_client.h"
#include "card.h"

//静的変数の宣言
CRakNet_Data* CCard_Client::m_pRakNetData = nullptr;

//=====================================
//コンストラクタ
//=====================================
CCard_Client::CCard_Client() :
    m_pPacket(nullptr),	//パケット
    m_pPeer(nullptr)    //ピア
{
    //動的確保
    //m_pRakNetData = new CLobby_Data;
}

//=====================================
//デストラクタ
//=====================================
CCard_Client::~CCard_Client()
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
bool CCard_Client::Init(int nPortNum, RakNet::RakPeerInterface* peer)
{
    m_pPeer = RakNet::RakPeerInterface::GetInstance();

    //ピアの取得
    RakNet::SocketDescriptor sd;
    m_pPeer->Startup(1, &sd, 1);

    // ブロードキャストでサーバーを探す
    m_pPeer->Ping("255.255.255.255", PORT, false);

    return true;
}

//=====================================
//終了処理
//=====================================
void CCard_Client::Uninit(RakNet::RakPeerInterface* peer)
{
    RakNet::RakPeerInterface::DestroyInstance(peer);
}

//=====================================
//通信処理
//=====================================
void CCard_Client::Communication(RakNet::RakPeerInterface* peer)
{
    //変数宣言
    RakNet::Packet* packet;

    //読み込み処理
    for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
    {
        switch (packet->data[0])
        {
        case ID_REMOTE_DISCONNECTION_NOTIFICATION:
            std::cout << "別のクライアントが切断されました\n";
            break;
        case ID_REMOTE_CONNECTION_LOST:
            std::cout << "別のクライアントが接続を失いました.\n";
            break;
        case ID_REMOTE_NEW_INCOMING_CONNECTION:
            std::cout << "別のクライアントが接続しました\n";
            break;
        case ID_CONNECTION_REQUEST_ACCEPTED:
            std::cout << "接続リクエストが承認されました\n";
            break;
        case ID_NEW_INCOMING_CONNECTION:
            std::cout << "新しいクライアントが接続しました\n";
            m_pRakNetData->NewConnection(packet, peer);
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "サーバーがいっぱいです\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "クライアントが切断されました\n";
            m_pRakNetData->DisConnection(packet, peer);
            break;
        case ID_CONNECTION_LOST:
            std::cout << "クライアントの接続が失われました\n";
            m_pRakNetData->DisConnection(packet, peer);
            break;
        case ID_GAME_MESSAGE_1:

            break;
        case ID_CARD_MESSAGE_1:
            std::cout << "カード情報\n";
            break;

        case ID_ALLCARD_MESSAGE_1:

            std::cout << "全カード情報\n";
            break;

        default:
            std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
            break;
        }
    }
}

//=====================================
//送信処理
//=====================================
void CCard_Client::Send(RakNet::RakPeerInterface* peer, RakNet::BitStream* out)
{
    //サーバーに送信
    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        //サーバーにブロードキャスト
        peer->Send(out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//特定カード情報のリクエスト
//=====================================
void CCard_Client::RequestCard(RakNet::RakPeerInterface* peer, int id)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GAME_MESSAGE::ID_CARD_MESSAGE_1);    //メッセージ
    bsOut.Write(id);                                                    //番号
   
    //サーバーに送信
    Send(peer, &bsOut);
}

//=====================================
//全カード情報のリクエスト
//=====================================
void CCard_Client::RequestAllCard(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GAME_MESSAGE::ID_ALLCARD_MESSAGE_1);    //メッセージ

    //サーバーに送信
    Send(peer, &bsOut);
}

//=====================================
//カード情報の受信
//=====================================
void CCard_Client::ReceiveCardInfo(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ
    CCard Card;                 //カード情報
   
    //読み込み
    bsIn.Read(messageId);
    bsIn.Read(Card);

    //下にカード情報を追加
}