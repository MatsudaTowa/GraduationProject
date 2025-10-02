//=============================================
//
//リザルトカメラ[result_camera.h]
//Author Matsuda Towa
//
//=============================================
#include "result_camera.h"

//=============================================
// コンストラクタ
//=============================================
My::CResultCamera::CResultCamera()
{
}

//=============================================
// デストラクタ
//=============================================
My::CResultCamera::~CResultCamera()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CResultCamera::Init()
{
	CCamera::Init();
	ChangeCameraState(new CBirdView);
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CResultCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CResultCamera::Update()
{
	CCamera::Update();
}

//=============================================
// カメラのデバッグ
//=============================================
void My::CResultCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nリザルトのカメラ");	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
