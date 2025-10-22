//=============================================
//
//カメラのステートパターン[camera_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "camera_state.h"
#include "manager.h"

//=============================================
// 上からの視点
//=============================================
void My::CBirdView::BirdView(CCamera* camera)
{
	camera->SetLength(LENGTH_Y);
	camera->SetRot({ 1.1f,0.0f,0.0f });
}

//=============================================
// 横からの視点
//=============================================
void My::CSideView::SideView(CCamera* camera)
{
	camera->SetPosR({ 0.0f, 50.0f,0.0f });
	camera->SetLength(LENGTH);
	camera->SetRot({ 0.0f,D3DX_PI,0.0f });
}
