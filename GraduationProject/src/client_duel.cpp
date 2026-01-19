//================================
//
// Client用のcpp[client.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "client_duel.h"
#include "game_player.h"
#include "enemy.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "active_scene_player_state.h"
#include "card_deffence.h"
#include "zone_manager.h"
#include "duel_manager.h"
#include "card.h"
#include "damage_number_UI_factory.h"

//=====================================
//コンストラクタ
//=====================================
CClient_Duel::CClient_Duel() :
    m_DuelPlayerList()	//リスト
{

}

//=====================================
//デストラクタ
//=====================================
CClient_Duel::~CClient_Duel()
{

}

//=====================================
//初期化処理
//=====================================
bool CClient_Duel::Init()
{
    return true;
}

//=====================================
//終了処理
//=====================================
void CClient_Duel::Uninit()
{

}

//=====================================
//登録処理
//=====================================
void CClient_Duel::Regist(RakNet::Packet* /*packet*/)
{

}

//=====================================
//敵を生成するか
//=====================================
bool CClient_Duel::CheckEnemyCreate(int id, int max)
{
    //現在の敵を確認
    std::list<My::CEnemy*> enemy = My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();
    for (auto iter : enemy)
    {
        //すでに敵が生成している
        if (iter->GetPlayerIdx() == id)
        {
            return false;
        }
    }

    //自分が存在しない
    if (My::CActiveSceneManager::GetInstance()->GetPlayer() == nullptr && id == max - 1) return false;

    //プレイヤーと同じ番号か
    if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
    {
        if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() == id) return false;
    }

    return true;
}

//=====================================
//削除処理
//=====================================
void CClient_Duel::Delete(RakNet::Packet* packet)
{
    // 受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;
    int nPlayerNum = 0;
    bsIn.Read(messageId);
    bsIn.Read(nPlayerNum);

    //接続人数が0以下なら抜ける
    //if (nPlayerNum <= 0) return;

    ////中身を空に
    //m_DuelPlayerList.clear();
    //int nStart = 0;         //ずらし始める番号

    ////人数分読み込み
    //for (int i = 0; i < nPlayerNum; i++)
    //{
    //    DuelPlayerParam Param;
    //    bsIn.Read(Param.Param);
    //    m_DuelPlayerList.push_back(Param);

    //    if (Param.Param.nIndex == i)
    //    {
    //        nStart++;
    //    }
    //}

    ////現在の敵を確認
    ////std::list<My::CEnemy*> EnemyList = My::CGameManager::GetInstance()->GetEnemyManager()->GetList();

    ////番号がずれている敵がいたら埋める
    //for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
    //{
    //    //消えた番号より大きいならずらす
    //    if (iter->GetPlayerIdx() > nStart)
    //    {
    //        iter->SetPlayerIdx(iter->GetPlayerIdx() - 1);
    //    }
    //    else if (iter->GetPlayerIdx() == nStart)
    //    {//消える番号と一致したプレイヤーは削除
    //        iter->SetisDelete(true);
    //        My::CActiveSceneManager::GetInstance()->GetEnemyManager()->Remove(iter);
    //    }
    //}

    ////プレイヤーの番号がずれるかを確認
    //if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
    //{
    //    if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() > nStart)
    //    {
    //        My::CActiveSceneManager::GetInstance()->GetPlayer()->SetPlayerIdx(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() - 1);
    //    }
    //}
}

//=====================================
//準備完了送信処理
//=====================================
void CClient_Duel::SendReady(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/)
{

}

//=====================================
//準備完了受信処理
//=====================================
void CClient_Duel::ReceiveReady(RakNet::Packet* /*packet*/)
{

}

