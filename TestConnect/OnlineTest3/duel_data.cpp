//================================
//
// 送受信データクラス用のヘッダー[duel_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "duel_data.h"
#include "main.h"
#include "duel_player_manager.h"
#include "card.h"
#include "raknet_server.h"

//=====================================
//コンストラクタ
//=====================================
CDuel_Data::CDuel_Data() :
    m_DuelPlayerList(),							//対戦プレイヤーのリスト
    m_isCheckStart{ false,false,false,false },	//開始の合図
    m_nReceiveNum(0),							//受信した値
    m_CastCardList(),							//キャストカードのリスト
    m_CastDiffenceCardVector(),					//キャストされたディフェンスカードのベクター
    m_ndeltaTaime(0),							//デルタタイム
    m_nOldTime(0)								//前回の時間
{
    //コンストラクタ時の時間を取得
    m_nOldTime = timeGetTime();
}

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

        iter->SetTag(CPlayer::TAG::TAG_CPU);
        iter->SetRakNetID(static_cast<RakNet::RakNetGUID>(-1));   //RakNetID

        //破棄の処理
       /* if (iter != nullptr)
        {
            delete iter;
            iter = nullptr;
        }

        m_DuelPlayerList.remove(iter);*/
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
   /* for (auto& iter : m_DuelPlayerList)
    {
        iter->SetIndex(i);
        i++;
    }*/

    //更新の合図を出すか確認
    if (IsDisconnectionSendUpdate())
    {
        SendUpdateSign(peer);
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
        My::CDuel_Player::DuelData SendData;
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
        My::CDuel_Player::DuelData SendData;
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
        My::CDuel_Player* pPlayer = new My::CDuel_Player; //クラスを作成し代入
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
        My::CDuel_Player::DuelData DuelData;

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
        My::CDuel_Player* pPlayer = new My::CDuel_Player; //クラスを作成し代入
        My::CDuel_Player::DuelData Data;
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

    //更新の合図を送る
    SendUpdateSign(peer);

    //受信数の初期化
    //m_nReceiveNum = 0;

    ////データの作成
    //RakNet::BitStream bsOut;
    //bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_STATUS);

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

    ////キャストされたカードを送信
    //SendCastCard(&bsOut);

    ////全クライアントにブロードキャスト
    //peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
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
    My::CDuel_Player::DuelData Data;                    //データの読み込み
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
    //::cout << PlayerNum << "/" << m_nReceiveNum << "\n";

    //プレイヤーの数だけ受信できていないなら許可しない
    if (m_nReceiveNum != PlayerNum) return false;

    return true;
}

//=====================================
//キャストカードの受信
//=====================================
void CDuel_Data::ReceiveCastCard(RakNet::Packet* packet)
{
    //ログ
    std::cout << "カードの受信\n";

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み
    CastCardInfo CastInfo = {};                     //データの読み込み
    int nTargetNum = 0;                             //ターゲット数

    //カード情報の読み込み
    bsIn.Read(CastInfo.nCardID);        //カード情報
    bsIn.Read(CastInfo.nPlayerID);      //使用者番号

    bsIn.Read(nTargetNum);              //ターゲット数

    //ターゲット数だけ周回
    for (int i = 0; i < nTargetNum; i++)
    {
        int nTarget = 0;                    //ターゲット数
        bsIn.Read(nTarget);                 //読み込み

        CastInfo.m_TargetIDList.push_back(nTarget);  //リストに追加
    }

    //カード情報を保存
    m_CastCardList.push_back(CastInfo);
}

//=====================================
//キャストカードの送信
//=====================================
void CDuel_Data::SendCastCard(RakNet::BitStream* bsout)
{
    //return; //TODO : 作業中の為return

    //ログ
    std::cout << "カードの送信\n";

    //キャストされたカード枚数
    bsout->Write((int)m_CastCardList.size());

    //キャストカード情報の送信
    for (auto& iter : m_CastCardList)
    {
        bsout->Write(iter.nCardID);     //カード番号
        bsout->Write(iter.nPlayerID);   //プレイヤーID

        bsout->Write((int)iter.m_TargetIDList.size());   //対象の数

        //対象の数だけ周回
        for (auto iter : iter.m_TargetIDList)
        {
            bsout->Write(iter); //対象の番号
        }

        //対象者のリストをクリア
        iter.m_TargetIDList.clear();
    }

    m_CastCardList.clear();
}

