//============================================================================================================
//
// �G�i�W�[UI�g �uenergy_frame.h�v
// Author : �������D
//
//============================================================================================================

#ifndef _ENERGY_FRAME_H_
#define _ENERGY_FRAME_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class CEnergy_frame : public CObject2D
	{
	public:
		/**
		* @brief �R���X�g���N�^
		* @param [in]�v���C�I���e�B
		*/
		CEnergy_frame(int nPriority);

		/** 
		* @brief �f�X�g���N�^
		*/
		~CEnergy_frame()override;

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
		* @param [in]�T�C�Y
		* @return �G�i�W�[UI�g�̃|�C���^
		*/
		static CEnergy_frame* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size);
	};
}

#endif // !_ENERGY_FRAME_H_
