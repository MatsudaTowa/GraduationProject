//=============================================
//
//�^�C�g����UI[title_screen.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_SCREEN_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _TITLE_SCREEN_H_
#include "main.h"
#include "object2D.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �^�C�g����ʃN���X
	 * @copydoc CObject2D
	 */
	class CTitle_Screen : public CObject2D
	{
	public:
		static const int TITLE_SCREEN_PRIORITY = 10;  //!<�`�揇

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CTitle_Screen(int nPriority = TITLE_SCREEN_PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CTitle_Screen()override;

		/**
		 * @brief ������
		 * @return ����������
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
		 * @return �X�N���[���̃|�C���^
		 */
		static CTitle_Screen* Create(D3DXVECTOR3 pos);
	private:
	};
}

#endif