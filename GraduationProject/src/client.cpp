//================================
//
// Client用のcpp[client.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "client.h"
#include "game_player.h"
#include "enemy.h"
#include "active_manager.h"

//=====================================
//コンストラクタ
//=====================================
CClient::CClient() :
    m_PlayerParamList(),	//リスト
    m_nMyID(-1)             //自身の番号
{

}

//=====================================
//デストラクタ
//=====================================
CClient::~CClient()
{

}

//=====================================
//初期化処理
//=====================================
bool CClient::Init()
{
    return true;
}

//=====================================
//終了処理
//=====================================
void CClient::Uninit()
{

}

//=====================================
//登録処理
//=====================================
void CClient::Regist(RakNet::Packet* packet)
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
    m_PlayerParamList.clear();

    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        PlayerParam Param;
        bsIn.Read(Param);
        m_PlayerParamList.push_back(Param);

        //敵を生成するか
        if (CheckEnemyCreate(i, nPlayerNum))
        {
            My::CEnemy::Create({i * 100.0f + 50.0f, 0.0f, 0.0f}, VEC3_RESET_ZERO, i);
        }
    }

    //自身の番号を確認
    int nLap = 0;   //周回回数
    for (auto& iter : m_PlayerParamList)
    {
        //自分より番号が小さいプレイヤーが抜けていたらその番号にする
        if (nLap < iter.nIndex)
        {
            iter.nIndex = nLap;
        }

        //初期状態なら最後の番号を代入
        if (m_nMyID == -1 && nLap == m_PlayerParamList.size() - 1)
        {
            m_nMyID = m_PlayerParamList.size() - 1;
            My::CPlayer::Create(new My::CGamePlayer, VEC3_RESET_ZERO, VEC3_RESET_ZERO, m_PlayerParamList.size() - 1);
        }

        ++nLap; //インクリメント
    }
}

//=====================================
//敵を生成するか
//=====================================
bool CClient::CheckEnemyCreate(int id, int max)
{
    //現在の敵を確認
    std::list<My::CEnemy*> enemy = My::CGameManager::GetInstance()->GetEnemyManager()->GetList();
    for (auto iter : enemy)
    {
        //すでに敵が生成している
        if (iter->GetPlayerIdx() == id)
        {
            return false;
        }
    }

    //自分が存在しない
    if (My::CGameManager::GetInstance()->GetPlayer() == nullptr && id == max - 1) return false;

    //プレイヤーと同じ番号か
    if (My::CGameManager::GetInstance()->GetPlayer() != nullptr)
    {
        if (My::CGameManager::GetInstance()->GetPlayer()->GetPlayerIdx() == id) return false;
    }

    return true;
}

//=====================================
//削除処理
//=====================================
void CClient::Delete(RakNet::Packet* packet)
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
    m_PlayerParamList.clear();
    int nStart = 0;         //ずらし始める番号
   
    //人数分読み込み
    for (int i = 0; i < nPlayerNum; i++)
    {
        PlayerParam Param;
        bsIn.Read(Param);
        m_PlayerParamList.push_back(Param);

        if (Param.nIndex == i)
        {
            nStart++;
        }
    }

    //現在の敵を確認
    //std::list<My::CEnemy*> EnemyList = My::CGameManager::GetInstance()->GetEnemyManager()->GetList();
 
    //番号がずれている敵がいたら埋める
    for (auto& iter : My::CGameManager::GetInstance()->GetEnemyManager()->GetList())
    {
        //消えた番号より大きいならずらす
        if (iter->GetPlayerIdx() > nStart)
        {
            iter->SetPlayerIdx(iter->GetPlayerIdx() - 1);
        }
        else if (iter->GetPlayerIdx() == nStart)
        {//消える番号と一致したプレイヤーは削除
            iter->Uninit();
        }
    }

    //プレイヤーの番号がずれるかを確認
    if (My::CGameManager::GetInstance()->GetPlayer() != nullptr)
    {
        if (My::CGameManager::GetInstance()->GetPlayer()->GetPlayerIdx() > nStart)
        {
            My::CGameManager::GetInstance()->GetPlayer()->SetPlayerIdx(My::CGameManager::GetInstance()->GetPlayer()->GetPlayerIdx() - 1);
        }
    }
}
