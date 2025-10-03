//=============================================
//
//カメラのステートパターン[camera_state.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _CAMERA_STATE_H_
#define _CAMERA_STATE_H_

#include "camera.h"
#include "player.h"


/** @brief My 名前空間 */
namespace My
{
	//=============================================
	//前方宣言
	//=============================================
	class CCamera;

	/** @brief カメラステートクラス */
	class CCameraState
	{
	public:
		/**
		* @brief 斜め視点(親では何もしない)
		* @param [in]カメラのポインタ
		*/
		virtual void BirdView([[maybe_unused]] CCamera* camera) {};
	};

	/** @brief 軌跡を描く際の視点クラス */
	class CBirdView : public CCameraState
	{
	public:
		/**
		* @brief 上からの視点
		* @param [in]カメラのポインタ
		*/
		virtual void BirdView(CCamera* camera)override;
	private:
		//バードビュー時のYの距離	
		static constexpr float LENGTH_Y = 580.0f;
	};
}
#endif // !_CAMERA_STATE_H
