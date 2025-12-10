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

	//キャラクタータグの列挙
	enum TAG
	{
		TAG_PLAYER,
		TAG_CPU,
		TAG_MAX
	};

	//構造体
	struct Data
	{
		RakNet::RakNetGUID RakNetID;	//プレイヤー判別用ID
		int nIndex;						//プレイヤー番号
		TAG Tag;						//タグ
	};

	struct ChangeData
	{
		Data OnlineData;		//オンラインのデータ
		std::vector<int> Deck;	//デッキ
	};
	
	//関数
	CPlayer() : m_nIndex(-1), m_RakNetID(), m_Tag(TAG_PLAYER), m_Deck(){} 	//コンストラクタ
	~CPlayer() {}								//デストラクタ

	//設定と取得

	//プレイヤー判別用ID
	void SetRakNetID(RakNet::RakNetGUID id) { m_RakNetID = id; }	//設定
	RakNet::RakNetGUID& GetRakNetID() { return m_RakNetID; }		//取得

	//プレイヤー番号
	void SetIndex(int index) { m_nIndex = index; }	//設定
	int& GetIndex() { return m_nIndex; }			//取得

	//タグ
	void SetTag(TAG tag) { m_Tag = tag; }	//設定
	TAG& GetTag() { return m_Tag; }			//取得

	//デッキ
	void AddDeck(int id) { m_Deck.push_back(id); }			//追加
	void SetDeck(std::vector<int> deck) { m_Deck = deck; }	//設定
	std::vector<int> GetDeck() { return m_Deck; }			//取得

private:

	//変数
	RakNet::RakNetGUID m_RakNetID;	//プレイヤー判別用のID
	int m_nIndex;					//プレイヤー番号
	TAG m_Tag;						//タグ
	std::vector<int> m_Deck;		//デッキ
};

#endif