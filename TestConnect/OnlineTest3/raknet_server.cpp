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
#include "RakNetTypes.h"
#include "GetTime.h"
#include "duel_player_manager.h"
#include "duel_player.h"

//静的変数の宣言
CRakNet_Data* CRakNet_Server::m_pRakNetData = nullptr;

//=====================================
//コンストラクタ
//=====================================
CRakNet_Server::CRakNet_Server() :
	m_pPacket(nullptr),	                    //パケット
    m_isUpdate(false),                      //更新
    m_SceneState(CRakNet_Data::SCENE_LOBBY),//シーンの状態
    m_deltaTime(0),                         //デルタタイム
    m_pPeer(nullptr)                        //ピア
{
    //動的確保
    m_pRakNetData = new CLobby_Data;

    RakNet::Time now = RakNet::GetTime();
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

    //ピアの代入
    m_pPeer = peer;

    //カード初期化
    CCard_Client::GetInstance()->Init();
    //CCard_Client::GetInstance()->RequestAllCard();

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
    CCard_Client::GetInstance()->Uninit();
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
        //カードサーバーとの受信
        CCard_Client::GetInstance()->Communication();

        //読み込み処理
        for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
        {
            //受信したメッセージ
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

                //プレイヤーが存在しないならリセット
                if (!m_pRakNetData->IsPlayerExist())
                {
                    //データの変更
                    ChangeData(new CLobby_Data);
                }

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
            case CRakNet_Data::GameMessages::ID_LOBY_MESSAGE_ADD_ENEMY:
                std::cout << "敵の追加\n";
                m_pRakNetData->AddCPU(packet, peer);
                break;
            case CRakNet_Data::GameMessages::ID_LOBY_MESSAGE_SEND_READY:
                std::cout << "準備フラグ\n";
                m_pRakNetData->Ready(packet, peer);
                break;

            case CRakNet_Data::GameMessages::ID_LOBY_MESSAGE_SEND_START:

                std::cout << "開始の合図を受信\n";
                //ロビーから戦闘に切り替わるフラグを受信したら
                if (m_pRakNetData->ChangeToDuel(packet, peer))
                {
                    //デッキのリクエスト
                    m_pRakNetData->SendRequestDeck(peer);

                    //データの変更
                    //ChangeData(new CDuel_Data);

                    //開始メンバーの送信処理
                    //m_pRakNetData->SendStartMember(peer);
                }
                
                break;

            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_CLIENT_START:

                //バトルを開始するか確認
                if (m_pRakNetData->CheckStartBattle(packet))
                {
                    m_pRakNetData->StartBattle(peer);
                }
                
                break;

            /*case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_SEND_STATUS:

                m_pRakNetData->SendStatus(packet, peer);
                break;*/

                //デッキのメッセージ
            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_DECK:

                //全員分取得したか
                if (m_pRakNetData->ReceiveDeck(packet))
                {
                    //データの変更
                    ChangeData(new CDuel_Data);

                    //開始メンバーの送信処理
                    m_pRakNetData->SendStartMember(peer);
                }
               
                break;

            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_DRAW:

                m_pRakNetData->ReceiveDrawCard(packet, peer);
                break;

            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_STATUS:

                m_pRakNetData->ReceiveStatus(packet, peer);
                break;

            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_CAST_CARD:

                m_pRakNetData->ReceiveCastCard(packet, peer);
                break;

            case CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_CAST_DEFCARD:

                m_pRakNetData->ReceiveCastDefCard(packet);
                break;

            default:
                std::cout << "Message with identifier " << (int)packet->data[0] << " has arrived.\n";
                break;
            }
        }

        //シーンの更新
        m_pRakNetData->UpdateScene(packet, peer);
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

    //変数の上書き
    std::list<CPlayer::ChangeData> DataList;
    DataList.clear();

    //メンバーの追加(CPU)
    m_pRakNetData->AddStartMember();

    //データの中身を確認
    if (m_pRakNetData != nullptr)
    {
        //基底パラメータの保存
        DataList = m_pRakNetData->GetData();

        //削除
        delete m_pRakNetData;
        m_pRakNetData = nullptr;
    }

    //上書き
    m_pRakNetData = data;               //クラスの変更
    m_pRakNetData->SetData(DataList);   //データの引き継ぎ
}

