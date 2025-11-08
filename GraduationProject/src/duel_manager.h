//================================
//
//duel_manager.cppに必要な宣言[duel_manager.h]
//Author:松田永久
// 
//================================

#ifndef _DUEL_MANAGER_H_ //このマクロ定義がされていなかったら
#define _DUEL_MANAGER_H_ //２重インクルード防止のマクロ定義

namespace My
{
	class CActiveScenePlayer;
	class CCard;
	//デュエルマネージャーのクラス
	class CDuelManager
	{
	public:

		//メンバ関数
		CDuelManager();			//コンストラクタ
		~CDuelManager();			//デストラクタ
		HRESULT Init();				//初期化

		/**
		 * @brief 終了
		 */
		void Uninit();

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CDuelManager* GetInstance()
		{
			//静的インスタンス
			static CDuelManager instance;
			return &instance;
		}

		void RegistUseCardList(CCard* card);	//登録
		void RemoveUseCardList(CCard* card);	//削除

		//リストの取得
		inline std::list<CCard*> GetUseCardList() { return m_UseCardList; }
	private:
		std::list<CCard*> m_UseCardList; //このゲームで使うカードのリスト
	};
}

#endif