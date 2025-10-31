//================================
//
// Client用のcpp[client.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "client_lobby.h"
#include "game_player.h"
#include "enemy.h"
#include "active_scene_manager.h"
#include "raknet.h"
#include "active_scene_player_state.h"

//=====================================
//コンストラクタ
//=====================================
CClient_Lobby::CClient_Lobby() :
    m_LobbyPlayerList()	//リスト
{

}

//=====================================
//デストラクタ
//=====================================
CClient_Lobby::~CClient_Lobby()
{

}

//=====================================
//初期化処理
//=====================================
bool CClient_Lobby::Init()
{
    return true;
}

//=====================================
//終了処理
//=====================================
void CClient_Lobby::Uninit()
{

}

//=====================================
//登録処理
//=====================================
void CClient_Lobby::Regist(RakNet::Packet* packet)
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
    m_LobbyPlayerList.clear();

    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        LobbyPlayerParam Param;
        bsIn.Read(Param);
        m_LobbyPlayerList.push_back(Param);

        //敵を生成するか
        if (CheckEnemyCreate(i, nPlayerNum))
        {
            My::CEnemy::Create({ i * 100.0f + 50.0f, 0.0f, 0.0f }, VEC3_RESET_ZERO, i);
        }
    }

    //自身の番号を確認
    int nLap = 0;   //周回回数
    for (auto& iter : m_LobbyPlayerList)
    {
        //自分より番号が小さいプレイヤーが抜けていたらその番号にする
        if (nLap < iter.Param.nIndex)
        {
            iter.Param.nIndex = nLap;
        }

        //初期状態なら最後の番号を代入
        if (My::CActiveSceneManager::GetInstance()->GetPlayer() == nullptr && nLap == m_LobbyPlayerList.size() - 1)
        {
            My::CPlayer::Create(new My::CActiveScenePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_LobbyPlayerList.size() - 1);
        }

        ++nLap; //インクリメント
    }
}

//=====================================
//敵を生成するか
//=====================================
bool CClient_Lobby::CheckEnemyCreate(int id, int max)
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
void CClient_Lobby::Delete(RakNet::Packet* packet)
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
    m_LobbyPlayerList.clear();
    int nStart = 0;         //ずらし始める番号

    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        LobbyPlayerParam Param;
        bsIn.Read(Param);
        m_LobbyPlayerList.push_back(Param);

        if (Param.Param.nIndex == i)
        {
            nStart++;
        }
    }

    //現在の敵を確認
    //std::list<My::CEnemy*> EnemyList = My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList();

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
void CClient_Lobby::SendReady(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bool isRaedy = false;
    int nID = My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx();
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_LOBY_MESSAGE_SEND_READY);

    //準備フラグの書き出し
    bsOut.Write(nID);   //番号

    //キャストを試す
    My::CLobbyCharacter* pState = dynamic_cast<My::CLobbyCharacter*>(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetState());

    //キャストが成功していたら
    if (pState != nullptr)
    {
        //準備フラグの取得
        isRaedy = pState->GetIsReady();  //代入

         //プレイヤーの数だけ周回
        for (auto& iter : m_LobbyPlayerList)
        {
            if (iter.Param.nIndex == nID)
            {
                iter.isReady = isRaedy;
            }
        }
    }

    bsOut.Write(isRaedy);   //準備フラグ

    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        // 全クライアントにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//準備完了受信処理
//=====================================
void CClient_Lobby::ReceiveReady(RakNet::Packet* packet)
{
    // 受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ
    int nPlayerNum = 0;         //プレイヤー数
    int nCount = 0;             //回数のカウント

    //読み込み
    bsIn.Read(messageId);
    bsIn.Read(nPlayerNum);

    //接続人数が0以下なら抜ける
    if (nPlayerNum <= 0) return;

    //番号を確認し準備フラグを代入する関数
    auto CheckTarget = [&](int id, bool ready)
    {
        //引数の番号の対象か確認
        for (auto& iter : My::CActiveSceneManager::GetInstance()->GetEnemyManager()->GetList())
        {
            //消えた番号より大きいならずらす
            if (iter->GetPlayerIdx() == id)
            {
                //ここで代入
                My::CLobbyCharacter* pState = dynamic_cast<My::CLobbyCharacter*>(iter->GetState());

                //キャストが成功していたら
                if (pState != nullptr)
                {
                    pState->SetIsReady(ready);  //代入
                }
                return;
            }
        }

        //プレイヤーの番号がずれるかを確認
        if (My::CActiveSceneManager::GetInstance()->GetPlayer() != nullptr)
        {
            if (My::CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx() == id)
            {
                //キャストを試す
                My::CLobbyCharacter* pState = dynamic_cast<My::CLobbyCharacter*>(My::CActiveSceneManager::GetInstance()->GetPlayer()->GetState());

                //キャストが成功していたら
                if (pState != nullptr)
                {
                    pState->SetIsReady(ready);  //代入
                }
            }
        }
    };

    //プレイヤーの数だけ周回
    for (auto& iter : m_LobbyPlayerList)
    {
        //読み込み反映
        bool isReady = false;
        bsIn.Read(isReady);

        //代入
        iter.isReady = isReady;
        CheckTarget(nCount, isReady);

        //カウントアップ
        nCount++;
    }

    //=======================受け取ったデータを元に反映==================================
    //ホスト(1プレイヤー)なら全員完了していたら進むUIを表示
}

//=====================================
//カードキャスト処理
//=====================================
void CClient_Lobby::CardCast(RakNet::Packet* packet)
{
   
}

//=====================================
//基底パラメータの設定
//=====================================
void CClient_Lobby::SetParam(std::list<CClient::PlayerParam> list)
{
    //引数のリスト周回
    for (auto iter : list)
    {
        //基底パラメータを代入
        LobbyPlayerParam Param;
        Param.Param = iter;

        //追加
        m_LobbyPlayerList.push_back(Param);
    }
}

//=====================================
//基底パラメータの取得
//=====================================
std::list<CClient::PlayerParam> CClient_Lobby::GetParam()
{
    //パラメータ保存用
    std::list<CClient::PlayerParam> ParamList;
    ParamList.clear();  //初期化

    //現在のプレイヤーを保存
    for (auto iter : m_LobbyPlayerList)
    {
        ParamList.push_back(iter.Param);
    }

    return ParamList;
}

//=====================================
//開始の合図を送信
//=====================================
void CClient_Lobby::SendStartSign(RakNet::RakPeerInterface* peer)
{
    //データの作成
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_LOBY_MESSAGE_SEND_START);
    RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    //サーバーの確認
    if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    {
        // 全クライアントにブロードキャスト
        peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    }
}

//=====================================
//開始の合図を送信
//=====================================
void CClient_Lobby::SendAddEnemy(RakNet::RakPeerInterface* peer)
{
    ////データの作成
    //RakNet::BitStream bsOut;
    //bsOut.Write((RakNet::MessageID)CRakNet::GameMessages::ID_LOBY_MESSAGE_ADD_ENEMY);
    //RakNet::SystemAddress server_address = peer->GetSystemAddressFromIndex(0);

    ////サーバーの確認
    //if (server_address != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
    //{
    //    // 全クライアントにブロードキャスト
    //    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, peer->GetSystemAddressFromIndex(0), false);
    //}
}
