//===============================================================================
//
//�A�N�e�B�u�V�[���̃L�����N�^�[�X�e�[�g�p�^�[��[active_scene_character_state.h]
//Author Matsuda Towa
//
//===============================================================================
#ifndef _ACTIVE_SCENE_CHARACTER_STATE_H_
#define _ACTIVE_SCENE_CHARACTER_STATE_H_

#include "character.h"

/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CActiveSceneCharacter;

	/** @brief �L�����N�^�[�X�e�[�g�N���X */
	class CActiveSceneCharacterState
	{
	public:
		CActiveSceneCharacterState() {};
		virtual ~CActiveSceneCharacterState() {};
		/**
		* @brief ���r�[����(�e�ł͉������Ȃ�)
		* @param [in]�L�����N�^�[�|�C���^
		*/
		virtual void Lobby([[maybe_unused]] CActiveSceneCharacter* character) {};
		/**
		* @brief �f���G������(�e�ł͉������Ȃ�)
		* @param [in]�L�����N�^�[�|�C���^
		*/
		virtual void Duel([[maybe_unused]] CActiveSceneCharacter* character) {};
	};
}
#endif // !_ACTIVE_SCENE_CHARACTER_STATE_H_
