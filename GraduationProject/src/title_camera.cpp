#include "precompile.h"
#include "title_camera.h"

//=============================================
// コンストラクタ
//=============================================
My::CTitleCamera::CTitleCamera()
{
}

//=============================================
// デストラクタ
//=============================================
My::CTitleCamera::~CTitleCamera()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CTitleCamera::Init()
{
	CCamera::Init();
	ChangeCameraState(new CBirdView);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CTitleCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CTitleCamera::Update()
{
	CCamera::Update();
}

//=============================================
// デバッグ表示
//=============================================
void My::CTitleCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nタイトルのカメラ");	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
