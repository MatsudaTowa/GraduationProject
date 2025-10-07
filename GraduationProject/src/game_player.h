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
#include "hand.h"


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
		static constexpr D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<�X�|�[���ʒu
		static constexpr D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<�X�|�[������

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
		static constexpr int ENERGY_UP_FRAME = 3 * 60;		//!<�G�i�W�[���オ��܂ł̃t���[���� TODO:����̃J�[�h�ɂ���Ă͕ϐ��ɏ��i���邩��

		CCount* m_pEnergyUpCount;	//!�G�i�W�[�𑝂₷�܂ł̃J�E���g

		/**
		 * @brief �G�i�W�[���グ�鏈��
		 */
		void EnergyUp();

		/**
		 * @brief ���͏���
		 */
		void Input();

		/**
		 * @brief �v���C���[�̃f�o�b�O�\��
		 */
		void Debug()override;

		CGamePlayerState* m_pState;						//!�v���C���[�̃X�e�[�g
		CHand* m_pHand;	// ��D
	};
}
#endif