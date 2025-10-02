//=============================================
//
//リザルトカメラ[result_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _RESULT_CAMERA_H_ //これが定義されてないとき

#define _RESULT_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief ゲームカメラクラス
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

		//カメラのデバック表示
		void DebugCameraDraw() override;

	private:
	};
}

#endif
