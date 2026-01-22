//=============================================
//
//ロビー全体のUI管理「lobby_UI_manager.h」
// Author松田永久
//
//=============================================
#ifndef _LOBBY_UI_MANAGER_H_ //これが定義されてないとき
#define _LOBBY_UI_MANAGER_H_
#include "main.h"
#include "active_scene_character.h"
#include "lobby_character_icon_UI.h"
namespace My
{
	class CReadyUI;
	//=============================================
	//ロビー全体のUIクラス
	//=============================================
	class CCharacterLobbyUIManager
	{
	public:
		/**
		 * @brief コンストラクタ
		 */
		CCharacterLobbyUIManager();
		/**
		 * @brief デストラクタ
		 */
		virtual ~CCharacterLobbyUIManager();
		/**
		 * @brief 初期化
		 * @param [in]キャラクターのポインタ
		 * @return 成功したか
		 */
		virtual HRESULT Init(CActiveSceneCharacter* character);
		/**
		 * @brief 終了
		 */
		virtual void Uninit();
		/**
		 * @brief 更新
		 */
		virtual void Update();

		inline CReadyUI* GetReadyUI()
		{
			return m_pReadyUI;
		}

		inline void SetReadyUI(CReadyUI* readyUI)
		{
			m_pReadyUI = readyUI;
		}

		inline CLobbyCharacterIconUI* GetIconUI()
		{
			return m_pIconUI;
		}

		inline void SetIconUI(CLobbyCharacterIconUI* iconUI)
		{
			m_pIconUI = iconUI;
		}

		static CCharacterLobbyUIManager* GetInstance()
		{
			static CCharacterLobbyUIManager instance;
			return &instance;
		}

	private:
		CReadyUI* m_pReadyUI;

		/**
		* @brief ロビーキャラクターアイコンUI
		*/
		CLobbyCharacterIconUI* m_pIconUI;
	};
}


#endif