//=====================================
//ステータスの送信
//=====================================
void CRakNet_Server::SendStatus()
{
    // データの作成
   RakNet::BitStream bsOut;
  
   //書き出し
   bsOut.Write((RakNet::MessageID)CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_STATUS);

   //書き出し
   for (auto iter : My::CDuel_Player_Manager::GetInstance()->GetList())
   {
       //送信用のデータをまとめる
       My::CDuel_Player::DuelData SendData;
       SendData.BaceData.RakNetID = iter->GetRakNetID();   //識別番号
       SendData.BaceData.nIndex = iter->GetIndex();        //番号
       SendData.BaceData.Tag = iter->GetTag();             //タグ
       SendData.Status = iter->GetStatus();                //ステータス
       bsOut.Write(SendData);
   }

   // 全クライアントにブロードキャスト
   m_pPeer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //優先的に送ることで反映を先にする
}

//=====================================
//キャストカードの送信
//=====================================
void CRakNet_Server::SendCastCard(My::CCard* Card)
{
    // データの作成
    RakNet::BitStream bsOut;
    int PlayerNum = My::CDuel_Player_Manager::GetInstance()->GetList().size();

    //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_CAST_CARD);

    //書き出し
    for (auto iter : My::CDuel_Player_Manager::GetInstance()->GetList())
    {
        //送信用のデータをまとめる
        int nCardID = 0;                //カード番号
        int nPlayerID = 0;              //プレイヤー番号
        int nTargetNum = 0;             //ターゲット数
        std::vector<int> TargetVector;  //ターゲットベクター

        //TODO : この下にカードの書き出し
        bsOut.Write(Card->GetParam().nCardID);
    }

    // 全クライアントにブロードキャスト
    m_pPeer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //優先的に送ることで反映を先にする
}

//=====================================
//トリガーカードの送信
//=====================================
void CRakNet_Server::SendTriggerCard(My::CCard* Card)
{
    // データの作成
    RakNet::BitStream bsOut;

    //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_TRIGGER);    //メッセージ
    bsOut.Write(Card->GetUserId());                                                         //使用者番号
    bsOut.Write(Card->GetBaseStatus().nCardID);                                             //カード番号
    bsOut.Write(Card->GetSameTypeId());                                                     //同種類の番号
    bsOut.Write(Card->GetBaseStatus().Maintype);                                            //カードの種類

    Card->SendTriggerData(&bsOut);  //カードごとの送るデータ

    //各プレイヤーのパラメータを送信
    for (auto iter : My::CDuel_Player_Manager::GetInstance()->GetList())
    {
        bsOut.Write(iter->GetStatus());
    }

    //全クライアントにブロードキャスト
    m_pPeer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //優先的に送ることで反映を先にする
}

//=====================================
//ゲーム終了の送信
//=====================================
void CRakNet_Server::SendGameSet()
{
    if (!IsSendGameSet()) return;

    // データの作成
    RakNet::BitStream bsOut;

    //書き出し
    bsOut.Write((RakNet::MessageID)CRakNet_Data::GameMessages::ID_DUEL_MESSAGE_GAMESET);    //メッセージ
   
    //全クライアントにブロードキャスト
    m_pPeer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //優先的に送ることで反映を先にする
}

//=====================================
//ゲーム終了の送信をするか
//=====================================
bool CRakNet_Server::IsSendGameSet()
{
    //生存者数
    int nLifePlayer = 0;

    //各プレイヤーのパラメータを送信
    for (auto iter : My::CDuel_Player_Manager::GetInstance()->GetList())
    {
        //体力が1以上なら生存者のカウント
        if (iter->GetStatus().life > 0) nLifePlayer++;
    }

    //生存者数が1人なら終了
    if (nLifePlayer <= 1)
    {
        return true;
    }

    return false;
}