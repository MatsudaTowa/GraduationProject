//================================
//
// 送受信データクラス用のヘッダー[duel_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "duel_data.h"
#include "main.h"

//=====================================
//新しいクライアントの接続処理
//=====================================
void CDuel_Data::NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    ////新しく情報を取得
    //CDuel_Player* pPlayer = new CDuel_Player;

    ////パラメータの設定
    //pPlayer->SetIndex(m_DuelPlayerList.size()); //番号
    //pPlayer->SetRakNetID(packet->guid);          //RakNetID
    //std::cout << "プレイヤー" << pPlayer->GetIndex() + 1 << "を受信しました\n";

    ////プレイヤーの情報を受け取る
    //{
    //    // データの作成
    //    RakNet::BitStream bsOut;
    //    int PlayerNum = pPlayer->GetIndex() + 1;
    //    bsOut.Write((RakNet::MessageID)ID_CONNECT_MESSAGE_1);
    //    bsOut.Write(PlayerNum);

    //    //既存プレイヤー情報を書き出し
    //    for (auto iter : m_DuelPlayerList)
    //    {
    //        //送信用のデータをまとめる
    //        CDuel_Player::DuelData SendData;
    //        SendData.BaceData.RakNetID = iter->GetRakNetID();
    //        SendData.BaceData.nIndex = iter->GetIndex();

    //        //送信
    //        bsOut.Write(SendData);
    //    }

    //    //新規プレイヤーの書き出し
    //    {
    //        //送信用のデータをまとめる
    //        CDuel_Player::DuelData SendData;
    //        SendData.BaceData.RakNetID = pPlayer->GetRakNetID();
    //        SendData.BaceData.nIndex = pPlayer->GetIndex();

    //        //送信
    //        bsOut.Write(SendData);
    //    }

    //    // 全クライアントにブロードキャスト
    //    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
    //}

    ////TODO:push_backのコードを追加予定
    //m_DuelPlayerList.push_back(pPlayer);





    //ログ
    std::cout << "対戦中にプレイヤーを受信しました\n";

    //クライアントのSystemAddressを取得
    RakNet::SystemAddress ClientAddress = packet->systemAddress;

    //クライアントに通知を送信して切断
    peer->CloseConnection(ClientAddress, true);

    std::cout << "対戦中に入ってくるな\n";

    //return Param;
}

//=====================================
//クライアントの切断処理
//=====================================
void CDuel_Data::DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //保存しているものから探す
    for (auto& iter : m_DuelPlayerList)
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

        m_DuelPlayerList.remove(iter);
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
    for (auto& iter : m_DuelPlayerList)
    {
        iter->SetIndex(i);
        i++;
    }
}

//=====================================
//プレイヤー数を送信する関数
//=====================================
void CDuel_Data::SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message)
{
    // データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)message);
    bsOut.Write(m_DuelPlayerList.size());

    //既存プレイヤー情報を書き出し
    for (auto iter : m_DuelPlayerList)
    {
        //送信用のデータをまとめる
        CDuel_Player::DuelData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();
        SendData.BaceData.nIndex = iter->GetIndex();

        //書き出し
        bsOut.Write(SendData);
    }

    // 全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//準備確認の関数
//=====================================
void CDuel_Data::Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
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
    int PlayerNum = m_DuelPlayerList.size();

    //書き出し
    bsOut.Write((RakNet::MessageID)ID_LOBY_MESSAGE_RECEIVE_READY);
    bsOut.Write(PlayerNum);

    //受信したデータを反映
    for (auto iter : m_DuelPlayerList)
    {
        //番号で確認
        if (iter->GetIndex() == nId)
        {
            break;
        }
    }

    //書き出し
    for (auto iter : m_DuelPlayerList)
    {
        //送信用のデータをまとめる
        CDuel_Player::DuelData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();
        SendData.BaceData.nIndex = iter->GetIndex();
        //bsOut.Write(iter->Getready());
    }

    // 全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//サーバーが変わったことを通知
//=====================================
void CDuel_Data::SendChangedServer(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    // データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_SERVER_START);

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}
