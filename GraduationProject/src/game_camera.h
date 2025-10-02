//=============================================
//
//�Q�[���J����[game_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_CAMERA_H_ //���ꂪ��`����ĂȂ��Ƃ�

#define _GAME_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My ���O��� */
namespace My
{
	/** @brief �Q�[���J�����N���X
	 * @copydoc CCamera
	*/
	class CGameCamera : public CCamera
	{
	public:
		CGameCamera();
		~CGameCamera() override;
		HRESULT Init() override;
		void Uninit() override;
		void Update() override;

		//�J�����̃f�o�b�N�\��
		void DebugCameraDraw() override;

	private:
	};
}

#endif
