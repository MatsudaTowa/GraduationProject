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
        SendData.BaceData.Tag = iter->GetTag();

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
        SendData.BaceData.Tag = iter->GetTag();
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
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_CLIENT_START);

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//プレイヤーリストの設定
//=====================================
void CDuel_Data::SetData(std::list<CPlayer::Data> data)
{
    //リストを削除
    for (auto& iter : m_DuelPlayerList)
    {
        //破棄の処理
        if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }

        m_DuelPlayerList.remove(iter);
    }

    //リストの削除
    m_DuelPlayerList.clear();

    //引数のリスト周回
    for (const auto& iter : data)
    {
        //基底パラメータを代入
        CDuel_Player* pPlayer = new CDuel_Player; //クラスを作成し代入
        pPlayer->SetIndex(iter.nIndex);
        pPlayer->SetRakNetID(iter.RakNetID);
        pPlayer->SetTag(iter.Tag);

        //追加
        m_DuelPlayerList.push_back(pPlayer);
    }
}

//=====================================
//プレイヤーリストの取得
//=====================================
std::list<CPlayer::Data> CDuel_Data::GetData()
{
    //変数宣言
    std::list<CPlayer::Data> List;
    List.clear();

    //現在のリストを基底にコピー
    for (const auto& iter : m_DuelPlayerList)
    {
        //基底構造体のリストに追加
        CPlayer::Data Data;                     //変数
        Data.nIndex = iter->GetIndex();         //番号
        Data.RakNetID = iter->GetRakNetID();    //RakNetID
        Data.Tag = iter->GetTag();              //タグ
        List.push_back(Data);                   //追加
    }

    return List;
}

//=====================================
//開始メンバーの送信
//=====================================
void CDuel_Data::SendStartMember(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_LOBY_MESSAGE_RECEIVE_START);

    //対戦相手の情報を共有
    for (const auto& iter : m_DuelPlayerList)
    {
        //変数宣言
        CDuel_Player::DuelData DuelData;

        DuelData.BaceData.nIndex = iter->GetIndex();        //番号
        DuelData.BaceData.RakNetID = iter->GetRakNetID();   //RakNetID
        DuelData.BaceData.Tag = iter->GetTag();             //タグ
        bsOut.Write(DuelData.BaceData);                     //書き出し

        //CPUがいるならこの段階でフラグを立てる
        if (iter->GetRakNetID() == static_cast<RakNet::RakNetGUID>(-1))
        {
            m_isCheckStart[iter->GetIndex()] = true;
        }
    }

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//対戦を開始するか
//=====================================
bool CDuel_Data::CheckStartBattle(RakNet::Packet* packet)
{
    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //受信したメッセージを飛ばす

    //リストの周回
    for (auto& iter : m_DuelPlayerList)
    {
        if (iter->GetRakNetID() == packet->guid)
        {
            m_isCheckStart[iter->GetIndex()] = true;
            break;
        }
    }
    
    //スタート可能かを返す
    for (auto iter : m_isCheckStart)
    {
        //一つでもフラグが立っていないならfalse
        if (iter == false)
        {
            return false;
        }
    }

    return true;
}

//=====================================
//対戦を開始
//=====================================
void CDuel_Data::StartBattle(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_START);

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//ステータスを送信
//=====================================
void CDuel_Data::SendStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //保存しているものから探す
    for (auto& iter : m_DuelPlayerList)
    {
        //破棄の処理
        if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }

        //m_DuelPlayerList.remove(iter);
    }

    //削除
    m_DuelPlayerList.clear();

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);
  
    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

    //4人分読み込み
    for (int i = 0; i < 4; i++)
    {
        //基底パラメータを代入
        CDuel_Player* pPlayer = new CDuel_Player; //クラスを作成し代入
        CDuel_Player::DuelData Data;
        bsIn.Read(Data);
        pPlayer->SetStatus(Data.Status);
        pPlayer->SetRakNetID(Data.BaceData.RakNetID);
        pPlayer->SetIndex(Data.BaceData.nIndex);
        pPlayer->SetTag(Data.BaceData.Tag);

        //追加
        m_DuelPlayerList.push_back(pPlayer);
    }

    //bsIn.Read(nId);
    //bsIn.Read(isReady);

    //// データの作成
    //RakNet::BitStream bsOut;
    //int PlayerNum = m_DuelPlayerList.size();

    ////書き出し
    //bsOut.Write((RakNet::MessageID)ID_DUEL_MESSAGE_SEND_STATUS);
   
    ////書き出し
    //for (auto iter : m_DuelPlayerList)
    //{
    //    //送信用のデータをまとめる
    //    CDuel_Player::DuelData SendData;
    //    SendData.BaceData.RakNetID = iter->GetRakNetID();   //識別番号
    //    SendData.BaceData.nIndex = iter->GetIndex();        //番号
    //    SendData.BaceData.Tag = iter->GetTag();             //タグ
    //    SendData.Status = iter->GetStatus();                //ステータス
    //    bsOut.Write(SendData);
    //}

    //// 全クライアントにブロードキャスト
    //peer->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);  //優先的に送ることで反映を先にする
}

//=====================================
//ステータスを受信
//=====================================
void CDuel_Data::ReceiveStatus(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //送信するかの確認
    if (!IsSendUpdate(packet)) return;

    //受信数の初期化
    m_nReceiveNum = 0;

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_STATUS);

    //書き出し
    for (auto iter : m_DuelPlayerList)
    {
        //送信用のデータをまとめる
        CDuel_Player::DuelData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();   //識別番号
        SendData.BaceData.nIndex = iter->GetIndex();        //番号
        SendData.BaceData.Tag = iter->GetTag();             //タグ
        SendData.Status = iter->GetStatus();                //ステータス
        bsOut.Write(SendData);
    }

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//更新の許可を出すかの確認
//=====================================
bool CDuel_Data::IsSendUpdate(RakNet::Packet* packet)
{
    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);
   
    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み
    CDuel_Player::DuelData Data;                    //データの読み込み
    bsIn.Read(Data);

    //同じ番号のステータスに反映
    for (auto& iter : m_DuelPlayerList)
    {
        //番号を確認
        if (iter->GetIndex() == Data.BaceData.nIndex)
        {
            //ステータスを代入
            iter->SetStatus(Data.Status);
            break;
        }
    }

    //受信数のカウント
    m_nReceiveNum++;

    //プレイヤーの人数を確認
    int PlayerNum = 0;
    for (auto iter : m_DuelPlayerList)
    {
        if (iter->GetTag() == CPlayer::TAG_PLAYER)
        {
            PlayerNum++;
        }
    }

    //テスト
    std::cout << PlayerNum << "/" << m_nReceiveNum << "\n";

    //プレイヤーの数だけ受信できていないなら許可しない
    if (m_nReceiveNum != PlayerNum) return false;

    return true;
}
