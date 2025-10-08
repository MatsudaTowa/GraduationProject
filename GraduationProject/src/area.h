//=============================================
//
//�G���A[area.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _AREA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _AREA_H_
#include "main.h"
#include "object2D_triangle.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �G���A�N���X
	 * @copydoc CObject2D_Triangle
	 */
	class CArea : public CObject2D_Triangle
	{
	public:
		static constexpr int PRIORITY = 20;  //!<�`�揇

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CArea(int nPriority = PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CArea()override;

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
		 * @param [in]���_�̈ʒu
		 * @return �G���A�̃|�C���^
		 */
		static CArea* Create(D3DXVECTOR2 triangle_vtx[NUM_VERTEX]);

		/**
		 * @brief �I�����Ă��邩�擾
		 * @return 
		 */
		inline bool GetSelect()
		{
			return m_isSelect;
		}

		/**
		 * @brief �I������Ă��邩�ݒ�
		 * @param [in]isSelect
		 */
		inline void SetSelect(bool isSelect)
		{
			m_isSelect = isSelect;
		}
	private:
		bool m_isSelect; //�I������Ă��邩
	};
}

#endif