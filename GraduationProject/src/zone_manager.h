//===========================================================================================================================================================
// 
// ゾーンのマネージャーのヘッダー [zone_manager.h]
// Author : Kasai Keisuke
// 
//===========================================================================================================================================================

#ifndef _ZONE_MANAGER_H_
#define _ZONE_MANAGER_H_

// include
#include "cemetery_zone.h"
#include "deck_zone.h"
#include "field_zone.h"
#include "hand_zone.h"
#include "wait_zone.h"

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
		* @brief 墓地のポインタ取得
		* @return 墓地のポインタ
		*/
		CCemeteryZone* GetCemetery();

		/**
		* @brief デッキのポインタ取得
		* @return デッキのポインタ
		*/
		CDeckZone* GetDeck();

		/**
		* @brief フィールドゾーンのポインタ取得
		* @return フィールドゾーンのポインタ
		*/
		CFieldZone* GetFieldZone();

		/**
		* @brief ハンドゾーンのポインタ取得
		* @return ハンドゾーンのポインタ
		*/
		CHandZone* GetHandZone();

		/**
		* @brief 待機ゾーンのポインタ取得
		* @return 待機ゾーンのポインタ
		*/
		CWaitZone* GetWaitZone();

	private:
		CCemeteryZone* m_pCemetery;	// 墓地のポインタ
		CDeckZone* m_pDeck;			// 山札のポインタ
		CFieldZone* m_pFieldZone;	// フィールドゾーンのポインタ
		CHandZone* m_pHandZone;		// 手札ゾーンのポインタ
		CWaitZone* m_pWaitZone;		// 待機ゾーンのポインタ
	};
}
#endif
