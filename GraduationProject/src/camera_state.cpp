//=============================================
//
//�J�����̃X�e�[�g�p�^�[��[camera_state.cpp]
//Author Matsuda Towa
//
//=============================================
#include "camera_state.h"
#include "manager.h"

//=============================================
// �ォ��̎��_
//=============================================
void My::CBirdView::BirdView(CCamera* camera)
{
	camera->SetLength(LENGTH_Y);
	camera->SetRot({ 1.1f,0.0f,0.0f });
}