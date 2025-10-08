//===========================================================================================================================================================
// 
// ��D�̃w�b�_�[ [hand.h]
// Author : souma umeda
// 
//===========================================================================================================================================================
#ifndef _HAND_H_
#define _HAND_H_
#include "main.h"


/** @brief My ���O��� */
namespace My
{
	// �O���錾
	class CCard;


	/**
	* @brief ��D�̃N���X
	*/
	class CHand
	{
	public:
		/**
		* @brief [ �萔 ]
		* @param �J�n���̎�D
		* @param ��D�̏������
		*/
		static unsigned const int START_HANDS = 1;
		static unsigned const int MAX_HANDSCARD = 10;

		/**
		* @brief �R���X�g���N�^
		*/
		CHand();

		/**
		* @brief �f�X�g���N�^
		*/
		~CHand();

		/**
		* @brief ������
		*/
		void Init();

		/**
		* @brief �J�n��
		*/
		void Start();

		/**
		* @brief �X�V
		*/
		void Update();

		/**
		* @brief �I��
		*/
		void Select();

		/**
		* @brief ��D������
		* @param [in]�h���[��
		*/
		void HandDraw(int drawnum);

		/**
		* @brief ����
		*/
		static CHand* Create();

	private:
		/**
		* @brief ��D�̐^��
		*/
		D3DXVECTOR3 m_CenterPos;

		/**
		* @brief ��D�̃J�[�h�̈ʒu�ݒ�
		*/
		void SetHandCardPos();

		//===========================================================================================================
		// �����o�ϐ�
		//===========================================================================================================

		/*
		* @brief ��D�̔z��
		* TODO : ��������z��ŊǗ��B�̂��Ƀ��X�g�\���Ȃǂ̍\���ŊǗ��\��
		*/ 
		My::CCard* m_pCard[MAX_HANDSCARD];

		/* @brief ���ݎ����Ă���J�[�h���� */
		int m_TotalNum;

		/* @brief �I��ԍ�(��D�������猩������) */
		int m_SelectNum;

		/* @brief �X�^�[�g��ʂ������ǂ��� */
		bool m_IsPassStart;
	};
}

#endif // !HAND_H_