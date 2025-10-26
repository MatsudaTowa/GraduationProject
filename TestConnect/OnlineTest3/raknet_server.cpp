//================================
//
// TCP接続のサーバー用のcpp[raknet_server.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet_server.h"
#include "lobby_data.h"
#include "duel_data.h"

//静的変数の宣言
CRakNet_Data* CRakNet_Server::m_pRakNetData = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet_Server::CRakNet_Server() :
	m_pPacket(nullptr)	//パケット
{
    //動的確保
    m_pRakNetData = new CLobby_Data;
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
bool CRakNet_Server::Init(int nPortNum, RakNet::RakPeerInterface* peer)
{
	//ピアの取得
	RakNet::SocketDescriptor sd(nPortNum, 0);
    peer->Startup(MAX_CLIENTS, &sd, 1);
	std::cout << "Starting the server.\n";
    peer->SetMaximumIncomingConnections(MAX_CLIENTS);

    //LAN検出用にアドバタイズ（ブロードキャスト）
    peer->AdvertiseSystem("255.255.255.255", nPortNum, 0, 0, 0);

    //切断を確認する時間を5秒に変更
    peer->SetTimeoutTime(5000, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

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
void CRakNet_Server::Uninit(RakNet::RakPeerInterface* peer)
{
    RakNet::RakPeerInterface::DestroyInstance(peer);
}

//=====================================
//通信処理
//=====================================
void CRakNet_Server::Communication(RakNet::RakPeerInterface* peer)
{
    //変数宣言
    RakNet::Packet* packet;

    //常時周回
    while (1) 
    {
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
            case CRakNet_Data::GameMessages::ID_GAME_MESSAGE_1:

            {
                // 受信側
                RakNet::BitStream bsIn(packet->data, packet->length, false);
                unsigned char messageId;
                float positionX, positionY;

                // BitStreamから構造体の各メンバーを読み出す
                bsIn.Read(messageId);
               // bsIn.Read(positionX);
                //bsIn.Read(positionY);

                std::cout << "データを受信した\n";

                /* RakNet::RakString rs;
                 RakNet::BitStream bsIn(packet->data, packet->length, false);
                 bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
                 bsIn.Read(rs);
                 std::cout << "Received from a client: " << rs.C_String() << "\n";*/
            }
            break;
            case CRakNet_Data::GameMessages::ID_LOBY_MESSAGE_SEND_READY:
                std::cout << "準備フラグ\n";
                m_pRakNetData->Ready(packet, peer);
                break;

            case CRakNet_Data::GameMessages::ID_LOBY_MESSAGE_RECEIVE_START:

                //ロビーから戦闘に切り替わるフラグを受信したら
                if (m_pRakNetData->ChangeToDuel(packet, peer))
                {
                    //データの変更
                    ChangeData(new CDuel_Data);
                }
                
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
    //上書き先のデータがあるか確認
    if (data == nullptr) return;

    //データの中身を確認
    if (m_pRakNetData != nullptr)
    {
        delete m_pRakNetData;
        m_pRakNetData = nullptr;
    }

    //上書き
    m_pRakNetData = data;
}
