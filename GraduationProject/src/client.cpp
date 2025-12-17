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
#include "active_scene_manager.h"
#include "raknet.h"

//=====================================
//コンストラクタ
//=====================================
CClient::CClient()// :
 //   m_PlayerParamList()	//リスト
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
void CClient::Regist(RakNet::Packet* /*packet*/)
{
   
}

//=====================================
//敵を生成するか
//=====================================
bool CClient::CheckEnemyCreate(int /*id*/, int /*max*/)
{
    return true;
}

//=====================================
//削除処理
//=====================================
void CClient::Delete(RakNet::Packet* /*packet*/)
{
   
}

//=====================================
//準備完了送信処理
//=====================================
void CClient::SendReady(RakNet::Packet* /*packet*/, RakNet::RakPeerInterface* /*peer*/)
{
    
}

//=====================================
//準備完了受信処理
//=====================================
void CClient::ReceiveReady(RakNet::Packet* /*packet*/)
{
    
}

//=====================================
//カードキャスト処理
//=====================================
void CClient::CardCast(RakNet::Packet* /*packet*/)
{
   
}

//=====================================
//デッキの受信処理
//=====================================
void CClient::ReceiveDeck(RakNet::Packet* packet)
{
    //受信側
    RakNet::BitStream bsIn(packet->data, packet->length, false);

    //人数を取得
    unsigned char messageId;    //メッセージ
    bsIn.Read(messageId);   //メッセージ

    std::list<My::CActiveSceneCharacter*> SortList = My::CActiveSceneManager::GetInstance()->GetCharacterList();
    SortList.sort([](My::CActiveSceneCharacter* a, My::CActiveSceneCharacter* b) 
        {
            return a->GetPlayerIdx() < b->GetPlayerIdx();
        });

    //キャラクターの周回
    for (My::CActiveSceneCharacter* iter : SortList)
    {
        int nDeckNum = 0;           //デッキ枚数
        int nCardId = 0;            //カードの番号

        //中身を削除
        iter->GetDeck().clear();

        //読み込み
        bsIn.Read(nDeckNum);   //デッキ枚数

        //デッキ枚数分読み込み
        for (int i = 0; i < nDeckNum; i++)
        {
            //カードのIDを読み込み代入
            bsIn.Read(nCardId);
            iter->GetDeck().push_back(nCardId);
        }
    }
}
