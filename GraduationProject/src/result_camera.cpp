//=============================================
//
//���U���g�J����[result_camera.h]
//Author Matsuda Towa
//
//=============================================
#include "result_camera.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CResultCamera::CResultCamera()
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CResultCamera::~CResultCamera()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CResultCamera::Init()
{
	CCamera::Init();
	ChangeCameraState(new CBirdView);
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CResultCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CResultCamera::Update()
{
	CCamera::Update();
}

//=============================================
// �J�����̃f�o�b�O
//=============================================
void My::CResultCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n���U���g�̃J����");	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
