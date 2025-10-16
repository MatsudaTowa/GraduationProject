//=============================================
//
//�G���A[center_area.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CENTER_AREA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _CENTER_AREA_H_
#include "main.h"
#include "object2D.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �G���A�N���X
	 * @copydoc CObject2D_Triangle
	 */
	class CCenterArea : public CObject2D
	{
	public:
		static constexpr int PRIORITY = 26;  //!<�`�揇

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CCenterArea(int nPriority = PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CCenterArea()override;

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
		 * @return �G���A�̃|�C���^
		 */
		static CCenterArea* Create();
	private:
	};
}

#endif