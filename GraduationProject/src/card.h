//===========================================================================================================================================================
// 
// �J�[�h���f�� [card.h]
// Author : �~�c�@�u�^
// 
//===========================================================================================================================================================
#ifndef _CARD_H_
#define _CARD_H_
#include "main.h"
#include "billboard.h"

#endif // !CARD_H_

namespace My
{
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
		 * @brief ����
		 * @param [in]�ʒu
		 */
		static CCard* Create();

	private:
		/**
		 * @brief �J�[�h��I�����Ă��邩�ۂ�(true��������I�����Ă���)
		 */
		bool m_IsChoice;

		/**
		 * @brief �`��
		 */

	};
}