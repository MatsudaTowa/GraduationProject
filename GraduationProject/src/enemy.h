//=============================================
//
//�G�l�~�[[enemy.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _ENEMY_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _ENEMY_H_
#include "main.h"
#include "character.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �v���C���[�N���X
	 * @copydoc CCharacter
	 */
	class CEnemy :public CCharacter
	{
	public:
		static constexpr int PRIORITY = 8;		//!<�`�揇

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
		CEnemy(int nPriority = PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CEnemy() override;

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
		 * @param [in]�ʒu
		 * @param [in]����
		 * @return �G�l�~�[�|�C���^
		 */
		static CEnemy* Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
	private:
	};
}
#endif