//=====================================
//カードキャスト処理
//=====================================
void CClient_Duel::CardCast(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //受信する変数
    //人数を取得
    unsigned char messageId;                //メッセージ
    My::CCard::CARDTYPE_ CardType;          //カードのタイプ
    My::CCard::CastDestination Destination; //キャスト先
    int nUserId = 0;                        //使用者番号
    int nCardId = 0;                        //カード番号
    int nSameTypeId = 0;                    //同じカードの番号
    uint64_t nCastTime = 0;                 //キャスト時間
    int nTargetSize = 0;                    //ターゲットの数
    std::vector<int> Target;                //ターゲット

    //読み込み
    bsIn.Read(messageId);          //メッセージ
    bsIn.Read(CardType);           //カードの種類
    bsIn.Read(nUserId);            //使用者番号
    bsIn.Read(nCardId);            //カードの番号
    bsIn.Read(nSameTypeId);        //同じカード番号
    bsIn.Read(Destination);        //キャスト先
    bsIn.Read(nCastTime);          //キャストした時間

    //bsIn.Read(nTargetSize);        //ターゲットの数

    ////周回
    //for (int i = 0; i < nTargetSize; ++i)
    //{
    //    //ターゲットの読み込み
    //    int nTargetId = 0;
    //    bsIn.Read(nTargetId);
    //    Target.push_back(nTargetId);
    //}

    //キャストカードの取得
    My::CCard* pCastCard = GetUsedCastCard(nUserId, nCardId, nSameTypeId);
    pCastCard->SetStartCastTime(nCastTime);      //キャスト時間の設定
    pCastCard->SetCastDestination(Destination);  //キャスト先の送信
    pCastCard->LoadCardInfo(&bsIn);              //読み込み処理

    //===================================================================================================

     //引数の番号のプレイヤーを取得
    My::CActiveSceneCharacter* Character = My::CActiveSceneManager::GetInstance()->GetCharacter(nUserId);

    //対戦状態にキャスト
    My::CDuelCharacter* pState = dynamic_cast<My::CDuelCharacter*>(Character->GetState());

    pCastCard->SetUserArea(My::CActiveSceneManager::GetInstance()->GetCharacter(nUserId)->GetArea());

    //山札のカードを手札に移動
    pCastCard->ChangeState(My::CCardState::CARD_CAST, pState);

    My::CPlayerDuelState* pPlayerState = nullptr;
    pPlayerState = dynamic_cast<My::CPlayerDuelState*>(Character->GetState());

    //中身がないなら抜ける
    if (pPlayerState == nullptr) return;

    //手札の整理
    pPlayerState->GetHand()->SetHandCardPos(pPlayerState);
}

//=====================================
//引数のカードに合致したカードを返す
//=====================================
My::CCard* CClient_Duel::GetUsedCastCard(int userid, int cardid, int sametypeid)
{
    //引数の番号のプレイヤーを取得
    My::CActiveSceneCharacter* Character = My::CActiveSceneManager::GetInstance()->GetCharacter(userid);

    //対戦状態にキャスト
    My::CDuelCharacter* pState = dynamic_cast<My::CDuelCharacter*>(Character->GetState());

    //中身がないなら抜ける
    if (pState == nullptr) return nullptr;

    //返す用のポインタ
    My::CCard* pCastCard = nullptr;

    //カードのポインタ
    for (My::CCard* pCard : pState->GetZoneManager()->GetHandZone()->GetList())
    {
        if (pCard->GetBaseStatus().nCardID != cardid) continue; //カードのID
        if (pCard->GetSameTypeId() != sametypeid) continue;     //同じカード番号

        pCastCard = pCard;  //見つけたら代入
        break;
    }

    return pCastCard;
}

//=====================================
//基底パラメータの設定
//=====================================
void CClient_Duel::SetParam(std::list<CClient::PlayerParam> list)
{
    //引数のリスト周回
    for (auto iter : list)
    {
        //基底パラメータを代入
        DuelPlayerParam Param;
        Param.Param = iter;

        //追加
        m_DuelPlayerList.push_back(Param);
    }
}

//=====================================
//基底パラメータの取得
//=====================================
std::list<CClient::PlayerParam> CClient_Duel::GetParam()
{
    //パラメータ保存用
    std::list<CClient::PlayerParam> ParamList;
    ParamList.clear();  //初期化

    //現在のプレイヤーを保存
    for (auto iter : m_DuelPlayerList)
    {
        ParamList.push_back(iter.Param);
    }

    return ParamList;
}

//=====================================
//コンピューターの追加(ロビーで追加の処理が出来たらいらない)
//=====================================
void CClient_Duel::AddCPU(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ

    //読み込み
    bsIn.Read(messageId);

    //リストの削除
    m_DuelPlayerList.clear();

    int nEnemyCount = 0;

    //最大人数分周回
    for (int i = 0; i < 4; i++)
    {
        //受け取る情報の変数
        CClient_Duel::DuelPlayerParam Param;
        
        //基底情報の取得
        bsIn.Read(Param.Param);

        //リストに追加
        m_DuelPlayerList.push_back(Param);

        //CPUなら敵を生成
        //if (Param.Param.ClientID == static_cast<RakNet::RakNetGUID>(-1))
        {
            //カウントアップ
            nEnemyCount++;

            //すでに生成されている敵は生成しない
            if (static_cast<int>(My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList().size() + 1) < nEnemyCount)
            {
                My::CEnemy::Create({ i * 100.0f + 50.0f, 0.0f, 0.0f }, VEC3_RESET_ZERO, i);
            }
        }
    }

    //読み込みの成功を送る
    SendSuccessDuelLoad(peer);
}

