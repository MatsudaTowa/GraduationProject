//=============================================
//
//�Q�[���v���C���[�̃X�e�[�g�p�^�[��[game_player_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_STATE_H_
#define _GAME_PLAYER_STATE_H_

#include "character.h"
#include "active_scene_character_state.h"

/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CGamePlayer;

	/** @brief �v���C���[�̃f���G���N���X */
	class CLobbyState :public CActiveSceneCharacterState
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CLobbyState();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CLobbyState() override;
		/**
		* @brief �f���G������
		* @param [in]�L�����N�^�[�|�C���^
		*/
		void Lobby(CActiveSceneCharacter* character) override;
	};

	/** @brief �v���C���[�̃f���G���N���X */
	class CDuelState :public CActiveSceneCharacterState
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CDuelState();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CDuelState() override;
		/**
		* @brief �f���G������
		* @param [in]�L�����N�^�[�|�C���^
		*/
		void Duel(CActiveSceneCharacter* character) override;
	private:
		static constexpr int ENERGY_UP_FRAME = 3 * 60;		//!<�G�i�W�[���オ��܂ł̃t���[���� TODO:����̃J�[�h�ɂ���Ă͕ϐ��ɏ��i���邩��

		/**
		 * @brief UI����
		 * @param [in]player
		 */
		void CreateDuelUI(CGamePlayer* player);

		/**
		 * @brief �G�i�W�[���グ�鏈��
		 * @param [in]player
		 */
		void EnergyUp(CGamePlayer* player);

		CCount* m_pEnergyUpCount;	//!�G�i�W�[�𑝂₷�܂ł̃J�E���g
	};
}
#endif // !_GAME_PLAYER_STATE_H_
