//=============================================
//
//�J����[camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CAMERA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _CAMERA_H_
#include "main.h"
#include "input.h"
#include "camera_state.h"

/** @brief My ���O��� */
namespace My
{
	//=============================================
	// �O���錾
	//=============================================
	class CCameraState;

	/** @brief �J�����N���X */
	class CCamera
	{
	public:
		CCamera();
		virtual ~CCamera();
		virtual HRESULT Init();
		virtual void Uninit();
		virtual void Update();
		void SetCamera();
		void ResetCamera();

		void InputMove(); //�J�����ړ�����
		void InputTurn(); //�J������]����

		void MouseTurn();

		void ChangeCameraState(CCameraState* state);

		//�����擾
		inline void SetRot(D3DXVECTOR3 rot)
		{
			m_rot = rot;
		}
		//���_�擾
		inline void SetPosV(D3DXVECTOR3 posV)
		{
			m_posV = posV;
		}
		//�����_�擾
		inline void SetPosR(D3DXVECTOR3 posR)
		{
			m_posR = posR;
		}

		inline void SetLength(float length)
		{
			m_fLength = length;
		}

		inline void SetProMtx(D3DXMATRIX mtx)
		{
			m_mtxProjection = mtx;
		}

		inline void SetViewMtx(D3DXMATRIX mtx)
		{
			m_mtxView = mtx;
		}

		//�����擾
		inline D3DXVECTOR3 GetRot()
		{
			return m_rot;
		}

		//���_�擾
		inline D3DXVECTOR3 GetPosV()
		{
			return m_posV;
		}
		//�����_�擾
		inline D3DXVECTOR3 GetPosR()
		{
			return m_posR;
		}

		inline float& GetLength()
		{
			return m_fLength;
		}

		inline D3DXMATRIX GetViewMtx()
		{
			return m_mtxView;
		}

		inline D3DXMATRIX GetPrjMtx()
		{
			return m_mtxProjection;
		}

		inline D3DVIEWPORT9 GetViewPort()
		{
			return m_ViewPort;
		}

		inline CCameraState* GetState()
		{
			return m_pCameraState;
		}

		//�J�����̃f�o�b�N�\��
		virtual void DebugCameraDraw();
	private:
		static constexpr float DEFAULT_MOVE = 1.0f; //�ʏ펞�̈ړ�
		static constexpr float DAMPING_COEFFICIENT = 0.2f; //�ړ���R

		static constexpr float BIRDVIEW_LENGTH_Y = 300.0f; //�o�[�h�r���[����Y�̋���

		static constexpr float THIRDVIEW_CORRECT_X = 20.0f; //�T�[�h�p�[�\���r���[���̕␳�lX
		static constexpr float THIRDVIEW_CORRECT_Y = 105.0f; //�T�[�h�p�[�\���r���[���̕␳�lY
		static constexpr float THIRDVIEW_CORRECT_Z = 20.0f; //�T�[�h�p�[�\���r���[���̕␳�lZ

		static constexpr float MOUSE_SENS = 0.001f; //�f�t�H���g�̃}�E�X���x

		static constexpr float TURN_SPEED = 0.02f; //�J�����̉�]�X�s�[�h

		static constexpr float MAX_TURN_X = 0.9f; //�T�[�h�p�[�\���r���[����X�̍ő����
		static constexpr float MIN_TURN_X = -0.15f; //�T�[�h�p�[�\���r���[����X�̍ŏ�����

		CCameraState* m_pCameraState;

		D3DXVECTOR3 m_posV; //���_
		D3DXVECTOR3 m_posR; //�����_

		D3DXVECTOR3 m_moveV; //���_�ړ���
		D3DXVECTOR3 m_moveR; //�����_�ړ���

		D3DXVECTOR3 m_vecU; //������x�N�g��

		D3DXVECTOR3 m_rot; //����
		D3DXVECTOR3 m_rotmove; //��]��

		float m_fLength; //�ΏۂƂ̋���
		float m_fAngle; //�ΏۂƂ̑Ίp���̊p�x

		D3DXMATRIX m_mtxProjection;		//�v���W�F�N�V����
		D3DXMATRIX m_mtxView;			//�r���[�s��
		D3DVIEWPORT9 m_ViewPort;		//�r���[�|�[�g
	};
}

#endif
