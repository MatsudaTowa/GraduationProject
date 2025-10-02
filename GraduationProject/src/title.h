//=============================================
//
//�^�C�g������[title.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _TITLE_H_
#include "main.h"
#include "scene.h"
#include "player.h"
#include "title_screen.h"

/** @brief My ���O��� */
namespace My
{
	/**
	 * @brief �^�C�g���N���X
	 * @copydoc CScene
	 */
	class CTitle :public CScene
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 */
		CTitle();

		/**
		 * @brief �f�X�g���N�^
		 */
		~CTitle() override;

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

		CPlayer* m_pPlayer;				//!<�v���C���[
	};
}

#endif