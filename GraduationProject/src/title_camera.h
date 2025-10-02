//=============================================
//
//タイトルカメラ[title_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TITLE_CAMERA_H_ //これが定義されてないとき

#define _TITLE_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief タイトルカメラクラス
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

		//カメラのデバック表示
		void DebugCameraDraw() override;

	private:
	};
}

#endif
