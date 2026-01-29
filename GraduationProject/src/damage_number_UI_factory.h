//=============================================
//
//何Pかの表示UI[damage_number_UI_factory.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _DAMAGE_NUMBER_UI_FACTORY_H_ //これが定義されてないとき

#define _DAMAGE_NUMBER_UI_FACTORY_H_
#include "main.h"

namespace My
{
	//=============================================
	//ダメージ数のUIを生成するクラス
	//=============================================
	class CDamageNumberUIFactory
	{
	public:
		
		/**
		 * @brief デストラクタ
		 */
		~CDamageNumberUIFactory();

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CDamageNumberUIFactory* GetInstance()
		{
			//静的インスタンス
			static CDamageNumberUIFactory instance;
			return &instance;
		}

		/**
		 * @brief 生成
		 * @param [in]位置
		 * @param [in]ダメージ数
		 * @return
		 */
		static void Create(D3DXVECTOR3 pos, int damage);
	private:

		//定数
		static constexpr float SPACE{ 50.0f };

		/**
		 * @brief コンストラクタ
		 */
		CDamageNumberUIFactory();
	};
}

#endif