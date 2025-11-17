//===============================================================================
//
//アクティブシーンのキャラクターステートパターン[active_scene_character_state.h]
//Author Matsuda Towa
//
//===============================================================================
#ifndef _ACTIVE_SCENE_CHARACTER_STATE_H_
#define _ACTIVE_SCENE_CHARACTER_STATE_H_

#include "character.h"

/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CActiveSceneCharacter;
	class CCharacterLobbyUIManager;
	class CZoneManager;
	
	/** @brief キャラクターステートクラス */
	class CActiveSceneCharacterState
	{
	public:
		CActiveSceneCharacterState() {};
		virtual ~CActiveSceneCharacterState() {};
		/**
		* @brief ロビー処理(親では何もしない)
		* @param [in]キャラクターポインタ
		*/
		virtual void Lobby([[maybe_unused]] CActiveSceneCharacter* character) {};
		/**
		* @brief デュエル処理(親では何もしない)
		* @param [in]キャラクターポインタ
		*/
		virtual void Duel([[maybe_unused]] CActiveSceneCharacter* character) {};
	};

	class CLobbyCharacter :public CActiveSceneCharacterState
	{
	public:
		struct LobbyData
		{
			bool isReady;
			int player_id;
		};
		CLobbyCharacter(CActiveSceneCharacter* character);
		~CLobbyCharacter() override;
		/**
		 * @brief ロビー
		 * @param [in]character
		 */
		virtual void Lobby(CActiveSceneCharacter* character) override;

		/**
		 * @brief データ設定
		 * @param [in]data
		 */
		inline void SetLobbyData(LobbyData data)
		{
			m_data = data;
		}

		/**
		 * @brief 準備OKか
		 * @param [in]isReady
		 */
		inline void SetIsReady(bool isReady)
		{
			m_data.isReady = isReady;
		}

		/**
		 * @brief ロビーデータ取得
		 * @return ロビーデータ
		 */
		inline LobbyData GetLobbyData()
		{
			return m_data;
		}

		/**
		 * @brief 準備OKか取得
		 * @return 準備OK
		 */
		inline bool GetIsReady()
		{
			return m_data.isReady;
		}

		/**
		 * @brief UI取得
		 * @return UI
		 */
		inline CCharacterLobbyUIManager* GetLobbyUI()
		{
			return m_pLobbyUImanager;
		}

		inline void SetLobbyUI(CCharacterLobbyUIManager* ui_manager)
		{
			m_pLobbyUImanager = ui_manager;
		}
	private:
		LobbyData m_data;
		CCharacterLobbyUIManager* m_pLobbyUImanager;			//!<ロビーUI
	};

	class CDuelCharacter :public CActiveSceneCharacterState
	{
	public:

		CDuelCharacter() {}
		CDuelCharacter(CActiveSceneCharacter* character);
		~CDuelCharacter() override;

		void Duel(CActiveSceneCharacter* character) override;

		/**
		 * @brief ゾーンマネージャー設定
		 * @param [in]zone
		 */
		inline void SetZoneManager(CZoneManager* zone)
		{
			m_pZoneManager = zone;
		}

		/**
		 * @brief ゾーンマネージャー取得
		 * @return ゾーンマネージャー
		 */
		inline CZoneManager* GetZoneManager()
		{
			return m_pZoneManager;
		}

	private:

		CZoneManager* m_pZoneManager;		//ゾーンマネージャー
	};
}
#endif // !_ACTIVE_SCENE_CHARACTER_STATE_H_
