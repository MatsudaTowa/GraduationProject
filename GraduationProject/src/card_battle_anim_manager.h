//================================
//
//card_battle_managerr.cppに必要な宣言[enemymanager.h]
//Author:yuuto shimadu
// 
//================================

#ifndef _CARD_BATTLE_ANIM_MANAGER_H_ //このマクロ定義がされていなかったら
#define _CARD_BATTLE_ANIM_MANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"
#include "card_battle_anim.h"

namespace My
{
	//エネミーマネージャーのクラス
	class CCardBattleAnimManager
	{
	public:
		//メンバ関数
		CCardBattleAnimManager();				//コンストラクタ
		~CCardBattleAnimManager();				//デストラクタ
		HRESULT Init();							//初期化
		void Uninit();							//終了
		void Regist(CCardBattleAnim* manager);	//登録
		void Remove(CCardBattleAnim* manager);	//削除
		void Update();

		//リストの取得
		std::list<CCardBattleAnim*> GetList() { return m_apManager; }

	private:

		//メンバ変数
		std::list<CCardBattleAnim*> m_apManager;	//敵を管理
	};
}

#endif