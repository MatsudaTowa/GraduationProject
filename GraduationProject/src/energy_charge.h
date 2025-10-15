//============================================================================================================
//
// �G�i�W�[UI�g �uenergy_charge.h�v
// Author : �������D
//
//============================================================================================================

#ifndef _ENERGY_CHARGE_H_
#define _ENERGY_CHARGE_H_
#include "main.h"

namespace My
{
	class CEnergy_Charge
	{
	public:
		/**
		* @brief [ �萔 ]
		* @param �f�t�H���g�̃G�i�W�[�����܂�܂ł̎���
		*/
		 static constexpr float CHARGE_DEF = 180.0f ;

		/**
		* @brief [ �萔 ]
		* @param �f�t�H���g�̃`���[�W���x
		*/
		 static constexpr float SPEED_DEF = 1.0f;

		/**
		* @brief �R���X�g���N�^
		* @param [in]�v���C�I���e�B
		*/
		CEnergy_Charge();

		/**
		* @brief �f�X�g���N�^
		*/
		~CEnergy_Charge();
		
		/**
		* @brief ������
		* @return �����������ǂ���
		*/
		HRESULT Init();
		
		/**
		* @brief �I��
		*/
		void Uninit();
		
		/**
		* @brief �X�V
		*/
		void Update();

		/**
		* @brief �񕜂ɕK�v�Ȏ��Ԃ̕ύX
		* @param [in]�ύX��̎���
		*/
		static void TimeChange(float NewChargeTime);

		/**
		* @brief �񕜎��ԕϓ�
		* @param [in]�ύX��̑��x
		*/
		static void SpeedChange(float NewChargeSpeed);

		/**
		* @brief �[�d��������������
		* @return �������܂��Ă��邩�iint�^�j
		*/
		inline static void SetPolyCnt(float Raito)
		{
			m_fChargeRaito = Raito;
		}
		inline static float GetPolyCnt()
		{
			return m_fChargeRaito;
		}

		/**
		* @brief ���g�̃C���X�^���X�𐶐��擾
		*/
		static CEnergy_Charge* GetInstance()
		{
			static CEnergy_Charge Ins;
			return &Ins;
		}

		/**
		* @brief ����
		*/
		static CEnergy_Charge* Create();

	private:
		/**
		* @brief �[�d����(�n���p)
		*/
		static float m_fChargeRaito;

		/**
		* @brief �o�߂�������
		*/
		float m_fTimeSpend;	

		/**
		* @brief �G�i�W�[�̉񕜂ɕK�v�Ȏ���
		*/
		float m_fChargeTime;		
		
		/**
		* @brief �G�i�W�[�̉񕜑��x
		*/
		float m_fChargeSpeed;		
		
		/**
		* @brief ���ʋ��x
		*/
		int m_nEffectLv;

		/**
		* @brief ���ʒ����ǂ����i�K�v���ԁj
		*/
		bool m_bEffect_T;			
		
		/**
		* @brief ���ʒ����ǂ����i���x�j
		*/
		bool m_bEffect_S;	

	};
}

#endif