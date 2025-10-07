//=============================================
//
//�G�i�W�[UI�����uenergy_UI.h�v
// Author���c�i�v
//
//=============================================
#ifndef _ENERGY_UI_H_ //���ꂪ��`����ĂȂ��Ƃ�
#define _ENERGY_UI_H_
#include "main.h"
#include "object2D.h"
#include "number.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �G�i�W�[UI�N���X */
	class CEnergy_UI
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CEnergy_UI();
		/**
		 * @brief �f�X�g���N�^
		 */
		~CEnergy_UI();
		/**
		 * @brief ������
		 * @return ����������
		 */
		HRESULT Init();
		/**
		 * @brief �I��
		 */
		void Uninit();

		/**
		 * @brief ����
		 * @param [in]�ʒu
		 * @return �G�i�W�[UI�|�C���^
		 */
		static CEnergy_UI* Create(D3DXVECTOR3 pos);

		/**
		 * @brief �����̐ݒ�
		 * @param [in]���C�t��
		 */
		void SetEnergyNumber(int energy);

		/**
		 * @brief �����̍��W�ݒ�
		 * @param [in]pos
		 */
		inline void SetNumPos(D3DXVECTOR3 pos)
		{
			m_num_pos = pos;
		}

	private:
		/**
		 * @brief ���ǉ�
		 * @param [in]U���W
		 * @param [in]����
		 * @param [in]�G�i�W�[
		 */
		void AddDigit(std::vector<int>& pos_texU, int& num_digit, int energy);

		/**
		 * @brief ���폜
		 * @param [in]�e�N�X�`��U���W
		 * @param [in]����
		 */
		void EraseDigit(std::vector<int> pos_texU, int& num_digit);

		D3DXVECTOR3 m_num_pos;					//�����̈ʒu
		std::vector<CNumber_2D*> m_pNumber;		//����
	};
}

#endif // DEBUG