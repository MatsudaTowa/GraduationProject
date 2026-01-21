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
#include "duel_player_manager.h"
#include "duel_manager.h"
#include "card_client.h"

//=====================================
//コンストラクタ
//=====================================
CDuel_Data::CDuel_Data() :
    m_DuelPlayerList(),							//対戦プレイヤーのリスト
    m_isCheckStart{ false,false,false,false },	//開始の合図
    m_nReceiveNum(0),							//受信した値
    m_CastCardList(),							//キャストカードのリスト
    m_CastDiffenceCardVector()					//キャストされたディフェンスカードのベクター
{
   
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
    RakNet::BitStream bsIn(packet->data, packet->length, false);
    unsigned char messageId;    //受信したメッセージの種類
    int nId = -1;               //番号
    bool isReady = false;       //準備のフラグ

    //読み取り
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
void CDuel_Data::SetData(std::list<CPlayer::ChangeData> data)
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
        pPlayer->SetIndex(iter.OnlineData.nIndex);
        pPlayer->SetRakNetID(iter.OnlineData.RakNetID);
        pPlayer->SetTag(iter.OnlineData.Tag);
        pPlayer->SetDeck(iter.Deck);

        //デッキを混ぜる処理
        //std::list<int> DeckList = iter.Deck();
        std::vector<int> DeckVector = pPlayer->GetDeck();

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(DeckVector.begin(), DeckVector.end(), g);

        //カウント用のマップ
        std::map<int, int> SameTypeCounter;

        //同じ種類のカードの何番目かを算出
        auto CalcSameTypeCount = [&SameTypeCounter](int cardid)
        {
            //返す用
            int nCardId = -1;

            //要素を見つける
            auto nCount = SameTypeCounter.find(cardid);

            //あるなら
            if (nCount != SameTypeCounter.end())
            {
                //カウントアップ
                nCount->second++;
                SameTypeCounter[cardid] = nCount->second;
                nCardId = nCount->second;
            }
            else
            {
                //0の新しい要素を追加
                SameTypeCounter[cardid] = 0;
                nCardId = 0;
            }

            return nCardId;
        };

        //カードのIDから生成
        for (auto iter : DeckVector)
        {
            My::CCard* pCard = My::CCardManager::GetInstance()->CreateCard(iter);
            pCard->SetUserId(pPlayer->GetIndex());
            pCard->SetCurrentZone(My::CCard::DECK);
            pCard->Init();
            pCard->SetSameTypeId(CalcSameTypeCount(pCard->GetBaseStatus().nCardID));
            pPlayer->GetZoneManager()->GetDeck()->AddList(pCard);
        }

        //デッキの設定
        pPlayer->SetDeck(DeckVector);

        //追加
        m_DuelPlayerList.push_back(pPlayer);
    }
}

