//================================
//
//card_manager.cppに必要な宣言[card_manager.h]
//Author:島津湧人
// 
//================================

#ifndef _CARD_MANAGER_H_ //このマクロ定義がされていなかったら
#define _CARD_MANAGER_H_ //２重インクルード防止のマクロ定義

//インクルード
//#include "card.h"
#include "card_client.h"

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
		void RegistCardList(CCard_Client::Param card);									//登録
		void RemoveCardList(CCard_Client::Param card);									//削除
		inline std::vector<CCard_Client::Param> GetUseCardVector() { return m_CardVector; }	//取得

		/**
		 * @brief 引数のIDのカード情報を取得
		 * @param [in]取得したいカードのID
		 * @return カードのパラメータ
		 */
		CCard_Client::Param GetCardParam(int id);

		/**
		 * @brief 生成
		 * @param [in]カードID
		 */
		void CreateCard(int id);
		
	private:

		//メンバ変数
		std::vector<CCard_Client::Param> m_CardVector; //このゲームで使うカードのリスト
	};
}

#endif