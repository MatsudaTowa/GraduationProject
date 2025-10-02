//=============================================
//
//�v���C���[[game_player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_PLAYER_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _GAME_PLAYER_H_
#include "main.h"
#include "player.h"
#include "game_player_state.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �Q�[���v���C���[�N���X
	 * @copydoc CPlayer
	 */
	class CGamePlayer :public CPlayer
	{
	public:
		static const D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<�X�|�[���ʒu
		static const D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<�X�|�[������

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CGamePlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CGamePlayer() override;

		/**
		 * @brief ������
		 * @return ����������
		 */
		HRESULT Init() override;

		/**
		 * @brief �I��
		 */
		void Uninit() override;

		/**
		 * @brief �X�V
		 */
		void Update() override;

		/**
		 * @brief �`��
		 */
		void Draw() override;

		/**
		 * @brief BGM���؂ꂽ���̏���
		 */
		void Play_Sound_BGM_000();

		/**
		 * @brief �X�e�[�g�ύX
		 * @param [in]���̃X�e�[�g
		 */
		void ChangeState(CGamePlayerState* state);
		
		/**
		 * @brief �X�e�[�g�擾
		 * @return �X�e�[�g�|�C���^
		 */
		inline CGamePlayerState* GetState()
		{
			return m_pState;
		}

private:
		static const D3DXVECTOR3 SHADOW_SIZE;	 //!<�e�̃T�C�Y

		/**
		 * @brief ���͏���
		 */
		void Input();

		/**
		 * @brief �v���C���[�̃f�o�b�O�\��
		 */
		void Debug()override;

		CGamePlayerState* m_pState;						//!�v���C���[�̃X�e�[�g
	};
}
#endif