//=====================================
//決闘の読み込み成功を送信
//=====================================
void CClient_Duel::SendSuccessDuelLoad(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_CLIENT_START);
    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        //サーバーにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//対戦の開始(中身が現状この世の終わり)
//=====================================
void CClient_Duel::StartBattle(RakNet::Packet* packet)
{
    //データを取得
    unsigned char messageId;    //メッセージ
    uint64_t StartTime = 0;     //開始時間

    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //読み込み
    bsIn.Read(messageId);
    bsIn.Read(StartTime);

    //対戦時のタイマーを開始
    My::CDuel_Manager::GetInstance()->GetDuelTimer().Start();
    My::CDuel_Manager::GetInstance()->GetDuelTimer().SetStartTime(StartTime);

    //ロビーから対戦に遷移
    //一時的にダウンキャストを行い、遷移の合図を送る
    My::CLobby* Lobby = nullptr;
    Lobby = dynamic_cast<My::CLobby*>(My::CActiveSceneManager::GetInstance()->GetState());

    //キャストが成功していたなら遷移の合図を送る
    if (Lobby != nullptr)
    {
        Lobby->SetBattleSign(true);
    }
}

//=====================================
//ステータスを送信
//=====================================
void CClient_Duel::SendStatus(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    //bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_SEND_STATUS);

    //対象を確認
    auto CheckTarget = [&](int id, My::CActiveSceneCharacter::Status status)
    {
        for (auto& iter : m_DuelPlayerList)
        {
            if (iter.Param.nIndex == id)
            {
                iter.Status = status;
                break;
            }
        }
    };

    //プレイヤーの代入
    if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
    {
        CheckTarget(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx(), My::CActiveSceneManager::GetInstance()->GetPlayer()->GetStatus());
    }

    //敵の情報を代入
    for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
    {
        CheckTarget(iter->GetPlayerIdx(), iter->GetStatus());
    }

    //リストの周回
    for (auto iter : m_DuelPlayerList)
    {
        bsOut.Write(iter);
    }

    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        //サーバーにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//ステータスを受信
//=====================================
void CClient_Duel::ReceiveStatus(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ

    //読み込み
    bsIn.Read(messageId);

    //対象を確認
    auto CheckTarget = [](int id, CClient_Duel::DuelPlayerParam param)
    {
        //番号で確認
        for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
        {
            //番号を確認
            if (iter->GetPlayerIdx() == id)
            {
                //TODO二重のダメージを防ぐための処理良くない処理
                param.Status.life = iter->GetStatus().life;

                iter->SetStatus(param.Status);
                return;
            }
        }

        //プレイヤーの番号で確認
        if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
        {
            if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() == id)
            {
                My::CActiveSceneManager::GetInstance()->GetPlayer()->SetStatus(param.Status);
                return;
            }
        }
    };

    //読み込み
    for (auto& iter : m_DuelPlayerList)
    {
        bsIn.Read(iter);
        CheckTarget(iter.Param.nIndex, iter);
    }

    //キャストカードの読み込み
   // ReceiveCastCard(&bsIn);     //通常
   // ReceiveCastDefCard(&bsIn);  //守備
}

//=====================================
//更新可能かを受信
//=====================================
void CClient_Duel::ReceiveIsUpdate(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ

    //読み込み
    bsIn.Read(messageId);
}

//=====================================
//自身のステータスを送信
//=====================================
void CClient_Duel::SendMyStatus(RakNet::RakPeerInterface* peer)
{
    //プレイヤーの中身がないなら返す
    if (My::CActiveSceneManager::GetInstance()->GetPlayer() == nullptr) return;

    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_STATUS);

    //リストの周回
    for (auto& iter : m_DuelPlayerList)
    {
        //周回
        if (iter.Param.nIndex != My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx()) continue;

        //ステータスの上書き
        iter.Status = My::CActiveSceneManager::GetInstance()->GetPlayer()->GetStatus();

        //書き出し
        bsOut.Write(iter);
    }

    //サーバーに送信
    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        //サーバーにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//カードドローの受信
