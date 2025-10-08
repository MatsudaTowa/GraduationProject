//=============================================
//
//2D�O�p�`�I�u�W�F�N�g[object2D_triangle.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT2D_TRIANGLE_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _OBJECT2D_TRIANGLE_H_
#include "main.h"
#include "object2D.h"

/** @brief My ���O��� */
namespace My
{

	/** @brief �A�j���[�V����2D�N���X */
	class CObject2D_Triangle : public CObject2D
	{
	public:
		static constexpr int NUM_VERTEX = 3;

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]nPriority
		 */
		CObject2D_Triangle(int nPriority);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CObject2D_Triangle()override;

		/**
		 * @brief ������
		 * @return ����������
		 */
		HRESULT Init()override;

		/**
		 * @brief �X�V 
		 */
		void Update()override;

		/**
		 * @brief �`��
		 */
		void Draw()override;

		/**
		 * @brief ���_����
		 */
		void SetVtx() override;

		/**
		 * @brief triangleVtx�̑��
		 * @param [in]triangleVtx
		 * @param [in]���_�ԍ�
		 */
		inline void SetTriangleVtx(D3DXVECTOR2 triangleVtx,int idx)
		{
			m_triangleVtx[idx] = triangleVtx;
		}

		/**
		 * @brief triangleVtx�̎擾
		 * @param [in]���_�ԍ�
		 * @return triangleVtx
		 */
		inline D3DXVECTOR2 GetTriangleVtx(int idx)
		{
			return m_triangleVtx[idx];
		}
	private:
		D3DXVECTOR2 m_triangleVtx[NUM_VERTEX] = {};
	};
}
#endif