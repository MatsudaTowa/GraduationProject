//***************************************************************************************************
//
// imgui(main.cpp)
// Author : Kasai Keisuke
//
//***************************************************************************************************

// include 
#include "main.h"
#include "manager.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

// プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam);
void AdjustWindowStyleForResize(HWND hWnd);

//***************************************************************************************************
// メイン関数
//***************************************************************************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// 新しく追加
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	//SetProcessDPIAware();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WINDCLASSEXのサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0にする(通常は使用しない)
		0,									// 0にする(通常は使用しない)
		hInstance,							// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		// ファイルのアイコン
	};

	MSG  msg;   // メッセージを格納する変数

	// -- クラス登録をワイド版に --
	WNDCLASSEXW wcW;
	ZeroMemory(&wcW, sizeof(wcW));
	wcW.cbSize = sizeof(wcW);
	wcW.style = CS_CLASSDC;
	wcW.lpfnWndProc = WindowProc;
	wcW.hInstance = hInstance;
	wcW.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	wcW.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wcW.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcW.lpszClassName = CLASS_NAME;      // L"windowClass"
	wcW.hIconSm = LoadIconW(NULL, IDI_APPLICATION);
	RegisterClassExW(&wcW);

	// -- ウィンドウ生成をワイド版に --
	RECT rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hWnd = CreateWindowExW(
		0,
		CLASS_NAME,         // L"windowClass"
		WINDOW_NAME,        // L"2Dゲーム基盤"
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left,
		rect.bottom - rect.top,
		NULL, NULL, hInstance, NULL
	);

	// 生成＆初期化
	My::CManager* pManager = new My::CManager();
	
	if (FAILED(pManager->Init(hInstance,hWnd, TRUE)))
	{// 初期化
		return -1;
	}

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// Windowsの処理
			if (msg.message == WM_QUIT)
			{// WM_QUITメッセージを受け取ったらループを抜ける
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{// DirectXの処理
			// マネージャーの更新処理
			pManager->Update();

			// マネージャーの描画処理
			pManager->Draw();
		}
	}

	// マネージャーの終了処理
	pManager->Uninit();

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	return (int)msg.wParam;

}

//***************************************************************************************************
// ウィンドウプロシージャ
//***************************************************************************************************

// マウスを使えるようにする
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM IParam)
{
	// ローカル変数宣言
	int nID;	// 返り値を格納

	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, IParam))
	{
		return true;
	}

	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		// WM_QUITへメッセージを送る
		PostQuitMessage(0);
		break;
	case WM_CLOSE:		// 閉じるボタン押下のメッセージ
		nID = MessageBoxW(hWnd, L"終了しますか？", L"終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{// はいが押された場合
			DestroyWindow(hWnd);	// ウィンドウの破棄
		}
		else
		{// いいえが押された場合
			return 0;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// ESCキーが押されたとき
			nID = MessageBoxW(hWnd, L"終了しますか？", L"終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{// はいが押された場合
				DestroyWindow(hWnd);	// ウィンドウを破棄する
			}
			else
			{
				return 0;
			}
			break;
		}
	case WM_LBUTTONDOWN:	// マウス左クリックのメッセージ

		SetFocus(hWnd);	// ウィンドウにフォーカスを合わせる

		break;

	case WM_CHAR:
	{
		ImWchar c = (ImWchar)wParam;
		wchar_t wc = (wchar_t)c;
		char buf[5] = { 0 };
		WideCharToMultiByte(CP_UTF8, 0, &wc, 1, buf, sizeof(buf), nullptr, nullptr);
		char log[128];
		sprintf_s(log, "WM_CHAR wParam=0x%04X => utf8=\"%s\"\n", (int)c, buf);
		OutputDebugStringA(log);
	}
	break;
	case WM_SIZE:
		if (wParam == SIZE_MAXIMIZED)
		{// ウィンドウが最大化されたとき
			RECT rc;
			GetClientRect(hWnd, &rc);
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize.x = float(rc.right - rc.left);
			io.DisplaySize.y = float(rc.bottom - rc.top);

			//AdjustWindowStyleForResize(hWnd);
		}

		if (ImGui::GetCurrentContext())
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	}


	return DefWindowProcW(hWnd, uMsg, wParam, IParam);	// 既定の処理を返す
}

//***************************************************************************************************
// 画面サイズ調整
//***************************************************************************************************
void AdjustWindowStyleForResize(HWND hWnd)
{
	LONG style = GetWindowLong(hWnd, GWL_STYLE);

	// サイズ変更なスタイルを追加
	style |= WS_THICKFRAME | WS_SIZEBOX;

	SetWindowLong(hWnd, GWL_STYLE, style);

	// スタイル変更を反映
	SetWindowPos(hWnd, nullptr, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}


