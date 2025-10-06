//================================
//
//enemymanager.cppに必要な宣言[enemymanager.h]
//Author:yuuto shimadu
// 
//================================

#ifndef _ENEMYMANAGER_H_ //このマクロ定義がされていなかったら
#define _ENEMYMANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"
#include "enemy.h"

namespace My
{
	//エネミーマネージャーのクラス
	class CEnemyManager
	{
	public:

		//メンバ関数
		CEnemyManager();			//コンストラクタ
		~CEnemyManager();			//デストラクタ
		HRESULT Init();				//初期化
		void Uninit();				//終了
		void Regist(CEnemy* enemy);	//登録
		void Erase(CEnemy* enemy);	//削除

		//リストの取得
		std::list<CEnemy*> GetList() { return m_apManager; }

	private:

		//メンバ変数
		std::list<CEnemy*> m_apManager;	//敵を管理
	};
}

#endif