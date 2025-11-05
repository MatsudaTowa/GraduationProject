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
void CClient_Duel::Regist(RakNet::Packet* packet)
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
    if (nPlayerNum <= 0) return;

    //中身を空に
    m_DuelPlayerList.clear();
    int nStart = 0;         //ずらし始める番号

    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        DuelPlayerParam Param;
        bsIn.Read(Param.Param);
        m_DuelPlayerList.push_back(Param);

        if (Param.Param.nIndex == i)
        {
            nStart++;
        }
    }

    //現在の敵を確認
    //std::list<My::CEnemy*> EnemyList = My::CGameManager::GetInstance()->GetEnemyManager()->GetList();

    //番号がずれている敵がいたら埋める
    for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
    {
        //消えた番号より大きいならずらす
        if (iter->GetPlayerIdx() > nStart)
        {
            iter->SetPlayerIdx(iter->GetPlayerIdx() - 1);
        }
        else if (iter->GetPlayerIdx() == nStart)
        {//消える番号と一致したプレイヤーは削除
            iter->SetisDelete(true);
            My::CActiveSceneManager::GetInstance()->GetEnemyManager()->Remove(iter);
        }
    }

    //プレイヤーの番号がずれるかを確認
    if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
    {
        if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() > nStart)
        {
            My::CActiveSceneManager::GetInstance()->GetPlayer()->SetPlayerIdx(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() - 1);
        }
    }
}

//=====================================
//準備完了送信処理
//=====================================
void CClient_Duel::SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{

}

//=====================================
//準備完了受信処理
//=====================================
void CClient_Duel::ReceiveReady(RakNet::Packet* packet)
{

}

//=====================================
//カードキャスト処理
//=====================================
void CClient_Duel::CardCast(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;
    int nPlayerNum = 0;
    bsIn.Read(messageId);
    bsIn.Read(nPlayerNum);

    //接続人数が0以下なら抜ける
    if (nPlayerNum <= 0) return;

    //中身を空に
    m_DuelPlayerList.clear();
    int nStart = 0;         //ずらし始める番号

    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        bool isReady = false;
        bsIn.Read(isReady);
    }
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
    int nPlayerNum = 0;         //プレイヤー数

    //読み込み
    bsIn.Read(messageId);

    //リストの削除
    m_DuelPlayerList.clear();

    //最大人数分周回
    for (int i = 0; i < 4; i++)
    {
        //受け取る情報の変数
        CClient_Duel::DuelPlayerParam Param;
        //Param.isReady = false;

        //基底情報の取得
        bsIn.Read(Param.Param);

        //リストに追加
        m_DuelPlayerList.push_back(Param);

        //CPUなら敵を生成
        if (Param.Param.ClientID == static_cast<RakNet::RakNetGUID>(-1))
        {
            My::CEnemy::Create({ i * 100.0f + 50.0f, 0.0f, 0.0f }, VEC3_RESET_ZERO, i);
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
void CClient_Duel::StartBattle()
{
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
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_DUEL_MESSAGE_SEND_STATUS);

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
    int nPlayerNum = 0;         //プレイヤー数

    //読み込み
    bsIn.Read(messageId);

    //対象を確認
    auto CheckTarget = [](int id, CClient_Duel::DuelPlayerParam param)
    {
        //番号で確認
        for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
        {
            //消えた番号より大きいならずらす
            if (iter->GetPlayerIdx() == id)
            {
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

    //更新可能のフラグを受け取る
    CRakNet::GetInstance()->SetIsUpdate(true);
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
    for (auto iter : m_DuelPlayerList)
    {
        //周回
        if (iter.Param.nIndex != My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx()) continue;

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
