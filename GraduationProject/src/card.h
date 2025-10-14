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
#include "card_state.h"

namespace My
{
	//class CCardState;

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
		void ChangeState(CCardState::CARD_STATE state);

		/**
		* @brief �X�e�[�g�擾
		*/
		inline CCardState* GetState() { return m_pState; }

	private:
		/**
		 * @brief �J�[�h��I�����Ă��邩�ۂ�(true��������I�����Ă���)
		 */
		bool m_IsChoice;

		/**
		 * @brief �J�[�h�̏��
		 */
		CCardState* m_pState;

		/**
		 * @brief �J�[�h�̏�ԗ�
		 */
		My::CCardState::CARD_STATE m_StateNum;
	};
};

#endif // !CARD_H_