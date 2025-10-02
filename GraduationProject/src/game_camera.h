//=============================================
//
//ゲームカメラ[game_camera.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _GAME_CAMERA_H_ //これが定義されてないとき

#define _GAME_CAMERA_H_
#include "main.h"
#include "camera.h"
#include "camera_state.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief ゲームカメラクラス
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

		//カメラのデバック表示
		void DebugCameraDraw() override;

	private:
	};
}

#endif
