////=============================================
////
////�G�l�~�[�̃X�e�[�g�p�^�[��[enemy_state.h]
////Author Matsuda Towa
////
////=============================================
//#ifndef _ENEMY_STATE_H_
//#define _ENEMY_STATE_H_
//
//#include "character.h"
//#include "active_scene_character_state.h"
//
///** @brief My ���O��� */
//namespace My
//{
//	//=============================================
//	//�O���錾
//	//=============================================
//	class CEnemy;
//
//	/** @brief �G�l�~�[�̃��r�[�N���X */
//	class CLobbyState :public CActiveSceneCharacterState
//	{
//	public:
//		/**
//		 * @brief �R���X�g���N�^
//		 */
//		CLobbyState();
//		/**
//		 * @brief �f�X�g���N�^
//		 */
//		~CLobbyState() override;
//		/**
//		* @brief �f���G������
//		* @param [in]�L�����N�^�[�|�C���^
//		*/
//		void Lobby(CActiveSceneCharacter* character) override;
//	};
//
//	/** @brief �G�l�~�[�̃f���G���N���X */
//	class CDuelState :public CActiveSceneCharacterState
//	{
//	public:
//		/**
//		 * @brief �R���X�g���N�^
//		 */
//		CDuelState();
//		/**
//		 * @brief �f�X�g���N�^
//		 */
//		~CDuelState() override;
//		/**
//		* @brief �f���G������
//		* @param [in]�L�����N�^�[�|�C���^
//		*/
//		void Duel(CActiveSceneCharacter* character) override;
//	private:
//		/**
//		 * @brief UI����
//		 * @param [in]enemy
//		 */
//		void CreateDuelUI(CEnemy* enemy);
//	};
//}
//#endif // !_ENEMY_STATE_H_
