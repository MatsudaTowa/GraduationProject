//=============================================
//
//何Pかの表示UI[countdown_UI_factory.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COUNTDOWN_UI_FACTORY_H_ //これが定義されてないとき

#define _COUNTDOWN_UI_FACTORY_H_

//インクルード
#include "countdown_number_UI.h"
#include "countdown_start_UI.h"

namespace My
{
	//=============================================
	//カウントダウンのUIを生成するクラス
	//=============================================
	class CCountdownUIFactory
	{
	public:

		//カウントダウンUIの種類
		enum class KIND_OF_COUNTDOWN_UI
		{
			THREE,
			TWO,
			ONE,
			START,
		};

		/**
		 * @brief デストラクタ
		 */
		~CCountdownUIFactory();

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CCountdownUIFactory* GetInstance()
		{
			//静的インスタンス
			static CCountdownUIFactory instance;
			return &instance;
		}

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]ダメージ数
		 * @return
		 */
		void Create(D3DXVECTOR3 pos, KIND_OF_COUNTDOWN_UI ui);
	private:

		/**
		 * @brief コンストラクタ
		 */
		CCountdownUIFactory();

		//変数
		std::map<KIND_OF_COUNTDOWN_UI, CCountdownUI*> m_CountDownUI;
	};
}

#endif