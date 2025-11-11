//================================
//
//card_manager.cppに必要な宣言[card_manager.h]
//Author:島津湧人
// 
//================================

#ifndef _CARD_MANAGER_H_ //このマクロ定義がされていなかったら
#define _CARD_MANAGER_H_ //２重インクルード防止のマクロ定義

//インクルード
#include "card.h"

namespace My
{
	//カードマネージャーのクラス
	class CCardManager
	{
	public:

		//メンバ関数
		CCardManager();			//コンストラクタ
		~CCardManager();		//デストラクタ
		HRESULT Init();			//初期化
		void Uninit();			//終了

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CCardManager* GetInstance()
		{
			//静的インスタンス
			static CCardManager instance;
			return &instance;
		}

		/**
		 * @brief カードのリストのリセット
		 */
		void ResetCardList();

		//リストの関数
		void RegistCardList(CCard* card);									//登録
		void RemoveCardList(CCard* card);									//削除
		inline std::list<CCard*> GetUseCardList() { return m_CardList; }	//取得
	private:

		//メンバ変数
		std::list<CCard*> m_CardList; //このゲームで使うカードのリスト
	};
}

#endif