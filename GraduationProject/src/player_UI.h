//=============================================
//
//プレイヤーのUI TODO:プレイヤー関連全てのUIをここにまとめて「player_UI.h」
// Author松田永久
//
//=============================================
#ifndef _PLAYER_UI_H_ //これが定義されてないとき
#define _PLAYER_UI_H_
#include "main.h"
#include "player_number_UI.h"
#include "active_scene_character.h"
#include "cemetery_button.h"
#include "button_deckdraw.h"

namespace My
{
	//=============================================
	//プレイヤーのUIクラス
	//=============================================
	class CPlayerUI
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CPlayerUI();
		/**
		 * @brief デストラクタ
		 */
		~CPlayerUI();
		/**
		 * @brief 初期化
		 * @param [in]キャラクターのポインタ
		 * @return 成功したか
		 */
		HRESULT Init(CActiveSceneCharacter* character);
		/**
		 * @brief 終了
		 */
		void Uninit();
		/**
		 * @brief 更新
		 */
		void Update();
		/**
		 * @brief 現在のプレイヤーUI設定
		 * @param [in]2D座標変換後
 		 * @param [in]キャラクターのポインタ
		 */
		void SetCurrentCharacter_UI(D3DXVECTOR3 screen_pos, CActiveSceneCharacter* character);

		/**
		 * @brief リセット
		 */
		void Reset();

		/**
		 * @brief 体力UIの設定
		 * @param [in]体力UIのポインタ
		 */
		inline void SetLifeUI(CLife_UI* plifeUI)
		{
			m_pLifeUI = plifeUI;
		}

		/**
		 * @brief エナジーUIの設定
		 * @param [in]エナジーUIのポインタ
		 */
		inline void SetEnergyUI(CEnergy_UI* pEnergyUI)
		{
			m_pEneryUI = pEnergyUI;
		}

		/**
		 * @brief 墓地の設定
		 * @param [in]ボタンののポインタ
		 */
		inline void SetCemeteryButton(CCemeteryButton* pCemeteryButton)
		{
			m_pCemeteryButton = pCemeteryButton;
		}

		/**
		 * @brief デッキドローボタンの設定
		 * @param [in]デッキドローボタンのポインタ
		 */
		inline void SetDeckDrawButton(CButtonDeckDraw* pDeckDrawButton)
		{
			m_pDeckDrawButton = pDeckDrawButton;
		}

		/**
		 * @brief 体力UIの取得
		 * @return [in]体力UIのポインタ
		 */
		inline CLife_UI* GetLifeUI()
		{
			return m_pLifeUI;
		}

		/**
		 * @brief エナジーUIの取得
		 * @return [in]エナジーUIのポインタ
		 */
		inline CEnergy_UI* GetEnergyUI()
		{
			return m_pEneryUI;
		}

		/**
		 * @brief 墓地ボタンの取得
		 * @return [in]墓地ボタンのポインタ
		 */
		inline CCemeteryButton* GetCemeteryButton()
		{
			return m_pCemeteryButton;
		}

		/**
		 * @brief デッキドローボタン取得
		 * @return [in]デッキドローボタンのポインタ
		 */
		inline CButtonDeckDraw* GetDecDrawButton()
		{
			return m_pDeckDrawButton;
		}
	private:
		CPlayerNumberUI* m_pNumberUI;
		CLife_UI* m_pLifeUI;					//!<体力UI TODO:UIのインスタンスをキャラクターで持たせないように改良
		CEnergy_UI* m_pEneryUI;					//!<エナジーUI TODO:UIのインスタンスをキャラクターで持たせないように改良
		CCemeteryButton* m_pCemeteryButton;		//!<墓地ボタン
		CButtonDeckDraw* m_pDeckDrawButton;		// デッキドローボタン
	};
}


#endif