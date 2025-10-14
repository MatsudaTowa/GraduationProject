//=============================================
//
//�v���C���[[player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PLAYER_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _PLAYER_H_
#include "main.h"
#include "active_scene_character.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �v���C���[�N���X
	 * @copydoc CCharacter
	 */	
	class CPlayer :public CActiveSceneCharacter
	{
	public:
		static constexpr int PLAYER_PRIORITY = 8;		//!<�`�揇

		static const D3DXVECTOR3 PLAYER_SPAWN_POS;	//!<�X�|�[���ʒu
		static const D3DXVECTOR3 PLAYER_SPAWN_ROT;	//!<�X�|�[������

		/** @brief ���[�V�����̎�ޗ�*/
		enum Motion_Type
		{
			MOTION_NEUTRAL = 0,
			MOTION_MOVE,
			MOTION_ATTACK,
			MOTION_MAX,
		};

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CPlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CPlayer() override;

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
		 * @brief ����
		 * @param [in]�v���C���[�|�C���^
		 * @return �v���C���[�|�C���^
		 */
		static CPlayer* Create(CPlayer* player);

		/**
		 * @brief �����̃��Z�b�g
		 */
		void ResetRot();

		/**
		 * @brief ���͏���
		 */
		void Input();
	private:
		static constexpr float SHADOW_SIZE_X = 20.0f;	 //!<�e�̃T�C�Y
		static constexpr float SHADOW_SIZE_Z = 20.0f;	 //!<�e�̃T�C�Y

		/**
		 * @brief �v���C���[�̃f�o�b�O�\��
		 */
		virtual void Debug();
	};
}
#endif