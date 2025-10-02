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
		/**
		* @brief ���_�A�����_�Z�o����
		* @param [in]�v���C���[�̃|�C���^
		* @param [in][out]�����_
		* @param [in][out]���_
		* @param [in][out]����
		*/
		void CalculateCameraView(CPlayer* pPlayer, D3DXVECTOR3& posR, D3DXVECTOR3& posV, D3DXVECTOR3& rot);
		//�o�[�h�r���[����Y�̋���	
		static constexpr float LENGTH_Y = 550.0f;
	};
}
#endif // !_CAMERA_STATE_H
