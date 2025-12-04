//================================
//
// デュエルプレイヤークラス用のヘッダー[duel_player.h]
// Author:Yuuto Shimadu
//
//================================

//二重マクロ防止
#ifndef _DUEL_PLAYER_H_
#define _DUEL_PLAYER_H_

//ヘッダーのインクルード
#include "player.h"
#include "zone_manager.h"

namespace My
{
	//デュエルプレイヤークラス
	class CDuel_Player : public CPlayer
	{
	public:

		//ステータス
		struct Status
		{
			int deckSize;	//!<デッキの枚数
			int trash;		//!<墓地枚数
			int hand;		//!<手札枚数
			int energy;		//!<エナジー
			int life;		//!<体力
		};

		//構造体
		struct DuelData
		{
			Data BaceData;							//基本のクライアントデータ
			Status Status = { 40, 0, 0, 0, 10 };	//ステータス
		};

		//関数
		CDuel_Player(); 	//コンストラクタ
		~CDuel_Player() {}	//デストラクタ

		//設定と取得
		//ステータス
		void SetStatus(Status status) { m_Status = status; }	//設定
		Status& GetStatus() { return m_Status; }				//取得

		//ゾーンマネージャーの取得
		My::CZoneManager* GetZoneManager() { return m_ZoneManager; }	//取得

	private:

		//変数
		Status m_Status = { 40, 0, 0, 0, 10 };	//ステータス

		//TODOここに対戦中に受け取る変数を追加
		My::CZoneManager* m_ZoneManager;
	};
}

#endif