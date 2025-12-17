//================================
//
//duel_player_manager.cppに必要な宣言[duel_player_manager.h]
//Author:島津湧人
// 
//================================

#ifndef _DUEL_PLAYER_MANAGER_H_ //このマクロ定義がされていなかったら
#define _DUEL_PLAYER_MANAGER_H_ //２重インクルード防止のマクロ定義

//インクルード
#include "duel_player.h"

namespace My
{
	//デュエルプレイヤーマネージャーのクラス
	class CDuel_Player_Manager
	{
	public:

		//メンバ関数
		CDuel_Player_Manager() : m_DuelPlayerList() {}			//コンストラクタ
		~CDuel_Player_Manager() { m_DuelPlayerList.clear(); }	//デストラクタ
		HRESULT Init();											//初期化
		void Uninit() { m_DuelPlayerList.clear(); }				//終了

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CDuel_Player_Manager* GetInstance()
		{
			//静的インスタンス
			static CDuel_Player_Manager instance;
			return &instance;
		}

		//リストの設定と取得
		void AddList(My::CDuel_Player* player) { m_DuelPlayerList.push_back(player); }	//追加
		std::list<My::CDuel_Player*> GetList() { return m_DuelPlayerList; }				//取得

		//引数のプレイヤーを取得
		My::CDuel_Player* GetDuelPlayer(int id)
		{
			//一致するプレイヤーを探して返す
			for (CDuel_Player* pPlayer : m_DuelPlayerList)
			{
				if (pPlayer->GetIndex() == id) return pPlayer;
			}

			return nullptr;
		}
		
	private:

		//メンバ変数
		std::list<My::CDuel_Player*> m_DuelPlayerList; //このゲームで使うカードのリスト
	};
}

#endif