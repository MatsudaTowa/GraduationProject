//=============================================
//
//���U���g[result.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _RESULT_H_
#include "main.h"
#include "scene.h"
#include "manager.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief ���U���g�N���X
	 * @copydoc CScene
	 */
	class CResult :public CScene
	{
	public:
		/**
		 * @brief �R���X�g���N�^
		 */
		CResult();

		/**
		 * @brief �f�X�g���N�^
		 */
		~CResult() override;

		/**
		 * @brief ������
		 * @return ����������
		 */
		HRESULT Init() override;

		/**
		 * @brief �I��
		 */
		void Uninit() override;

		/**
		 * @brief �X�V
		 */
		void Update() override;

		/**
		 * @brief �`��
		 */
		void Draw() override;
	private:
	};
}

#endif