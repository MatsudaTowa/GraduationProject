//=============================================
//
//�J�E���g�����s����N���X[count.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _COUNT_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _COUNT_H_
#include "main.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �J�E���g�N���X */
	class CCount
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 */		
		CCount() :m_Cnt(0), m_Frame(0) {};

		/**
		 * @brief �f�X�g���N�^
		 */
		~CCount() {};

		/**
		 * @brief �v��
		 * @return �J�E���g
		 */
		int CountMeasure()
		{
			++m_Cnt;
			return m_Cnt;
		}

		/**
		 * @brief �J�E���g�A�b�v
		 * @return �I�����Ă��邩�ǂ���
		 */
		bool CountUp()
		{
			if (m_Cnt >= m_Frame)
			{//�t���[�����ɓ��B���Ă����
				return true;
			}

			//�J�E���g�A�b�v
			++m_Cnt;
			return false;
		}

		/**
		 * @brief �J�E���g�_�E��
		 * @return �I�����Ă��邩�ǂ���
		 */
		bool CountDown()
		{
			if (m_Cnt <= 0)
			{//�t���[�����ɓ��B���Ă����
				return true;
			}
			//�J�E���g�_�E��
			--m_Cnt;
			return false;
		}

		/**
		 * @brief �J�E���g���
		 * @param [in]�������J�E���g��
		 */
		inline void SetCnt(int Cnt) { m_Cnt = Cnt; }

		/**
		 * @brief �t���[�������
		 * @param [in]�������t���[����
		 */
		inline void SetFrame(int Frame) { m_Frame = Frame; }

		/**
		 * @brief ���݂̃J�E���g�擾
		 * @return ���݂̃J�E���g
		 */
		inline int GetCnt() { return m_Cnt; }

		/**
		 * @brief ���݂̃t���[���擾
		 * @return ���݂̃t���[��
		 */
		inline int GetFrame() { return m_Frame; }
	private:
		int m_Frame;	//!<�t���[����
		int m_Cnt;		//!<�J�E���g��
	};
}
#endif