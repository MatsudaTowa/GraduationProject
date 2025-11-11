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
	class CLobbyUIManager;

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
		inline CLobbyUIManager* GetLobbyUI()
		{
			return m_pLobbyUImanager;
		}
	private:
		LobbyData m_data;
		CLobbyUIManager* m_pLobbyUImanager;			//!<ロビーUI
	};

	class CDuelCharacter :public CActiveSceneCharacterState
	{
	public:
		void Duel(CActiveSceneCharacter* character) override;
	private:
	};
}
#endif // !_ACTIVE_SCENE_CHARACTER_STATE_H_
