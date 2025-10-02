//=============================================
//
//ゲームカメラ[game_camera.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game_camera.h"

//=============================================
// コンストラクタ
//=============================================
My::CGameCamera::CGameCamera()
{
}

//=============================================
// デストラクタ
//=============================================
My::CGameCamera::~CGameCamera()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CGameCamera::Init()
{
	CCamera::Init();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CGameCamera::Uninit()
{
	CCamera::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CGameCamera::Update()
{
	CCamera::Update();
}

//=============================================
// デバッグ表示
//=============================================
void My::CGameCamera::DebugCameraDraw()
{
#ifdef _DEBUG
	CCamera::DebugCameraDraw();
	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nゲームのカメラ");	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}