//=====================================
//キャスト守備カードの受信
//=====================================
void CDuel_Data::ReceiveCastDefCard(RakNet::Packet* packet)
{
    //ログ
    std::cout << "守備カードの受信\n";

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み
    CastDiffenceCardInfo CastInfo = {};             //データの読み込み
    int nTargetNum = 0;                             //ターゲット数

    //カード情報の読み込み
    bsIn.Read(CastInfo.nCardID);        //カード情報
    bsIn.Read(CastInfo.nUsePlayer);     //使用者番号

    bsIn.Read(nTargetNum);              //ターゲット数

    //ターゲット数だけ周回
    for (int i = 0; i < nTargetNum; i++)
    {
        DiffenceTarget Target;

        bsIn.Read(Target.nAttackCardUserId);           //読み込み
        bsIn.Read(Target.nTargetCard);                 //読み込み

        CastInfo.DiffenceTarget.push_back(Target);  //リストに追加
    }

    //カード情報を保存
    m_CastDiffenceCardVector.push_back(CastInfo);
}

//=====================================
//キャスト守備カードの送信
//=====================================
void CDuel_Data::SendCastDeffenceCard(RakNet::BitStream* bsout)
{
    //ログ
    std::cout << "カードの送信\n";

    //キャストされたカード枚数
    bsout->Write((int)m_CastDiffenceCardVector.size());

    //キャストカード情報の送信
    for (auto& iter : m_CastDiffenceCardVector)
    {
        bsout->Write(iter.nCardID);     //カード番号
        bsout->Write(iter.nUsePlayer);  //プレイヤーID

        bsout->Write((int)iter.DiffenceTarget.size());   //対象の数

        //対象の数だけ周回
        for (auto iter : iter.DiffenceTarget)
        {
            bsout->Write(iter.nAttackCardUserId);   //対象カードの使用者の番号
            bsout->Write(iter.nTargetCard);         //対象カードのベクターの番号
        }

        //対象者のリストをクリア
        iter.DiffenceTarget.clear();
    }

    m_CastDiffenceCardVector.clear();
}

//=====================================
//更新の合図を送る
//=====================================
void CDuel_Data::SendUpdateSign(RakNet::RakPeerInterface* peer)
{
    //受信数の初期化
    m_nReceiveNum = 0;

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_STATUS);

    //書き出し
    for (auto iter : m_DuelPlayerList)
    {
        //送信用のデータをまとめる
        My::CDuel_Player::DuelData SendData;
        SendData.BaceData.RakNetID = iter->GetRakNetID();   //識別番号
        SendData.BaceData.nIndex = iter->GetIndex();        //番号
        SendData.BaceData.Tag = iter->GetTag();             //タグ
        SendData.Status = iter->GetStatus();                //ステータス
        bsOut.Write(SendData);
    }

    //キャストされたカードを送信
    //SendCastCard(&bsOut);           //通常カード
    //SendCastDeffenceCard(&bsOut);   //守備カード

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//=====================================
//クライアントの切断時に更新の合図を送るか
//=====================================
bool CDuel_Data::IsDisconnectionSendUpdate()
{
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
    //std::cout << PlayerNum << "/" << m_nReceiveNum << "\n";

    //プレイヤーの数だけ受信できていないなら許可しない
    if (m_nReceiveNum != PlayerNum) return false;

    return true;
}

//======================================
//シーンの更新
//======================================
void CDuel_Data::UpdateScene(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //経過時間の算出
    DWORD CurrentTime = timeGetTime();					//現在の時間を取得
    m_ndeltaTaime = CurrentTime - m_nOldTime;           //経過した時間を算出
    m_nOldTime = CurrentTime;                           //現在の時間

    //ステイカードの更新
    UpdateStayCard();

    //プレイヤーの更新
    UpdateDuelPlayer(m_ndeltaTaime);
}

//======================================
//ステイカードの更新
//======================================
void CDuel_Data::UpdateStayCard()
{
    //ステイカードの保持
    std::list<My::CCard*> m_StayCardList;

    //各プレイヤーのステイカードを周回して確認(NOTE : ゾーンが変わる可能性があるので別の場所で更新するため)
    for (auto& player : My::CDuel_Player_Manager::GetInstance()->GetList())
    {
        //更新するカードの取得
        for (My::CCard* pCard : player->GetZoneManager()->GetCastPreviewZone()->GetList())
        {
            m_StayCardList.push_back(pCard);
        }

        //各レイヤーのステイカードを周回
        for (My::CCard* pStayCard : m_StayCardList)
        {
            pStayCard->Update(player);
        }

        //周回が終わったらクリア
        m_StayCardList.clear();
    }
}

//======================================
//プレイヤーの更新
//======================================
void CDuel_Data::UpdateDuelPlayer(int delta)
{
    //データを送るか
    bool isSend = false;

    //プレイヤーの更新
    for (auto& iter : My::CDuel_Player_Manager::GetInstance()->GetList())
    {
        if (iter->UpdateEnergy(m_ndeltaTaime))
        {
            isSend = true;
        }
    }

    //1人でもエナジーの更新があれば送信
    if (isSend)
    {
        CRakNet_Server::GetInstance()->SendStatus();
    }
}