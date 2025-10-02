//=============================================
//
//�p�[�e�B�N��[particle.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _PARTICLE_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _PARTICLE_H_
#include "main.h"
#include "billboard.h"
#include "count.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �}�E�X�̋O�ՃN���X
	 * @copydoc CBillboard_Anim
	 */
	class CParticle : public CBillboard_Anim
	{
	public:
		static const int PRIORITY = 21;		//!<�`�揇

		/**
		 * @brief �R���X�g���N�^
		 * @param �v���C�I���e�B
		 */
		CParticle(int nPriority = PRIORITY);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CParticle()override;

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
		 * @brief �ړ��ʎ擾
		 * @return �ړ���
		 */
		inline D3DXVECTOR3 GetMove()
		{
			return m_move;
		}

		/**
		 * @brief �̗͎擾
		 * @return �̗�
		 */
		inline int GetLife()
		{
			return m_life;
		}

		/**
		 * @brief �ŏ��̐F�擾
		 * @return �ŏ��̐F
		 */
		inline D3DXCOLOR GetStartColor()
		{
			return m_startcolor;
		}

		/**
		 * @brief �ړ��ʐݒ�
		 * @param [in]�ړ���
		 */
		inline void SetMove(D3DXVECTOR3 move)
		{
			m_move = move;
		}

		/**
		 * @brief �̗͐ݒ�
		 * @param [in]�̗�
		 */
		inline void SetLife(int life)
		{
			m_life = life;
		}

		/**
		 * @brief �ŏ��̗̑͐ݒ�
		 * @param [in]�ŏ��̗̑�
		 */
		inline void SetStartLife(int startlife)
		{
			m_startlife = startlife;
		}

		/**
		 * @brief �ŏ��̐F�ݒ�
		 * @param [in]�ŏ��̐F
		 */
		inline void SetStartColor(D3DXCOLOR color)
		{
			m_startcolor = color;
		}
	private:
		D3DXVECTOR3 m_move;
		int m_life;
		int m_startlife;
		D3DXCOLOR m_startcolor;
	};
}

#endif