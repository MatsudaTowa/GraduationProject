//===========================================================================================================================================================
// 
// ゾーンのマネージャーのヘッダー [zone_manager.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _ZONE_MANAGER_H_
#define _ZONE_MANAGER_H_

// include
#include "deck.h"

/** @brief My 名前空間 */
namespace My
{
	/**
	* @brief ゾーンマネージャークラス
	*/
	class CZoneManager
	{
	public:
		/**
		* @brief コンストラクタ
		*/
		CZoneManager();

		/**
		* @brief デストラクタ
		*/
		~CZoneManager();

		/**
		* @brief 初期化処理
		* @return 成功したかどうか
		*/
		HRESULT Init();

		/**
		* @brief 終了処理
		*/
		void Uninit();

		/**
		* @brief 更新処理
		*/
		void Update();

		/**
		* @brief デッキのポインタ取得
		*/
		CDeck* GetDeck();

	private:
		CDeck* m_pDeck;	// 山札のポインタ
	};
}
#endif
