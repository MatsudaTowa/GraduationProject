//=============================================
//
//�r���{�[�h[billboard.h]
//Auther Matsuda Towa
//
//=============================================
#ifndef _BILLBOARD_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _BILLBOARD_H_
#include "main.h"
#include "object3D.h"
#include "object3D_Anim.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �r���{�[�h�N���X */
	class CBillboard : public CObject3D
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CBillboard(int nPriority);

		/**
		 * @brief �f�X�g���N�^
		 */
		~CBillboard()override;

		/**
		 * @brief �`��
		 */
		void Draw()override;
	};

	/** @brief �A�j���[�V�����r���{�[�h�N���X */
	class CBillboard_Anim : public CObject3D_Anim
	{
	public:

		/**
		 * @brief �R���X�g���N�^
		 * @param [in]�v���C�I���e�B
		 */
		CBillboard_Anim(int nPriority):CObject3D_Anim(nPriority) {};

		/**
		 * @brief �f�X�g���N�^
		 */
		~CBillboard_Anim()override {};

		/**
		 * @brief �`��
		 */
		void Draw()override;
	};
}
#endif