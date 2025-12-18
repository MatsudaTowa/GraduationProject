//================================
//
// RakNet接続のサーバー用のcpp[raknet.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet.h"
#include "client_lobby.h"
#include "client_duel.h"
#include "active_scene_manager.h"

//静的変数宣言
CClient* CRakNet::m_Client = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet::CRakNet() :
    m_pPacket(nullptr),	 //パケット
    m_pPeer(nullptr),    //ピア
    m_isOnline(false),   //オンラインか
    m_isUpdate(false)    //更新しても良いか
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

        case ID_LOBY_MESSAGE_ADD_ENEMY:

            //敵の追加処理
            m_Client->ReceiveAddEnemy(packet);
            break;

        case ID_LOBY_MESSAGE_SEND_READY:
        
            //準備OKの合図を送る処理
            m_Client->SendReady(packet, peer);
            break;

        case ID_LOBY_MESSAGE_RECEIVE_READY:
        
            //準備OKの合図を受信する処理処理
            m_Client->ReceiveReady(packet);
            break;

        case ID_LOBY_MESSAGE_REQUEST_DECK:  //デッキの要求
            SendDeck();
            break;

        case ID_LOBY_MESSAGE_RECEIVE_START:
            ChangeClient(new CClient_Duel); //クライアント処理を変更
            m_Client->AddCPU(packet, peer); //コンピューターの追加
            break;

        case ID_DUEL_MESSAGE_CLIENT_START:
            //ChangeClient(new CClient_Duel);
            break;

        case ID_DUEL_MESSAGE_DECK:
            m_Client->ReceiveDeck(packet);
            break;

        case ID_DUEL_MESSAGE_START:
            m_Client->StartBattle(packet);
            break;

       /* case ID_DUEL_MESSAGE_SEND_STATUS:
            m_Client->ReceiveStatus(packet);
            break;*/

        case ID_DUEL_MESSAGE_DRAW:
            m_Client->ReceiveDrawCard(packet);
            //m_isUpdate = true;
            break;

        case ID_DUEL_MESSAGE_STATUS:
            m_Client->ReceiveStatus(packet);
            m_isUpdate = true;
            break;

        case ID_DUEL_MESSAGE_CAST_CARD:
            m_Client->CardCast(packet);
            break;

        case ID_DUEL_MESSAGE_TRIGGER:
            m_Client->ReceiveTriggerCard(packet);
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
//クライアント処理の変更
//=====================================
void CRakNet::ChangeClient(CClient* client)
{
    //引数にデータがあるか確認
    if (client == nullptr) return;

    //変数
    std::list<CClient::PlayerParam> Param = {};    //基底のパラメータリスト
    Param.clear();

    //クライアントの中身を確認
    if (m_Client != nullptr)
    {
        Param = m_Client->GetParam();
        //クライアントの削除
        delete m_Client;
        m_Client = nullptr;
    }

    //代入
    m_Client = client;

    //基底パラメータの引き継ぎ
    m_Client->SetParam(Param);
}

//=====================================
//開始の合図を送信
//=====================================
void CRakNet::SendStartSign()
{
    //開始の合図を送る処理
    m_Client->SendStartSign(m_pPeer);
}

//=====================================
//デッキの内容を送信
//=====================================
void CRakNet::SendDeck()
{
    //データの作成
    RakNet::BitStream bsOut;    //送信用変数
    int nDeckNum = My::CActiveSceneManager::GetInstance()->GetPlayer()->GetDeck().size();           //デッキ枚数

    //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_DECK);
    bsOut.Write(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx());
    bsOut.Write(nDeckNum);

    //プレイヤーのデッキを書き出し
    for (int id : My::CActiveSceneManager::GetInstance()->GetPlayer()->GetDeck())
    {
        bsOut.Write(id);    //カードのiDを書き出す
    }

    //アドレスを取得
    RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        // 全クライアントにブロードキャスト
        m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//ステータスの送信
//=====================================
void CRakNet::SendStatus()
{
    //m_Client->SendStatus(m_pPeer);
}

//=====================================
//敵の追加を送信
//=====================================
void CRakNet::SendAddEnemy()
{
    m_Client->SendAddEnemy(m_pPeer);
}

//=====================================
//自身のステータスを送る
//=====================================
void CRakNet::SendMyStatus()
{
    //m_Client->SendMyStatus(m_pPeer);
    m_isUpdate = false;
}

//=====================================
//カードドローのリクエスト処理
//=====================================
void CRakNet::RequestDrawCard()
{
    //データの作成
    RakNet::BitStream bsOut;    //送信用変数
    int nUserId = My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx();  //使用者番号
 
    //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_DRAW);    //メッセージ
    bsOut.Write(nUserId);                                                           //使用者番号
   
    //アドレスを取得
    RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        // 全クライアントにブロードキャスト
        m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//カードキャストのリクエスト処理
//=====================================
void CRakNet::RequestCastCard(int cardid, int sameid, std::vector<int> targetplayer)
{
    //データの作成
    RakNet::BitStream bsOut;    //送信用変数
    int nUserId = My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx();  //使用者番号

   //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_CAST_CARD);   //メッセージ
    bsOut.Write(nUserId);                                                               //使用者番号
    bsOut.Write(cardid);                                                                //カード番号
    bsOut.Write(sameid);                                                                //同種の何番目のカードか
    bsOut.Write(targetplayer.size());                                                   //対象者の数

    //対象者の番号を書き出し
    for (int iter : targetplayer)
    {
        bsOut.Write(iter);
    }

    //アドレスを取得
    RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        // 全クライアントにブロードキャスト
        m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
    }
}
