//================================
//
// TCP接続のサーバー用のcpp[raknet_server.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "card_client.h"
#include "card_assist.h"
#include "card_attack.h"
#include "card_deffence.h"
#include "card_manager.h"
#include "card_assist_buff.h"
#include "card_assist_debuff.h"

//=====================================
//コンストラクタ
//=====================================
CCard_Client::CCard_Client() :
    m_pPacket(nullptr),	//パケット
    m_pPeer(nullptr)    //ピア
{
   
}

//=====================================
//デストラクタ
//=====================================
CCard_Client::~CCard_Client()
{
    
}

//=====================================
//初期化処理
//=====================================
bool CCard_Client::Init()
{
    if (m_pPeer != nullptr) return true;

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
void CCard_Client::Uninit()
{
    if (m_pPeer == nullptr) return;
    RakNet::RakPeerInterface::DestroyInstance(m_pPeer);
    m_pPeer = nullptr;
}

//=====================================
//通信処理
//=====================================
void CCard_Client::Communication()
{
    //変数宣言
    RakNet::Packet* packet;

    //読み込み処理
    for (packet = m_pPeer->Receive(); packet; m_pPeer->DeallocatePacket(packet), packet = m_pPeer->Receive())
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
            break;
        case ID_NO_FREE_INCOMING_CONNECTIONS:
            std::cout << "サーバーがいっぱいです\n";
            break;
        case ID_DISCONNECTION_NOTIFICATION:
            std::cout << "クライアントが切断されました\n";
            break;
        case ID_CONNECTION_LOST:
            std::cout << "クライアントの接続が失われました\n";
            break;

        case ID_UNCONNECTED_PONG:
        {
            RakNet::SystemAddress serverAddress = packet->systemAddress;
            std::cout << "Server found at: " << serverAddress.ToString() << std::endl;

            // サーバーに接続
            m_pPeer->Connect(serverAddress.ToString(false), PORT, 0, 0);
           // std::cout << "Connecting to " << serverAddress.ToString() << "...\n";
            break;
        }

        case ID_GAME_MESSAGE_1:

            break;
        case ID_CARD_MESSAGE_1:
            //std::cout << "カード情報\n";
            ReceiveCardInfo(packet);
            break;

        case ID_ALLCARD_MESSAGE_1:

            //std::cout << "全カード情報\n";
            ReceiveAllCardInfo(packet);
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
void CCard_Client::RequestCard(int id)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GAME_MESSAGE::ID_CARD_MESSAGE_1);    //メッセージ
    bsOut.Write(id);                                                    //番号

    //サーバーに送信
    Send(m_pPeer, &bsOut);
}

//=====================================
//全カード情報の取得
//=====================================
void CCard_Client::RequestAllCard()
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GAME_MESSAGE::ID_ALLCARD_MESSAGE_1);    //メッセージ

    //サーバーに送信
   // Send(m_pPeer, &bsOut);

    //サーバーに送信
    RakNet::SystemAddress server_address = m_pPeer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        //サーバーにブロードキャスト
        m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_pPeer->GetSystemAddressFromIndex(0), false);
    }
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
    Param CardParam;            //カード情報

    //読み込み
    bsIn.Read(messageId);
    bsIn.Read(CardParam);

    //登録
    My::CCardManager::GetInstance()->RegistCardList(CardParam);

    //下にカード情報を追加
    //RegistCard(CardParam);
}

