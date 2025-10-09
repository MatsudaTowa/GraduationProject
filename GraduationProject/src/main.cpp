//=============================================
//
//3DTemplate[main.cpp]
//Author Matsuda Towa
//
//=============================================
#pragma comment(lib,"winmm.lib")
#include "main.h"
#include "manager.h"

//=============================================
//マクロ定義
//=============================================
#define CLASS_NAME "WindowClass" //文字列
#define WINDOW_NAME "流墨拳" //キャプションに表示

//=============================================
//プロトタイプ宣言
//=============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ToggleFullscreen(HWND hWnd);// ウィンドウをフルスクリーンにする方法

//=============================================
//グローバル変数
//=============================================
My::CManager* g_pManager = nullptr;
int g_nCntFPS = 0; //FPSカウンタ
bool g_isFullscreen = false;// ウィンドウを切り替えるためのフラグ
RECT g_windowRect;// ウィンドウを切り替えるための変数

//=============================================
//メイン関数
//=============================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	//_CrtSetBreakAlloc(246); // 必ずプログラムの最初に実行する
#ifdef _DEBUG
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	DWORD dwCurrentTime; //現在時刻
	DWORD dwExecLastTime; //最後に処理したときの時刻
	DWORD dwFrameCnt; //フレームカウント
	DWORD dwFPSLastTime; //最後にカウントしたFPS

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX), //メモリサイズ
		CS_CLASSDC, //ウィンドウのスタイル
		WindowProc, //ウィンドウプロシージャ
		0, //通常は使用しない
		0, //通常は使用しない
		hInstance, //インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION), //タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW), //マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1), //背景色
		NULL, //メニューバー
		CLASS_NAME, //ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION), //ファイルアイコン
	};
	HWND hWnd; //ウィンドウハンドル
	MSG msg; //メッセージを格納する
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウィンドウを生成
	hWnd = CreateWindowEx(0,	//拡張ウィンドウスタイル
		CLASS_NAME, //クラスの名前
		WINDOW_NAME, //ウィンドウの名前
		WS_OVERLAPPEDWINDOW, //ウィンドウスタイル
		CW_USEDEFAULT, //ウィンドウの左上のx座標
		CW_USEDEFAULT,//ウィンドウの左上のy座標
		SCREEN_WIDTH, //ウィンドウの幅
		SCREEN_HEIGHT, //ウィンドウの高さ
		NULL, //親ウィンドウのハンドル)
		NULL, //メニューハンドル
		hInstance, //インスタンスハンドル
		NULL); //ウィンドウ作成データ

	//生成＆初期化
	My::CManager* pManager = My::CManager::GetInstance();

	//初期化処理
	if (FAILED(pManager->Init(hInstance,hWnd, TRUE)))
	{//初期化失敗したとき
		return -1;
	}

	timeBeginPeriod(1); //分解能を設定
	dwCurrentTime = 0; //初期化
	dwFrameCnt = 0;
	dwExecLastTime = timeGetTime(); //現在時刻
	dwFPSLastTime = timeGetTime(); //現在時刻
	//ウィンドウの表示
	ShowWindow(hWnd, nCmdShow); //ウィンドウの表示を設定
	UpdateWindow(hWnd); //クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//ウィンドウの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg); //仮想キーを文字へ変換
				DispatchMessage(&msg); //ウィンドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCntFPS = (dwFrameCnt * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime; //FPSを測定した時刻を保存
				dwFrameCnt = 0; //フレームカウント
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime; //最終時刻の設定
				pManager->Update();
				pManager->Draw();
				dwFrameCnt++;
			}
		}
	}
	pManager->Uninit();
#ifdef _DEBUG
	//_CrtDumpMemoryLeaks(); // この一行を追加する
	//ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	_CrtDumpMemoryLeaks();
#endif
	return(int)msg.wParam;
}

//=============================================
//ウィンドウプロシージャ
//=============================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;


	switch (uMsg)
	{
	case WM_CLOSE:

		nID = MessageBox(hWnd, "終了しますか？", "確認", MB_YESNO | MB_ICONWARNING);
		if (nID == IDYES)
		{
			//ウィンドウを破棄する
			DestroyWindow(hWnd);
		}
		else
		{

			return 0; //0を返さないと終了してしまう
		}
		break;

	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //キーが押された時のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE: //escキーが押された
			nID = MessageBox(hWnd, "終了しますか？", "確認", MB_YESNO | MB_ICONWARNING);
			if (nID == IDYES)
			{
				//ウィンドウを破棄する
				DestroyWindow(hWnd);
			}
			break;
		case VK_F11:
			ToggleFullscreen(hWnd);
			break;

		}
		break;
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================
//ウィンドウ切り替え処理
//=============================================
void ToggleFullscreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

//=============================================
//FPS取得
//=============================================
int GetFps()
{
	return g_nCntFPS;
}
