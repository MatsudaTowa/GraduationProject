//=============================================
//
//ゲームマネージャー[gamemaneger.h]
//Auther MatsudaTowa
//
//=============================================

#ifndef _GAME_MANAGER_H_	//これが定義されてないとき
#define _GAME_MANAGER_H_	//二重マクロ防止

//ヘッダーのインクルード
#include "game_player.h"
#include "game_state.h"
#include "field.h"
#include "enemy_manager.h"
#include "area.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief ゲームマネージャークラス */
	class CGameManager
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CGameManager();

		/**
		 * @brief デストラクタ
		 */		
		~CGameManager();

		/**
		 * @brief 終了
		 */		
		void Uninit();

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CGameManager* GetInstance()
		{
			//静的インスタンス
			static CGameManager instance;
			return &instance;
		}

		/**
		 * @brief エネミーリスト取得
		 * @return エネミーリスト
		 */
		CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }

		/**
		 * @brief プレイヤーの設定
		 * @param [in]プレイヤーのポインタ
		 */		
		void SetPlayer(CGamePlayer* player) { m_pPlayer = player; }

		/**
		 * @brief プレイヤーの取得
		 * @return プレイヤーのポインタ
		 */		
		CGamePlayer* GetPlayer() { return m_pPlayer; }

		/**
		 * @brief エリアの設定
		 * @param [in]エリアのポインタ
		 * @param [in]どこのエリアか
		 */
		void SetArea(CArea* area,int idx) { m_pArea[idx] = area; }

		/**
		 * @brief エリアの取得
		 * @param [in]エリア番号
		 * @return エリアのポインタ
		 */
		CArea* GetArea(int idx) { return m_pArea[idx]; }

		/**
		 * @brief 床の設定
		 * @param [in]床のポインタ
		 */
		void SetField(CField* field) { m_pField = field; }

		/**
		 * @brief 床の取得
		 * @return 床のポインタ
		 */
		CField* GetField() { return m_pField; }

		/**
		 * @brief ゲームのステートの設定
		 * @param [in]ゲームのステートのポインタ
		 */
		void SetState(CGameState* state) { m_pState = state; }

		/**
		 * @brief ゲームのステート取得
		 * @return ゲームのステートポインタ
		 */
		CGameState* GetState() { return m_pState; }

		/**
		 * @brief ステート変更
		 * @param [in]次のステート
		 */
		void ChangeState(CGameState* state);

		/**
		 * @brief ステート実行処理
		 * @param [in]ゲームのシーンポインタ
		 */
		void GameStateExecution(CGame* game)
		{
			if (m_pState == nullptr) { return; }

			m_pState->Normal(game);
			m_pState->Pause(game);
		}
	
		/**
		 * @brief 終了設定
		 * @param [in]終了させるか
		 */
		inline void SetFinish(bool is_finish)
		{
			m_isFinish = is_finish;
		}

		/**
		 * @brief 終了取得
		 * @return 終了させるか
		 */
		inline bool GetFinish()
		{
			return m_isFinish;
		}
	private:
		CEnemyManager* m_pEnemyManager;
		CArea* m_pArea[CInputMouse::AREA::MAX - 1]; //真ん中のエリア以外に三角形を生成
		CGameState* m_pState;				//!<ゲームのステート状態
		CGamePlayer* m_pPlayer;				//!<プレイヤー
		CField* m_pField;					//!<床
		bool m_isFinish;					//!ゲームの処理を終わらせるか
	};
}

#endif