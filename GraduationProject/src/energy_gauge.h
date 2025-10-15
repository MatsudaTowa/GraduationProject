//============================================================================================================
//
// �G�i�W�[UI�g �uenergy_gauge.h�v
// Author : �������D
//
//============================================================================================================

#ifndef _ENERGY_GAUGE_H_
#define _ENERGY_GAUGE_H_
#include "main.h"
#include "object2D.h"

namespace My
{
	class CEnergy_Gauge : public CObject2D
	{
	public:
		/**
		* @brief [�񋓌^] ���
		* @param �ʏ�(1)
		* @param ����(2)
		* @param ����(3)
		* @param ��~(4)
		* @param ����(5)
		*/
		enum class EFFECT
		{
			NONE = 0,
			EFFECT_NORMAL,
			EFFECT_SPDUP,
			EFFECT_SPDDOWN,
			EFFECT_STOP,
			EFFECT_BACKWARD,
			EFFECT_MAX
		};

		/**
		* @brief [�萔]
		* @param �G�t�F�N�g���x���P�̎��̐F�̔Z��
		*/
		static constexpr float EFFECT_LV1 = 0.2f;

		/**
		* @brief �R���X�g���N�^
		* @param [in]�v���C�I���e�B
		*/
		CEnergy_Gauge(int nPriority);

		/**
		* @brief �f�X�g���N�^
		*/
		~CEnergy_Gauge()override;

		/**
		* @brief ������
		* @return �����������ǂ���
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
		* @return �Q�[�W�����̃|���S���̃|�C���^
		*/
		static CEnergy_Gauge* Create(D3DXVECTOR3 pos);

		/**
		* @brief �Q�[�W�Ɏg���|���S�����܂Ƃ߂Đ���
		*/
		static void CreateEnergy();

		/**
		* @brief ��Ԑݒ�
		* @param [in]�ǂ�ȏ��
		* @param [in]���ʋ��x�i�ő�T�j
		*/
		inline static void EffectSet(EFFECT AnyEffect,int Level)
		{
			m_Effect = AnyEffect;
			m_nEffectLevel = Level;
		}

	private:
		/**
		* @brief �J�E���g���擾���鏈��
		* @return ���Ԗڂ̂��
		*/
		inline static int GetPolyCnt()
		{
			return m_nPolyCount;
		}

		/**
		* @brief ���ǂ�ȏ�ԁH
		*/
		static EFFECT m_Effect;

		/**
		* @brief �Q�[�W�̃|���S���̃J�E���^�[
		*/
		static int m_nPolyCount;

		/**
		* @brief �`���[�W���ꂽ�G�i�W�[�������H�i�擾�K�v�j
		*/
		static int m_nEnergyRaito;

		/**
		* @brief ���ʋ��x�i�ő�T�j
		*/
		static int m_nEffectLevel;

		/**
		* @brief �S�̂ŉ��Ԗڂ�
		*/
		int m_nPolyNum;

		/**
		* @brief �`�悵�Ă����Q�[�W�H
		*/
		bool m_bDisplay;


	};
}

#endif