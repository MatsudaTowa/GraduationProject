//=============================================
//
//�G�l�~�[�̃X�e�[�g�p�^�[��[enemy_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ENEMY_STATE_H_
#define _ENEMY_STATE_H_

#include "character.h"
#include "active_scene_character_state.h"

/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CEnemy;

	/** @brief �G�l�~�[�̃��r�[�N���X */
	class CEnemyLobbyState :public CActiveSceneCharacterState
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CEnemyLobbyState();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CEnemyLobbyState() override;
		/**
		* @brief �f���G������
		* @param [in]�L�����N�^�[�|�C���^
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief �G�l�~�[�̃f���G���N���X */
	class CEnemyDuelState :public CActiveSceneCharacterState
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CEnemyDuelState();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CEnemyDuelState() override;
		/**
		* @brief �f���G������
		* @param [in]�L�����N�^�[�|�C���^
		*/
		void Duel(CActiveSceneCharacter* character) override;
	private:
		/**
		 * @brief UI����
		 * @param [in]enemy
		 */
		void CreateDuelUI(CEnemy* enemy);
	};
}
#endif // !_ENEMY_STATE_H_
