//================================
//
// 送受信データクラス用のヘッダー[taknet_data.cpp]
// Author:Yuuto Shimadu
//
//================================

//ヘッダーのインクルード
#include "raknet_data.h"

//=====================================
//新しいクライアントの接続処理
//=====================================
void CRakNet_Data::NewConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    
}

//=====================================
//クライアントの切断処理
//=====================================
void CRakNet_Data::DisConnection(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
    
}

//=====================================
//プレイヤー数を送信する関数
//=====================================
void CRakNet_Data::SendPlayerNum(RakNet::RakPeerInterface* peer, GameMessages message)
{
    
}

//=====================================
//準備確認の関数
//=====================================
void CRakNet_Data::Ready(RakNet::Packet* packet, RakNet::RakPeerInterface* peer)
{
  
}

//=====================================
//デッキの受信
//=====================================
bool CRakNet_Data::ReceiveDeck(RakNet::Packet* packet)
{
    return false;
}

//=====================================
//デッキのリクエストを送信
//=====================================
void CRakNet_Data::SendRequestDeck(RakNet::RakPeerInterface* peer)
{
   //デッキの要求を送信
    RakNet::BitStream bsOut;
    bsOut.Write((RakNet::MessageID)ID_LOBY_MESSAGE_REQUEST_DECK);
 
    //全クライアントにブロードキャスト
    peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}
