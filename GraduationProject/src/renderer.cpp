//=============================================
//
//レンダラーの処理[renderer.cpp]
//Author Matsuda Towa
//
//=============================================
#include "renderer.h"
#include "object.h"
#include "manager.h"

LPDIRECT3DDEVICE9 My::CRenderer::m_pD3DDevice = nullptr;

/**
 * @brief コンストラクタ
 */
My::CRenderer::CRenderer():
m_pD3D(nullptr),
m_pFont(nullptr),
m_pVtxBuffMain(nullptr)
{
}

/**
 * @brief デストラクタ
 */
My::CRenderer::~CRenderer()
{
}

/**
 * @brief 初期化
 * @param [in]ハンドルワンド
 * @param [in]ウィンドウタイプ
 */
HRESULT My::CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm; //ディスプレイ
	D3DPRESENT_PARAMETERS d3dpp; //プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	//現在のディスプレイを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}
	//デバイスのプレゼンテーションの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //ゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH; //ゲーム画面サイズ（横幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; //ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format; //形式
	d3dpp.BackBufferCount = 1; //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //ダブルバッファの切り替え
	d3dpp.EnableAutoDepthStencil = TRUE; //デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; //デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow; //ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //インターバル

	//Direct3Dデバイスの生成（描画処理と頂点処理をハードウェアで行う）
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		//Direct3Dデバイスの生成（描画処理はハードウェア、頂点処理はCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			//Direct3Dのデバイスの生成（両方ともCPU）
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンタステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f,1.0f,1.0f,0.0f));
	
	m_fFogStartPos = 100.0f;
	m_fFogEndPos = 800.0f;
	m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStartPos));
	m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEndPos));
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);


	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"HG創英角ｺﾞｼｯｸUB", &m_pFont);
	return S_OK;
}

/**
 * @brief 終了処理
 */
void My::CRenderer::Uninit()
{
	//デバック表示の破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}

	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

/**
 * @brief 更新処理
 */
void My::CRenderer::Update()
{
}

/**
 * @brief 描画処理
 */
void My::CRenderer::Draw()
{
	//画面クリア（バックバッファ＆Zバッファのクリア)
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(255, 255, 0, 0), 1.0f, 0);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
		DrawFPS(GetFps());
		CObject::DrawAll();
		GET_INPUT_MOUSE->Debug();

		GET_CAMERA(GET_CAMERA_IDX)->DebugCameraDraw();
		m_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

/**
 * @brief FPS表示
 * @param [in]FPSカウント
 */
void My::CRenderer::DrawFPS(int nCntFPS)
{
#ifdef _DEBUG

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];
	//EditInfo* pEdit = GetEditinfo();
	//文字列に代入
	wsprintf
	(&aStr[0],
		"FPS:%d"
		, nCntFPS
	);

	//テキストの描画
	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG
}

/**
 * @brief デバイス取得
 * @return デバイスポインタ
 */
LPDIRECT3DDEVICE9 My::CRenderer::GetDevice()
{
	return m_pD3DDevice;
}

/**
 * @brief フォント取得
 * @return フォントポインタ
 */
LPD3DXFONT My::CRenderer::GetFont()
{
	return m_pFont;
}
