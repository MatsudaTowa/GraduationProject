//===========================================================================================================================================================
// 
// �J�[�h�X�e�[�g�̃w�b�_�[ [card_state.h]
// Author : �~�c�@�u�^
// 
//===========================================================================================================================================================
#ifndef _CARD_STATE_H_
#define _CARD_STATE_H_
#include "card_state.h"

class CCard;

namespace My
{
	/** 
	* @brief �J�[�h�̃X�e�[�g�N���X 
	*/
	class CCardState
	{
		CCardState() {};
		virtual ~CCardState() {};

		/** @brief �j���[�g���� */
		virtual void Neutral(CCard* pCard) {};
	};

	//class CCardStateNeutral :public CCardState
	//{
	//	void Neutral(CCard* pCard)override {};
	//};
}

#endif // !CARD_STATE_H_
