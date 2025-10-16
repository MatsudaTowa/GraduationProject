//===========================================
// 
// レンダラー[renderer.cpp]
// Auther:UedaKou
// 
//===========================================
#include "renderer.h"	// レンダラー
#include "main.h"		// メイン
#include "manager.h"	// マネージャー


#include "../object/object.h"		// オブジェクト
#include "../object/2D/text.h"		// テキストクラス

namespace My
{
	const D3DXCOLOR CRenderer::s_BGColor = D3DCOLOR_RGBA(0, 0, 0, 0);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CRenderer::CRenderer()
	{
		m_pD3D = nullptr;
		m_pD3DDevice = nullptr;
		m_BGColor = s_BGColor;
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CRenderer::~CRenderer()
	{
	}
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="hWnd">ウィンドウのハンドル</param>
	/// <param name="bWindow">ウィンドウモード (TRUE: ウィンドウ, FALSE: フルスクリーン)</param>
	/// <returns>初期化結果（成功: S_OK / 失敗: E_FAIL）</returns>
	HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
	{
		D3DDISPLAYMODE d3ddm;			// ディスプレイモード
		D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

		// Direct3Dオブジェクトの生成
		m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (m_pD3D == NULL)
		{
			return E_FAIL; // 失敗時は即終了
		}
		// 現在のディスプレイモード取得
		if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
		{
			return E_FAIL; // 失敗時は即終了
		}

		// プレゼンテーションパラメータ（初期化）
		ZeroMemory(&d3dpp, sizeof(d3dpp));							// パラメータのゼロクリア
		d3dpp.BackBufferWidth = SCREEN_W;							// ゲーム画面サイズ(幅)
		d3dpp.BackBufferHeight = SCREEN_H;							// ゲーム画面サイズ(高さ)
		d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
		d3dpp.BackBufferCount = 1;									// バックバッファの数
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え
		d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファを16bit使う
		d3dpp.Windowed = bWindow;									// ウィンドウモード
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

		// ----------------------------------------
		// Direct3D デバイスの作成（優先順にトライ）
		// ----------------------------------------

		// 1. ハードウェア頂点処理（最も高速）
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			// 2. ソフトウェア頂点処理
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				// 3. リファレンスデバイス（超低速だが動作確認用途）
				if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
					D3DDEVTYPE_REF,
					hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp,
					&m_pD3DDevice)))
				{
					return E_FAIL;	// どれも失敗
				}
			}
		}

		// レンダーステートの設定
		m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		// サンプラーステートの設定
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		// テクスチャステージステートの設定
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

		return S_OK;
	}
	/// <summary>
	/// 終了処理
	/// </summary>
	void CRenderer::Uninit()
	{
		timeEndPeriod(1);	// fpsリセット
		CObject::ReleaseAll();
	}
	/// <summary>
	/// 更新処理
	/// </summary>
	void CRenderer::Update()
	{
		CObject::UpdateAll();

		timeEndPeriod(1);	// fpsリセット
	}
	/// <summary>
	/// 描画処理
	/// </summary>
	void CRenderer::Draw()
	{
		// 全体マネージャーインスタンス
		CManager* instance = CManager::GetInstance();
		// カメラ取得
		CCamera* pCamera = instance->GetCamera();
		// シーンマネージャー
		Scene::CBase* Scene = instance->GetSceneManager();

		//画面クリア(バックバッファ&Zバッファのクリア)
		m_pD3DDevice->Clear(0, nullptr,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			m_BGColor,	// 背景色
			1.0f,
			0);

		// 描画開始（描画コマンドの発行開始）
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{
			// カメラからの距離を元に描画オブジェクトのソート
			CObject::CalculateDistanceToCamera();
			CObject::Sort();

			// カメラのビュー行列／射影行列をデバイスに設定
			if (pCamera != nullptr)
			{
				pCamera->SetCamera();
			}

			// すべてのゲームオブジェクトを描画（静的・動的問わず）
			CObject::DrawAll();

			// 現在アクティブなシーンに応じた描画処理を実行（タイトル／ゲーム中／リザルト等）
			Scene->Draw();

#ifdef _DEBUG
			// デバッグ用にFPS表示を描画
			DrawFPS();
#endif // !_DEBUG
			// 描画コマンドの終了（GPUへ一括送信）
			m_pD3DDevice->EndScene();
		}
		// 画面に描いた内容をフロントバッファへ反映（実際に表示される）
		m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}
	/// <summary>
	/// FPS表示
	/// </summary>
	void CRenderer::DrawFPS()
	{
		// 全体マネージャー
		CManager* pManager = CManager::GetInstance();
		// デバックテキスト取得
		CText* pText = pManager->GetDebugText();

		// デバックテキストが存在したら
		if (pText != nullptr)
		{
			// FPSを取得
			int nCountFPS = My::GetFPS();
			string aStr;	// 表示用の文字列バッファ
			wsprintf(&aStr[0], "FPS:%d\n", nCountFPS);	// テキスト保持
			pText->PrintText(aStr);
		}
	}
	/// <summary>
	/// Direct3Dデバイス取得
	/// </summary>
	/// <returns>現在使用中のDirect3Dデバイス</returns>
	LPDIRECT3DDEVICE9 CRenderer::GetDevice()
	{
		return m_pD3DDevice;
	}
}