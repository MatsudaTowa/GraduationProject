//=============================================
//
//���U���g�J����[result_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_CAMERA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _RESULT_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �Q�[���J�����N���X
	 * @copydoc CCamera
	*/
	class CResultCamera : public CCamera
	{
	public:
		CResultCamera();
		~CResultCamera() override;
		HRESULT Init() override;
		void Uninit() override;
		void Update() override;

		//�J�����̃f�o�b�N�\��
		void DebugCameraDraw() override;

	private:
	};
}

#endif
