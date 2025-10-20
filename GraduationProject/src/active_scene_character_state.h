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
		struct SEND_DATA
		{
			bool isDuel;

		};
		void Lobby(CActiveSceneCharacter* character) override;
	private:

	};

	class CDuelCharacter :public CActiveSceneCharacterState
	{
	public:
		void Duel(CActiveSceneCharacter* character) override;
	private:
	};
}
#endif // !_ACTIVE_SCENE_CHARACTER_STATE_H_
