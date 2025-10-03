//=============================================
//
//�J�����̃X�e�[�g�p�^�[��[camera_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CAMERA_STATE_H_
#define _CAMERA_STATE_H_

#include "camera.h"
#include "player.h"


/** @brief My ���O��� */
namespace My
{
	//=============================================
	//�O���錾
	//=============================================
	class CCamera;

	/** @brief �J�����X�e�[�g�N���X */
	class CCameraState
	{
	public:
		/**
		* @brief �΂ߎ��_(�e�ł͉������Ȃ�)
		* @param [in]�J�����̃|�C���^
		*/
		virtual void BirdView([[maybe_unused]] CCamera* camera) {};
	};

	/** @brief �O�Ղ�`���ۂ̎��_�N���X */
	class CBirdView : public CCameraState
	{
	public:
		/**
		* @brief �ォ��̎��_
		* @param [in]�J�����̃|�C���^
		*/
		virtual void BirdView(CCamera* camera)override;
	private:
		//�o�[�h�r���[����Y�̋���	
		static constexpr float LENGTH_Y = 580.0f;
	};
}
#endif // !_CAMERA_STATE_H