//=====================================
//プレイヤーリストの取得
//=====================================
std::list<CPlayer::ChangeData> CDuel_Data::GetData()
{
    //変数宣言
    std::list<CPlayer::ChangeData> List;
    List.clear();

    //現在のリストを基底にコピー
    for (const auto& iter : m_DuelPlayerList)
    {
        //基底構造体のリストに追加
        CPlayer::ChangeData Data;                          //変数
        Data.OnlineData.nIndex = iter->GetIndex();         //番号
        Data.OnlineData.RakNetID = iter->GetRakNetID();    //RakNetID
        Data.OnlineData.Tag = iter->GetTag();              //タグ
        Data.Deck = iter->GetDeck();                       //デッキ
        List.push_back(Data);                              //追加
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

    //デッキを送る
    //SendDeck(peer);
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
    //対戦時のタイマーを開始
    CDuel_Manager::GetInstance()->GetDuelTimer().Start();

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_START);        //メッセージ
    bsOut.Write(CDuel_Manager::GetInstance()->GetDuelTimer().GetStartTime());   //開始時間

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);

    //デッキ内容の送信
    SendDeck(peer);
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
void CDuel_Data::ReceiveCastCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //ログ
    std::cout << "カードの受信\n";

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //=========================================================================================

    //変数宣言
    CCard_Client::CardType nCardType;               //カードの種類
    int nUserId = 0;                                //使用者番号
    int nCardId = 0;                                //カード番号
    int nSameId = 0;                                //同種の中の番号
    My::CCard::CastDestination CastDestination;     //キャスト先

     //情報の読み込み
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み
    bsIn.Read(nCardType);           //カードの種類
    bsIn.Read(nUserId);             //使用者番号
    bsIn.Read(nCardId);             //カード番号
    bsIn.Read(nSameId);             //同じカードの番号
    bsIn.Read(CastDestination);     //キャスト先

    //カードを使用したプレイヤーを取得
    My::CDuel_Player* pPlayer = My::CDuel_Player_Manager::GetInstance()->GetDuelPlayer(nUserId);
    My::CCard* pCastCard = nullptr;
    
    //手札のカードを確認
    for (My::CCard* pCard : pPlayer->GetZoneManager()->GetHandZone()->GetList())
    {
        //一致するカードを確認
        if (pCard->GetBaseStatus().nCardID != nCardId) continue;
        if (pCard->GetSameTypeId() != nSameId) continue;

        //カードの代入
        pCastCard = pCard;
        break;
    }

    //カードの中身を確認
    if (!pCastCard) return;

    //コストが足りるかを確認(不足しているなら返す)
    if (pPlayer->GetStatus().energy < pCastCard->GetBaseStatus().nCost) return;

    //キャスト先の設定
    pCastCard->SetCastDestination(CastDestination);

    //キャスト情報の読み込み
    if (!pCastCard->LoadCastInfo(&bsIn, CastDestination)) return;

    //=========================================================================================

    //エナジーを消費してカードをキャスト
    //pPlayer->GetStatus().energy -= pCard->GetBaseStatus().nCost;    //エナジーを消費
    pCastCard->SetUserId(nUserId);
    //pCastCard->SetTargetIdVector(Target);
    pCastCard->SetStartCastTime(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime());
    //pCastCard->ChangeState(My::CCardState::CARD_CAST, pPlayer);
    //pCastCard->SetCastDestination(CastDestination);

    //キャストカードの送信
    SendCastCard(peer, pCastCard);
}

//=====================================
//キャストカードの送信
//=====================================
void CDuel_Data::SendCastCard(RakNet::RakPeerInterface* peer, My::CCard* castcard)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_CAST_CARD);    //メッセージ
    //bsOut.Write(castcard->GetUserId());                                         //使用者番号
    //bsOut.Write(castcard->GetBaseStatus().nCardID);                             //カード番号
    //bsOut.Write(castcard->GetSameTypeId());                                     //同じカードの番号
    //bsOut.Write(CDuel_Manager::GetInstance()->GetDuelTimer().GetElapsedTime()); //経過時間(使用時間)
    //bsOut.Write((int)castcard->GetTargetIdVector().size());                     //ターゲット数

    bsOut.Write(castcard->GetCardType());                                       //カードの種類
    bsOut.Write(castcard->GetUserId());                                         //使用者番号
    bsOut.Write(castcard->GetBaseStatus().nCardID);                             //カード番号
    bsOut.Write(castcard->GetSameTypeId());                                     //同種の何番目のカードか
    bsOut.Write(castcard->GetCastDestination());                                //キャスト先の列挙
    bsOut.Write(castcard->GetStartCastTime());                                  //カードのキャスト開始時間

    //カード情報の書き出し
    castcard->SendCastInfo(bsOut);

    ////ターゲット数だけ周回して書き出し
    //for (int TargetId : castcard->GetTargetIdVector())
    //{
    //    bsOut.Write(TargetId);  //数の書き出し
    //}

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
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
    //タイマーの更新
    CDuel_Manager::GetInstance()->GetDuelTimer().Update();

    //ステイカードの更新
    UpdateStayCard();

    //プレイヤーの更新
    UpdateDuelPlayer(CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime());
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
        //エナジーの更新
        if (iter->UpdateEnergy(delta))
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

