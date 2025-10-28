//================================
//
// 送受信データクラス用のヘッダー[lobby_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "lobby_data.h"
#include "main.h"

//=====================================
//新しいクライアントの接続処理
//=====================================
void CLobby_Data::NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //新しく情報を取得
    CLobby_Player* pPlayer = new CLobby_Player;

    //パラメータの設定
    pPlayer->SetIndex(m_LobbyPlayerList.size()); //番号
    pPlayer->SetRakNetID(packet->guid);          //RakNetID
    std::cout << "プレイヤー" << pPlayer->GetIndex() + 1 << "を受信しました\n";

    //プレイヤーの情報を受け取る
    {
        // データの作成
        RakNet::BitStream bsOut;
        int PlayerNum = pPlayer->GetIndex() + 1;
        bsOut.Write((RakNet::MessageID)ID_CONNECT_MESSAGE_1);
        bsOut.Write(PlayerNum);

        //既存プレイヤー情報を書き出し
        for (auto iter : m_LobbyPlayerList)
        {
            //送信用のデータをまとめる
            CLobby_Player::LobbyData SendData;
            SendData.BaceData.RakNetID = iter->GetRakNetID();
            SendData.BaceData.nIndex = iter->GetIndex();
            SendData.isReady = iter->Getready();

            //送信
            bsOut.Write(SendData);
        }

        //新規プレイヤーの書き出し
        {
            //送信用のデータをまとめる
            CLobby_Player::LobbyData SendData;
            SendData.BaceData.RakNetID = pPlayer->GetRakNetID();
            SendData.BaceData.nIndex = pPlayer->GetIndex();
            SendData.isReady = pPlayer->Getready();

            //送信
            bsOut.Write(SendData);
        }

        // 全クライアントにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
    }

    //TODO:push_backのコードを追加予定
    m_LobbyPlayerList.push_back(pPlayer);

    //return Param;
}

//=====================================
//クライアントの切断処理
//=====================================
void CLobby_Data::DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //保存しているものから探す
    for (auto& iter : m_LobbyPlayerList)
    {
        //異なるIDなら飛ばす
        if (iter->GetRakNetID() != packet->guid) continue;

        //削除
        std::cout << "プレイヤー" << iter->GetIndex() + 1 << "をリストから削除しました\n";

        //破棄の処理
        if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }
        
        m_LobbyPlayerList.remove(iter);
        //上記のこれ破棄してなくない？

        break;
    }

    //現在の人数をクライアントに送信
    SendPlayerNum(peer, ID_DISCONNECT_MESSAGE_1);

   //===========TODO以下の処理はサーバーとクライアントでそれぞれ行っている==============//
   //クライアント : 消えたプレイヤーを確認し、番号を数えなおす
   //サーバー : 現在のプレイヤーの情報を送り、その後サーバー側のデータのみ番号変更
   //別々で行っているので統一できていない
   //※修正必須

   //番号の間を埋める
   int i = 0;

   //番号の補正
   for (auto& iter : m_LobbyPlayerList)
   {
       iter->SetIndex(i);
       i++;
   }
}

//=====================================
//プレイヤー数を送信する関数
//=====================================
void CLobby_Data::SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message)
{
    // データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)message);
    bsOut.Write(m_LobbyPlayerList.size());

    //既存プレイヤー情報を書き出し
    for (auto iter : m_LobbyPlayerList)
    {
        //送信用のデータをまとめる
        CLobby_Player::LobbyData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();
        SendData.BaceData.nIndex = iter->GetIndex();
        SendData.isReady = iter->Getready();

        //書き出し
        bsOut.Write(SendData);
    }

    // 全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//準備確認の関数
//=====================================
void CLobby_Data::Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //データの受信
    //RakNet::BitStream bsIn;
    RakNet::BitStream bsIn(packet->data, packet->length, false);
    //bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
    unsigned char messageId;    //受信したメッセージの種類
    int nId = -1;               //番号
    bool isReady = false;       //準備のフラグ

    //読み取り
    //bsIn.Read(messageId);
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
    bsIn.Read(nId);
    bsIn.Read(isReady);

    // データの作成
    RakNet::BitStream bsOut;
    int PlayerNum = m_LobbyPlayerList.size();

    //書き出し
    bsOut.Write((RakNet::MessageID)ID_LOBY_MESSAGE_RECEIVE_READY);
    bsOut.Write(PlayerNum);

    //受信したデータを反映
    for (auto iter : m_LobbyPlayerList)
    {
        //番号で確認
        if (iter->GetIndex() == nId)
        {
            iter->SetReady(isReady);    //上書き
            std::cout << std::boolalpha << isReady << "\n"; // true
            break;
        }
    }

    //書き出し
    for (auto iter : m_LobbyPlayerList)
    {
        //送信用のデータをまとめる
        CLobby_Player::LobbyData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();
        SendData.BaceData.nIndex = iter->GetIndex();
        SendData.isReady = iter->Getready();
        bsOut.Write(iter->Getready());
    }

    // 全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//対戦に変更
//=====================================
bool CLobby_Data::ChangeToDuel(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //リストの周回
    for (auto& iter : m_LobbyPlayerList)
    {
        //準備ができていないプレイヤーがいたらfalse
        if (!iter->Getready())
        {
            return false;
        }
    }

    //ロビー中尚且つ全員が準備中ならtrueを返す
    return true;
}

//=====================================
//プレイヤーリストの設定
//=====================================
void CLobby_Data::SetData(std::list<CPlayer::Data> data)
{
    //リストを削除
    for (auto& iter : m_LobbyPlayerList)
    {
        //破棄の処理
        if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }

        m_LobbyPlayerList.remove(iter);
    }

    //リストの削除
    m_LobbyPlayerList.clear();

    //引数のリスト周回
    for (const auto& iter : data)
    {
        //基底パラメータを代入
        CLobby_Player* pPlayer = new CLobby_Player; //クラスを作成し代入
        pPlayer->SetIndex(iter.nIndex);
        pPlayer->SetRakNetID(iter.RakNetID);

        //追加
        m_LobbyPlayerList.push_back(pPlayer);
    }
}

//=====================================
//プレイヤーリストの取得
//=====================================
std::list<CPlayer::Data> CLobby_Data::GetData()
{
    //変数宣言
    std::list<CPlayer::Data> List;
    List.clear();

    //現在のリストを基底にコピー
    for (const auto& iter : m_LobbyPlayerList)
    {
        //基底構造体のリストに追加
        CPlayer::Data Data;                     //変数
        Data.nIndex = iter->GetIndex();         //番号
        Data.RakNetID = iter->GetRakNetID();    //RakNetID
        List.push_back(Data);                   //追加
    }

    return List;
}

//=====================================
//開始メンバーを送信
//=====================================
void CLobby_Data::AddStartMember()
{
    //足りない数だけプレイヤーを送信
    for (int i = m_LobbyPlayerList.size(); i < 4; i++)
    {
        //プレイヤーを追加
        CLobby_Player* pPlayer = new CLobby_Player;

        //パラメータの設定
        pPlayer->SetIndex(i);                                       //番号
        pPlayer->SetRakNetID(static_cast<RakNet::RakNetGUID>(-1));  //RakNetID(CPUとわかるように-1を代入)
        pPlayer->SetReady(true);                                    //敵は準備がいらないのでtrue

        //追加
        m_LobbyPlayerList.push_back(pPlayer);                       
    }
}
