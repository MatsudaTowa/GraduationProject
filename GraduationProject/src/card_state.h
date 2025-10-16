//===========================================================================================================================================================
// 
// �J�[�h�X�e�[�g�̃w�b�_�[ [card_state.h]
// Author : �~�c�@�u�^
// 
//===========================================================================================================================================================
#ifndef _CARD_STATE_H_
#define _CARD_STATE_H_
#include "card_state.h"

namespace My
{
	// �O���錾
	class CCard;

	/** 
	* @brief �J�[�h�̃X�e�[�g�N���X 
	*/
	class CCardState
	{
	public:
		/**
		* @brief �J�[�h�̃X�e�[�g��
		* @param ��������Ă��Ȃ����
		* @param �J�[�h���I������Ă�����
		* @param �J�[�h���Ăяo�����
		* @param �J�[�h�̔����ҋ@����
		* @param �J�[�h�̌��ʂ𔭓�����
		*/
		typedef enum
		{
			CARD_NEUTRAL = 0,
			CARD_PICKUP,
			CARD_CAST,
			CARD_STAY,
			CARD_TRIGGER,
			CARD_MAX
		}CARD_STATE;

		/** @brief ������ */
		virtual void Init() {};
		virtual void Init(CCard* cpy) {};

		/** @brief �I�� */
		virtual void Uninit();
		
		/** @brief �X�V */
		virtual void Update(CCard* cpy) {};

		/** @brief �ݒ� */
		void Copy(CCard* cpy);

	protected:
		//My::CCard* m_pCardCpy;
	private:
	};


	/**
	* @brief �j���[�g�������
	*/
	class CCardStateNeutral :public CCardState
	{
	public:
		/** @brief �X�V */
		void Update(CCard* cpy)override;
	};


	/**
	* @brief �I�����
	*/
	class CCardStatePickup :public CCardState
	{
	public:
		/** @brief �X�V */
		void Update(CCard* cpy)override;
	};

	class CCardStateCast :public CCardState
	{
	public:
		/** @brief ������ */
		void Init(CCard* cpy) override;

		/** @brief �X�V */
		void Update(CCard* cpy) override;
	};

	class CCardStateStay :public CCardState
	{
	public:
		/** @brief ������ */
		void Init() override;

		/** @brief �X�V */
		void Update(CCard* cpy)override;

	private:
		/** @brief �ҋ@�J�E���g */
		unsigned int m_Staycount;

		/** @brief �ݒ肳�ꂽ�ҋ@���� */
		const unsigned int STAY_TIME = ONE_SECOND * 3;
	};

	class CCardStateTrigger :public CCardState
	{
	public:
		/** @brief ������ */
		void Init(CCard* cpy) override;
	};
}

#endif // !CARD_STATE_H_