//======================================
//デッキの読み込み
//======================================
bool CDuel_Data::ReceiveDeck(RakNet::Packet* packet)
{
    //変数
    int nPlayerId = 0;  //プレイヤー番号
    int nDeckNum = 0;   //デッキ枚数
    int nCardId = 0;    //カード番号

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み

    //パラメータ
    bsIn.Read(nPlayerId);
    bsIn.Read(nDeckNum);

    //対応したプレイヤーを見つける
    CPlayer* pPlayer = nullptr;

    //受信したIDのプレイヤーを探す
    for (auto& iter : m_DuelPlayerList)
    {
        if (nPlayerId != iter->GetIndex()) continue;

        //代入
        pPlayer = iter;
        break;
    }

    pPlayer->GetDeck().clear();

    //デッキ枚数の読み込み
    for (int i = 0; i < nDeckNum; i++)
    {
        //デッキカードの読み込み
        bsIn.Read(nCardId);
        pPlayer->AddDeck(nCardId);
    }

    return false;
}

//======================================
//デッキの送信
//======================================
void CDuel_Data::SendDeck(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    int nDeckNum = 0;
    int nCardId = 0;

    bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_DECK);

    //対戦相手の情報を共有
    for (const auto& iter : m_DuelPlayerList)
    {
        //デッキ枚数
        bsOut.Write((int)iter->GetDeck().size());

        //デッキのカードID
        for (int card : iter->GetDeck())
        {
            bsOut.Write(card);
        }
    }

    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

//======================================
//カードドローの受信
//======================================
void CDuel_Data::ReceiveDrawCard(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //変数宣言
    int nPlayerId = 0;                      //プレイヤー番号
    My::CDuel_Player* pPlayer = nullptr;    //プレイヤーのポインタ

    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み

    //パラメータ
    bsIn.Read(nPlayerId);   //プレイヤー番号

    //カードドローを送信
    SendDrawCard(peer, nPlayerId);
}

//======================================
//ドローの確認
//======================================
bool CDuel_Data::CheckDrawCard(int playerid)
{
    //ドローの条件確認
    if (playerid < 0 && playerid >= 4) return false;    //対戦中の番号が送られているか

    //番号のプレイヤーを取得
    My::CDuel_Player* pPlayer = My::CDuel_Player_Manager::GetInstance()->GetDuelPlayer(playerid);

    //中身の確認
    if (!pPlayer) return false;

    return true;    //なにこれ
}

//======================================
//カードドローの送信
//======================================
void CDuel_Data::SendDrawCard(RakNet::RakPeerInterface* peer, int userid)
{
    //プレイヤーのポインタ
    My::CDuel_Player* pPlayer = nullptr;

    //受信した番号のプレイヤーを探す
    for (auto& iter : m_DuelPlayerList)
    {
        if (userid != iter->GetIndex()) continue;

        //プレイヤーの取得
        pPlayer = iter;
    }

    //カードのドロー処理
    if (pPlayer->DrawCard())
    {
        //データの送信
        RakNet::BitStream bsOut;

        //書き出し
        bsOut.Write((RakNet::MessageID)GameMessages::ID_DUEL_MESSAGE_DRAW); //メッセージ
        bsOut.Write(pPlayer->GetIndex());                                    //使用者番号
        bsOut.Write(pPlayer->GetStatus());                                   //使用者のステータス

        //全クライアントにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
    }
}

//======================================
//プレイヤーが存在するか
//======================================
bool CDuel_Data::IsPlayerExist()
{
    return !m_DuelPlayerList.empty();
}

//======================================
//プレイヤーが存在するか
//======================================
bool CDuel_Data::ReceiveGameSet(RakNet::Packet* packet)
{
    //データの受信
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み取り
    bsIn.IgnoreBytes(sizeof(RakNet::MessageID));    //メッセージの読み込み

    //プレイヤーの人数
    int nPlayerNum = 0;

    //受信時にカウントを進める
    m_nGameSetPlayerNum++;

    //対戦プレイヤーの周回
    for (const auto& iter : m_DuelPlayerList)
    {
        if (iter->GetTag() == CPlayer::TAG_PLAYER)
        {
            nPlayerNum++;
        }
    }

    //プレイヤーの人数分受信したら真を返す
    if (nPlayerNum == m_nGameSetPlayerNum) return true;

    return false;
}
