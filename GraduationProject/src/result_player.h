//=============================================
//
//�v���C���[[result_player.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_PLAYER_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _RESULT_PLAYER_H_
#include "main.h"
#include "player.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief ���U���g�v���C���[�N���X
	 * @copydoc CPlayer
	 */
	class CResultPlayer :public CPlayer
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CResultPlayer(int nPriority = PLAYER_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CResultPlayer() override;

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
	};
}
#endif