//=====================================
//カードの登録
//=====================================
void CCard_Client::RegistCard(Param param)
{
    ////ベースのパラメータを読み込み
    //My::CCard::BaseStatus Base;
    //Base.nCardID = param.nCardID;                           //カード番号
    //Base.nPackID = param.nPackID;                           //パック番号
    //Base.imagePath = param.ImagePath;                       //画像パス
    //Base.maintype = (My::CCard::CARDTYPE_)param.Maintype;   //カード種類
    //Base.name = param.Name;                                 //名前
    //Base.nCost = param.nCost;                               //コスト
    //Base.raritytype = (My::CCard::RARITY)param.Raritytype;  //レアリティ
    //Base.ruby = param.Ruby;                                 //フリガナ

    ////カードの種類に応じて生成先を変更
    //switch (param.Maintype)
    //{
    //case ATTACK:    //攻撃
    //{
    //    My::CCardAttack* pAttack = new My::CCardAttack(PRIORITY);

    //    //パラメータの代入
    //    pAttack->SetBaseStatus(Base);         //ベース
    //    pAttack->SetAttackValue(param.nDamage);    //ダメージ
    //    pAttack->SetAttackType((My::CCardAttack::AttackType)param.Attacktype);   //攻撃種類

    //    //登録
    //    My::CCardManager::GetInstance()->RegistCardList(pAttack);

    //}
    //   
    //    break;

    //case DEFENSE:    //防御
    //{
    //    My::CCardDeffence* pDeffence = new My::CCardDeffence(PRIORITY);

    //    //パラメータの代入
    //    pDeffence->SetBaseStatus(Base);                   //ベース
    //    pDeffence->SetCounterValue(param.nCounter);       //カウンター
    //    pDeffence->SetDefenceType((My::CCardDeffence::DefenseType)param.Defensetype);   //守備の種類
    //    pDeffence->SetDefenceValue(param.nGuard);           //ガード値

    //    //登録
    //    My::CCardManager::GetInstance()->RegistCardList(pDeffence);
    //}

    //    break;

    //case ASSIST:    //アシスト
    //{
    //    //My::CCardAssist* pAssist = new My::CCardAssist(PRIORITY);
    //    My::CCardAssist* pAssist = nullptr;
    //    pAssist = LoadAssistCard(param);

    //    //パラメータの代入
    //    pAssist->SetBaseStatus(Base);              //ベース
    //    pAssist->SetAssistType((My::CCardAssist::AssistType)param.Assisttype);  //アシストの種類

    //    //登録
    //    My::CCardManager::GetInstance()->RegistCardList(pAssist);
    //}

    //    break;
    //default:
    //    break;
    //}
}

//=====================================
//アシストカードの読み込み
//=====================================
//My::CCardAssist* CCard_Client::LoadAssistCard(Param param)
//{
//    //switch (param.Assisttype)
//    //{
//    //case CCard_Client::AssistType::BUFF:    //バフ
//    //{
//    //    My::CCardAssist_Buff* pBuff = new My::CCardAssist_Buff(PRIORITY);
//    //    pBuff->SetBuffType((My::CCardAssist_Buff::BuffType)param.Bufftype);
//    //    return pBuff;
//    //}
//    //    break;
//
//    //case CCard_Client::AssistType::DEBUFF:  //デバフ
//    //{
//    //    My::CCardAssist_Debuff* pDebuff = new My::CCardAssist_Debuff(PRIORITY);
//    //    return nullptr;
//    //    //pDebuff->SetDebuffType((My::CCardAssist_Debuff::DebuffType)param.);   //TODOデバフタイプ出来次第追加
//    //}
//    //    break;
//
//    //case CCard_Client::AssistType::OBSTRUCT://妨害TODO:妨害クラスが出来次第追加
//    //    return nullptr;
//    //    break;
//    //}
//
//    //return nullptr;
//}

//=====================================
//全カード情報の受信
//=====================================
void CCard_Client::ReceiveAllCardInfo(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ
    int nMax = 0;               //最大枚数

    //読み込み
    bsIn.Read(messageId);
    bsIn.Read(nMax);

    //サーバーに登録されている枚数分読み込み
    for (int i = 0; i < nMax; i++)
    {
        Param CardParam = {};       //カード情報
        RakNet::RakString msg;      //文字列読み込み用

       // bsIn.Read(CardParam);
        bsIn.Read(CardParam.nPackID);
        bsIn.Read(CardParam.nCardID);
        bsIn.Read(msg);
        CardParam.Name = msg.C_String(); msg.Clear();
        bsIn.Read(msg);
        CardParam.Ruby = msg.C_String(); msg.Clear();
        bsIn.Read(CardParam.nCost);
        bsIn.Read(CardParam.nDamage);
        bsIn.Read(CardParam.nGuard);
        bsIn.Read(CardParam.nCounter);
        bsIn.Read(CardParam.nHeal);
        bsIn.Read(CardParam.Maintype);
        bsIn.Read(CardParam.Raritytype);
        bsIn.Read(CardParam.Assisttype);
        bsIn.Read(CardParam.Attacktype);
        bsIn.Read(CardParam.Defensetype);
        bsIn.Read(CardParam.Bufftype);
        bsIn.Read(CardParam.Healtype);
        bsIn.Read(msg);
        CardParam.ImagePath = msg.C_String();

        //登録
        My::CCardManager::GetInstance()->RegistCardList(CardParam);

        //下にカード情報を追加
        //RegistCard(CardParam);
    }

}