//=====================================
void CClient_Duel::ReceiveDrawCard(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;                     //メッセージ
    int nUserId = 0;                             //使用者の番号
    My::CActiveSceneCharacter::Status Status;    //ステータス

    //読み込み
    bsIn.Read(messageId); //メッセージ
    bsIn.Read(nUserId);   //使用者の番号
    bsIn.Read(Status);    //ステータス

      //プレイヤーのポインタ
    My::CActiveSceneCharacter* pPlayer = nullptr;

    //受信した番号のプレイヤーを探す
    //for (auto& iter : My::CActiveSceneManager::GetInstance()->GetCharacterList())
    //{
    //    if (nUserId != iter->GetPlayerIdx()) continue;

    //    //プレイヤーの取得
    //    pPlayer = iter;
    //    break;
    //}

    pPlayer = My::CActiveSceneManager::GetInstance()->GetCharacter(nUserId);

    //対戦状態にキャスト
    My::CDuelCharacter* pState = dynamic_cast<My::CDuelCharacter*>(pPlayer->GetState());

    //中身がないなら抜ける
    if (pState == nullptr) return;

    //山札のカードを手札に移動
    My::CCard* pCard = pState->GetZoneManager()->GetDeck()->GetTopCard();
    pState->GetZoneManager()->MoveZone(pCard, pState->GetZoneManager()->GetDeck(), pState->GetZoneManager()->GetHandZone(), true);
    pCard->SetCurrentZone(My::CCard::HAND);

    //一時的な処理
    if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() != pCard->GetUserId())
    {
        //試しに画面外に飛ばす
        pCard->SetPos({ -1000.0f, 0.0f, 0.0f });
    }

    My::CPlayerDuelState* pPlayerState = nullptr; 
    pPlayerState = dynamic_cast<My::CPlayerDuelState*>(pPlayer->GetState());

    //中身がないなら抜ける
    if (pPlayerState == nullptr) return;

    pPlayerState->GetHand()->SetHandCardPos(pPlayerState);
}

//=====================================
//トリガーカードの受信
//=====================================
void CClient_Duel::ReceiveTriggerCard([[maybe_unused]] RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;      //メッセージ
    int nUserId = 0;              //使用者の番号
    int nCardId = 0;              //カード番号
    int nSameTypeId = 0;          //同種類の番号
    My::CCard::CARDTYPE_ CardType;//カード種類
    My::CActiveSceneCharacter::Status Status;    //ステータス

    //読み込み
    bsIn.Read(messageId);   //メッセージ
    bsIn.Read(nUserId);     //使用者の番号
    bsIn.Read(nCardId);     //カード番号
    bsIn.Read(nSameTypeId); //同種類の番号
    bsIn.Read(CardType);    //カードの種類

    //アクティブシーン
    My::CActiveSceneCharacter* pUsedPlayer = My::CActiveSceneManager::GetInstance()->GetCharacter(nUserId); //受信した番号のキャラクターを取得
    My::CDuelCharacter* pDuelState = dynamic_cast<My::CDuelCharacter*>(pUsedPlayer->GetState());            //取得したキャラクターの状態をキャスト
    My::CCard* TriggerCard = nullptr;                                                                       //トリガーしたカード

    //キャストゾーンのカードを周回
    for (My::CCard* pCard : pDuelState->GetZoneManager()->GetCastPreviewZone()->GetList())
    {
        //受信したデータに一致するカードを探す
        if (pCard->GetBaseStatus().nCardID != nCardId) continue;    //カード番号
        if (pCard->GetSameTypeId() != nSameTypeId) continue;        //同種の番号

        //見つけたら代入
        TriggerCard = pCard;
        break;
    }
    
    //トリガー受信時の処理
    TriggerCard->ReceiveTrigger();

    //リストを取得してソートする
    std::list<My::CActiveSceneCharacter*> SortList = My::CActiveSceneManager::GetInstance()->GetCharacterList();
    SortList.sort([](My::CActiveSceneCharacter* a, My::CActiveSceneCharacter* b)
        {
            return a->GetPlayerIdx() < b->GetPlayerIdx();
        });

    //番号順に周回
    for (auto& iter : SortList)
    {
        int nLife = iter->GetStatus().life;

        //ステータスの読み込み
        bsIn.Read(Status);      
        iter->SetStatus(Status);

        //体力に変動があればUIを表示
        if (nLife - Status.life > 0)
        {
            D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), iter->GetPos()); //スクリーン座標に変換
            My::CDamageNumberUIFactory::GetInstance()->Create(screen_pos, nLife - Status.life);
            //My::CDamageNumberUIFactory::GetInstance()->Create({640.0f, 360.0f, 0.0f}, nLife - Status.life);
        }
    }
}

//=====================================
//ゲームセットの受信
//=====================================
void CClient_Duel::ReceiveGameSet(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //変数宣言
    unsigned char messageId;                     //メッセージ
    
    //読み込み
    bsIn.Read(messageId); //メッセージ

    //終章フラグを立てる
    My::CActiveSceneManager::GetInstance()->SetFinish(true);
}
