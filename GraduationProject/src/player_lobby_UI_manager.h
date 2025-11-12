//=============================================
//
//プレイヤーロビー全体のUI管理「player_lobby_UI_manager.h」
// Author松田永久
//
//=============================================
#ifndef _PLAYER_LOBBY_UI_MANAGER_H_ //これが定義されてないとき
#define _PLAYER_LOBBY_UI_MANAGER_H_
#include "main.h"
#include "active_scene_character.h"
#include "character_lobby_UI_manager.h"
namespace My
{
	class CReadyButton;
	class CMatchStartButton;
	//=============================================
	//ロビー全体のUIクラス
	//=============================================
	class CPlayerLobbyUIManager:public CCharacterLobbyUIManager
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CPlayerLobbyUIManager();
		/**
		 * @brief デストラクタ
		 */
		~CPlayerLobbyUIManager() override;
		/**
		 * @brief 初期化
		 * @param [in]キャラクターのポインタ
		 * @return 成功したか
		 */
		HRESULT Init(CActiveSceneCharacter* character) override;
		/**
		 * @brief 終了
		 */
		void Uninit()override;
		/**
		 * @brief 更新
		 */
		void Update()override;

		inline CReadyButton* GetReadyButton()
		{
			return m_pReadyButton;
		}

		inline void SetReadyButton(CReadyButton* readyButton)
		{
			m_pReadyButton = readyButton;
		}

		inline CMatchStartButton* GetStartButton()
		{
			return m_pStartButton;
		}

		inline void SetReadyButton(CMatchStartButton* startButton)
		{
			m_pStartButton = startButton;
		}

	private:
		CReadyButton* m_pReadyButton;
		CMatchStartButton* m_pStartButton;
	};
}


#endif