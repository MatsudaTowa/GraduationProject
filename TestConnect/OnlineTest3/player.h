//================================
//
// プレイヤークラス用のヘッダー[player.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

//ヘッダーのインクルード
#include "main.h"
#include "RakPeerInterface.h"

//プレイヤークラス
class CPlayer
{
public:

	//構造体
	struct Data
	{
		RakNet::RakNetGUID RakNetID;	//プレイヤー判別用ID
		int nIndex;						//プレイヤー番号
	};
	
	//関数
	CPlayer() : m_nIndex(-1), m_RakNetID(){} 	//コンストラクタ
	~CPlayer() {}								//デストラクタ

	//設定と取得

	//プレイヤー判別用ID
	void SetRakNetID(RakNet::RakNetGUID id) { m_RakNetID = id; }	//設定
	RakNet::RakNetGUID& GetRakNetID() { return m_RakNetID; }		//取得

	//プレイヤー番号
	void SetIndex(int index) { m_nIndex = index; }	//設定
	int& GetIndex() { return m_nIndex; }			//取得

private:

	//変数
	RakNet::RakNetGUID m_RakNetID;	//プレイヤー判別用のID
	int m_nIndex;					//プレイヤー番号
};

#endif