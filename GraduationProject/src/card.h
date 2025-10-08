//===========================================================================================================================================================
// 
// �J�[�h���f�� [card.h]
// Author : �~�c�@�u�^
// 
//===========================================================================================================================================================
#ifndef _CARD_H_
#define _CARD_H_
#include "main.h"
#include "objectX.h"

namespace My
{
	class CCardState;

	class CCard :public CObjectX
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CCard(int nPriority);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CCard()override;

		/**
		 * @brief ������
		 */
		HRESULT Init()override;

		/**
		 * @brief �I��
		 */
		void Uninit()override;

		/**
		 * @brief �X�V
		 */
		void Update()override;

		/**
		 * @brief �`��
		 */
		void Draw()override;

		/**
		 * @brief �U���ҋ@����
		 */
		void StandbyAttack();

		/**
		 * @brief ����
		 * @param [in]�ʒu
		 */
		static CCard* Create();

		/**
		* @brief ��Ԃ�ύX����
		*/
		void ChangeState(CCardState*state);

	private:
		/**
		 * @brief �J�[�h��I�����Ă��邩�ۂ�(true��������I�����Ă���)
		 */
		bool m_IsChoice;

		/**
		 * @brief �J�[�h�̏��
		 */
		CCardState* m_pState;
	};
};

#endif // !CARD_H_