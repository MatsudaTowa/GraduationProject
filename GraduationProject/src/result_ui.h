//=============================================
//
//���U���g[result_ui.h]
//Author Kasai Keisuke
//
//=============================================
#ifndef _RESULT_UI_H_	// ���ꂪ��`����Ă��Ȃ��Ƃ�

#define _RESULT_UI_H_
#include "main.h"
#include "object2D.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief ���U���gUI�N���X 
	 * @copydoc CObject2D
	 */
	class CResult_Ui : public CObject2D
	{
	public:
		static const int RESULT_SCREEN_PRIORITY = 21;	//!<�`�揇

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CResult_Ui(int nPriority = RESULT_SCREEN_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CResult_Ui()override;

		/**
		 * @brief ����������
		 * @return ����������
		 */
		HRESULT Init()override;

		/**
		 * @brief �I������ 
		 */
		void Uninit()override;

		/**
		 * @brief �X�V����
		 */
		void Update()override;

		/**
		 * @brief �`�揈�� 
		 */
		void Draw()override;

		/**
		 * @brief ��������
		 * @param [in] �ʒu
		 * @return UI�̃|�C���^
		 */
		static CResult_Ui* Create(D3DXVECTOR3 pos);
	};
}

#endif