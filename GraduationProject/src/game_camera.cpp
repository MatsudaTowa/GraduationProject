//=============================================
//
//�Q�[���J����[game_camera.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_camera.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CGameCamera::CGameCamera()
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CGameCamera::~CGameCamera()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CGameCamera::Init()
{
	CCamera::Init();
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CGameCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CGameCamera::Update()
{
	CCamera::Update();
}

//=============================================
// �f�o�b�O�\��
//=============================================
void My::CGameCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n�Q�[���̃J����");	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
