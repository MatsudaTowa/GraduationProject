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
		/** @brief ������ */
		virtual void Init() {};

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
		/** @brief �I�� */
		void Uninit()override;

		/** @brief �X�V */
		void Update(CCard* cpy)override;
	};


	/**
	* @brief �I�����
	*/
	class CCardStatePickup :public CCardState
	{
	public:
		/** @brief �I�� */
		void Uninit()override;

		/** @brief �X�V */
		void Update(CCard* cpy)override;
	};

	class CCardStateCast :public CCardState
	{
	public:
		/** @brief �I�� */
		void Uninit()override;

		/** @brief �X�V */
		void Update(CCard* cpy)override;
	};

	class CCardStateStay :public CCardState
	{

	};

	class CCardStatePlay :public CCardState
	{

	};
}

#endif // !CARD_STATE_H_
