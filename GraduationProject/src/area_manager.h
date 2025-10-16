//================================
//
//area_manager.cpp�ɕK�v�Ȑ錾[area_manager.h]
//Author:���c�i�v
// 
//================================

#ifndef _AREA_MANAGER_H_ //���̃}�N����`������Ă��Ȃ�������
#define _AREA_MANAGER_H_ //�Q�d�C���N���[�h�h�~�̃}�N����`

//�w�b�_�[�̃C���N���[�h
#include "main.h"
#include "area.h"
#include "center_area.h"

namespace My
{
	//�G�l�~�[�}�l�[�W���[�̃N���X
	class CAreaManager
	{
	public:

		//�����o�֐�
		CAreaManager();			//�R���X�g���N�^
		~CAreaManager();			//�f�X�g���N�^
		HRESULT Init();				//������
		void Uninit();				//�I��
		/**
		 * @brief �G���A�𐶐�
		 */
		void CreateArea();

		/**
		 * @brief �G���A�̑I��
		 */
		void SelectArea();

		/**
		 * @brief �G���A�̐ݒ�
		 * @param [in]�G���A�̃|�C���^
		 * @param [in]�ǂ��̃G���A��
		 */
		void SetArea(CArea* area, int idx) { m_pArea[idx] = area; }

		/**
		 * @brief ���S�G���A�̐ݒ�
		 * @param ���S�G���A
		 */
		void SetCenterArea(CCenterArea* area) { m_pCenterArea = area; }

		/**
		 * @brief �G���A�̎擾
		 * @param [in]�G���A�ԍ�
		 * @return �G���A�̃|�C���^
		 */
		CArea* GetArea(int idx) { return m_pArea[idx]; }

		/**
		 * @brief ���S�G���A�̎擾
		 * @return ���S�G���A
		 */
		CCenterArea* GetCenterArea() { return m_pCenterArea; }

		/**
		 * @brief �I�����Ă��邩�擾
		 * @return
		 */
		inline bool GetActive()
		{
			return m_isActive;
		}

		/**
		 * @brief �I������Ă��邩�ݒ�
		 * @param [in]isSelect
		 */
		inline void SetActive(bool isActive)
		{
			m_isActive = isActive;
		}

	private:
		CArea* m_pArea[CInputMouse::AREA::MAX - 1]; //�^�񒆂̃G���A�ȊO�ɎO�p�`�𐶐�
		CCenterArea* m_pCenterArea;					//�^�񒆂̃G���A
		bool m_isActive; //���s���邩
	};
}

#endif