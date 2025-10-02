#include "precompile.h"
#include "title_camera.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CTitleCamera::CTitleCamera()
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CTitleCamera::~CTitleCamera()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CTitleCamera::Init()
{
	CCamera::Init();
	ChangeCameraState(new CBirdView);
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CTitleCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CTitleCamera::Update()
{
	CCamera::Update();
}

//=============================================
// �f�o�b�O�\��
//=============================================
void My::CTitleCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n�^�C�g���̃J����");	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
