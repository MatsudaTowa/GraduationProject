//=============================================
//
//�Q�[���}�l�[�W���[[gamemaneger.h]
//Auther MatsudaTowa
//
//=============================================

#ifndef _GAME_MANAGER_H_	//���ꂪ��`����ĂȂ��Ƃ�
#define _GAME_MANAGER_H_	//��d�}�N���h�~

//�w�b�_�[�̃C���N���[�h
#include "game_player.h"
#include "active_scene_state.h"
#include "field.h"
#include "enemy_manager.h"
#include "area_manager.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �Q�[���}�l�[�W���[�N���X */
	class CGameManager
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CGameManager();

		/**
		 * @brief �f�X�g���N�^
		 */		
		~CGameManager();

		/**
		 * @brief �I��
		 */		
		void Uninit();

		/**
		 * @brief �������g�̃C���X�^���X�𐶐����擾
		 */
		static CGameManager* GetInstance()
		{
			//�ÓI�C���X�^���X
			static CGameManager instance;
			return &instance;
		}

		/**
		 * @brief �G�l�~�[���X�g�擾
		 * @return �G�l�~�[���X�g
		 */
		CEnemyManager* GetEnemyManager() { return m_pEnemyManager; }

		/**
		 * @brief �G���A�}�l�[�W���擾
		 * @return �G���A�}�l�[�W��
		 */
		CAreaManager* GetAreaManager() { return m_pAreaManager; }

		/**
		 * @brief �G�l�~�[���X�g�ݒ�
		 * @param [in]�G�l�~�[���X�g
		 */
		void SetEnemyManager(CEnemyManager* enemy_manager) { m_pEnemyManager = enemy_manager; }
		/**
		 * @brief �G���A�}�l�[�W���ݒ�
		 * @param [in]�G���A�}�l�[�W��
		 */
		void SetAreaManager(CAreaManager* area_manager) { m_pAreaManager = area_manager; }

		/**
		 * @brief �v���C���[�̐ݒ�
		 * @param [in]�v���C���[�̃|�C���^
		 */		
		void SetPlayer(CGamePlayer* player) { m_pPlayer = player; }

		/**
		 * @brief �v���C���[�̎擾
		 * @return �v���C���[�̃|�C���^
		 */		
		CGamePlayer* GetPlayer() { return m_pPlayer; }

		/**
		 * @brief ���̐ݒ�
		 * @param [in]���̃|�C���^
		 */
		void SetField(CField* field) { m_pField = field; }

		/**
		 * @brief ���̎擾
		 * @return ���̃|�C���^
		 */
		CField* GetField() { return m_pField; }

		/**
		 * @brief �Q�[���̃X�e�[�g�̐ݒ�
		 * @param [in]�Q�[���̃X�e�[�g�̃|�C���^
		 */
		void SetState(CActiveSceneState* state) { m_pState = state; }

		/**
		 * @brief �Q�[���̃X�e�[�g�擾
		 * @return �Q�[���̃X�e�[�g�|�C���^
		 */
		CActiveSceneState* GetState() { return m_pState; }

		/**
		 * @brief �X�e�[�g�ύX
		 * @param [in]���̃X�e�[�g
		 */
		void ChangeState(CActiveSceneState* state);

		/**
		 * @brief �X�e�[�g���s����
		 * @param [in]�Q�[���̃V�[���|�C���^
		 */
		void GameStateExecution(CGame* game)
		{
			if (m_pState == nullptr) { return; }

			m_pState->Lobby(game);
			m_pState->Duel(game);
			m_pState->Pause(game);
			m_pState->CardCast(game);
		}

		/**
		 * @brief �I���ݒ�
		 * @param [in]�I�������邩
		 */
		inline void SetFinish(bool is_finish)
		{
			m_isFinish = is_finish;
		}

		/**
		 * @brief �I���擾
		 * @return �I�������邩
		 */
		inline bool GetFinish()
		{
			return m_isFinish;
		}
	private:
		CEnemyManager* m_pEnemyManager;
		CAreaManager* m_pAreaManager;
		CActiveSceneState* m_pState;				//!<�Q�[���̃X�e�[�g���
		CGamePlayer* m_pPlayer;				//!<�v���C���[
		CField* m_pField;					//!<��
		bool m_isFinish;					//!�Q�[���̏������I��点�邩
	};
}

#endif