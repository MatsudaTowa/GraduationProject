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
#include "active_scene_state.h"
#include "field.h"
#include "enemy_manager.h"
#include "area_manager.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief ゲームマネージャークラス */
	class CActiveSceneManager
	{
	public:

		/**
		 * @brief コンストラクタ
		 */
		CActiveSceneManager();

		/**
		 * @brief デストラクタ
		 */		
		~CActiveSceneManager();

		/**
		 * @brief 終了
		 */		
		void Uninit();

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CActiveSceneManager* GetInstance()
		{
			//静的インスタンス
			static CActiveSceneManager instance;
			return &instance;
		}

		/**
		 * @brief エネミーリスト取得
		 * @return エネミーリスト
		 */
		CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }

		/**
		 * @brief エリアマネージャ取得
		 * @return エリアマネージャ
		 */
		CAreaManager* GetAreaManager() { return m_pAreaManager; }

		/**
		 * @brief エネミーリスト設定
		 * @param [in]エネミーリスト
		 */
		void SetEnemyManager(CEnemyManager* enemy_manager) { m_pEnemyManager = enemy_manager; }
		/**
		 * @brief エリアマネージャ設定
		 * @param [in]エリアマネージャ
		 */
		void SetAreaManager(CAreaManager* area_manager) { m_pAreaManager = area_manager; }

		/**
		 * @brief プレイヤーの設定
		 * @param [in]プレイヤーのポインタ
		 */		
		void SetPlayer(CActiveScenePlayer* player) 
		{ 
			m_pPlayer = player; 

			if (player == nullptr) return;
			m_nPlayerId = player->GetPlayerIdx();	//プレイヤーの番号の保持
		}

		/**
		 * @brief プレイヤーの取得
		 * @return プレイヤーのポインタ
		 */		
		CActiveScenePlayer* GetPlayer() { return m_pPlayer; }

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
		void SetState(CActiveSceneState* state) { m_pState = state; }

		/**
		 * @brief ゲームのステート取得
		 * @return ゲームのステートポインタ
		 */
		CActiveSceneState* GetState() { return m_pState; }

		/**
		 * @brief ステート変更
		 * @param [in]次のステート
		 */
		void ChangeState(CActiveSceneState* state);

		/**
		 * @brief ステート実行処理
		 * @param [in]ゲームのシーンポインタ
		 */
		void GameStateExecution(CActiveScene* game)
		{
			if (m_pState == nullptr) { return; }

			m_pState->Lobby(game);
			m_pState->Duel(game);
			m_pState->Pause(game);
			m_pState->CardCast(game);
			m_pState->Connect(game);
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

		/**
		 * @brief キャラクターをまとめたリストを返す
		 * @return キャラクターリスト
		 */
		std::list<CActiveSceneCharacter*> GetCharacterList();

		/**
		 * @brief 引数の番号を返す
		 * @param [id]取得したいプレイヤーの番号
		 * @return キャラクターのポインタ
		 */
		CActiveSceneCharacter* GetCharacter(int id);

		/**
		* @brief 対戦中の通信処理
		*/
		void DuelConnect();

		/**
		* @brief ターゲット矢印マネージャーの設定
		*/
		inline void SetTargetArrowManager(CTargetArrowManager* pManager) { m_pTargetArrowManager = pManager; }

		/**
		* @brief ターゲット矢印マネージャーの取得
		*/
		inline CTargetArrowManager* GetTargetArrowManager() { return m_pTargetArrowManager; }

		/**
		 * @brief プレイヤーの番号取得
		 * @return プレイヤーの番号
		 */
		int GetPlayerIndex() { return m_nPlayerId; }

	private:

		CEnemyManager* m_pEnemyManager;
		CAreaManager* m_pAreaManager;
		CActiveSceneState* m_pState;				//!<ゲームのステート状態
		CActiveScenePlayer* m_pPlayer;				//!<プレイヤー
		CField* m_pField;							//!<床
		bool m_isFinish;							//!ゲームの処理を終わらせるか
		int m_nPlayerId;							//!プレイヤーの番号(Resultで使う為)
		CTargetArrowManager* m_pTargetArrowManager;	// ターゲット矢印のマネージャー
	};
}

#endif