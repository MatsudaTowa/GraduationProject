//=============================================
//
//�^�C�g���J����[title_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_CAMERA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _TITLE_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �^�C�g���J�����N���X
	 * @copydoc CCamera
	*/
	class CTitleCamera : public CCamera
	{
	public:
		CTitleCamera();
		~CTitleCamera() override;
		HRESULT Init() override;
		void Uninit() override;
		void Update() override;

		//�J�����̃f�o�b�N�\��
		void DebugCameraDraw() override;

	private:
	};
}

#endif
