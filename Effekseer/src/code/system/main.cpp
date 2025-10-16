//===========================================
// 
// メイン[main.cpp]
// Auther:UedaKou
// 
//===========================================
#include "main.h"		// メインヘッダ
#include "manager.h"	// 全体管理マネージャー


// グローバル宣言
int g_nCountFPS = 0;// FPSカウント用のグローバル変数

// アプリケーションクラスへのグローバルアクセス用ポインタ
// WindowProc など、非メンバ関数から CApplication にアクセスするために必要
static My::CApplication* g_pApp = nullptr;

//// <summary>
/// アプリケーションのエントリーポイント（メイン関数）
/// Windows APIにおける WinMain 関数
/// </summary>
/// <param name="hInstance">現在のアプリケーションインスタンスのハンドル</param>
/// <param name="hInstancePrev">以前のインスタンス（常に NULL）</param>
/// <param name="lpCmdLine">コマンドライン引数文字列</param>
/// <param name="nCmdShow">ウィンドウの表示状態</param>
/// <returns>アプリケーションの終了コード（0で正常終了）</returns>
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hindtscePrev, _In_ LPSTR LpCmdline, _In_ int nCmdShow)
{
	My::CApplication app;	// アプリケーション本体のインスタンス（ローカル）

	g_pApp = &app;		// グローバルポインタに登録（WindowProc などから参照するため）

	// 初期化処理（ウィンドウ生成、DirectX初期化など）
	if (!app.Init(hInstance, nCmdShow))
	{
		return -1;
	}

	app.Run();	// メインループ（更新・描画処理）

	app.Uninit();	// リソース解放など終了処理

	return 0;	// 終了コード（正常終了）
}
/// <summary>
/// ウィンドウプロシージャ（メッセージ処理関数）
/// Windows OSから送られてくる各種メッセージを処理する
/// </summary>
/// <param name="hWnd">ウィンドウのハンドル</param>
/// <param name="uMsg">メッセージの種類（例：WM_DESTROY、WM_KEYDOWNなど）</param>
/// <param name="wParam">追加の情報（キーコードやマウスボタン情報など）</param>
/// <param name="lParam">追加の情報（位置座標や拡張情報など）</param>
/// <returns>処理結果（OSへ返す値）</returns>
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;
	My::CManager* instance = My::CManager::GetInstance();// マネージャーインスタンス取得


	switch (uMsg)
	{
	case WM_CREATE:  // ウィンドウが作成されたときのメッセージ
		// ウィンドウ生成直後に送られるメッセージ（ここでは何もしない）
		break;

	case WM_DESTROY:
		// ウィンドウ破棄時：アプリケーション終了メッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		// キー入力があった場合 → アプリケーションクラスの処理に委譲
		if (g_pApp)
		{
			g_pApp->OnKeyDown(wParam);
		}
		break;

	case WM_COMMAND: // ボタンなどのコマンドが発生したとき
		// メニューやボタンの操作など（今回は未使用）
		break;

	case WM_CLOSE:
		// ウィンドウ右上の「×」ボタン押下時の処理
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			instance->Uninit();		// 終了処理を実行
			DestroyWindow(hWnd);	// ウィンドウを破棄する
		}
		else
		{
			// ユーザーがキャンセルした場合は終了中断
			return 0;//0を返さないと終了してしまう
		}
		break;

	case WM_LBUTTONDOWN:	// マウス左クリックのメッセージ
		// ウィンドウにフォーカスを合わせる
		SetFocus(hWnd);
		break;
	}

	// 既定のメッセージ処理を呼び出す
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
/// <summary>
/// 現在のFPSを取得する
/// </summary>
/// <returns>FPS値</returns>
int  My::GetFPS()
{
	return g_nCountFPS;
}
/// <summary>
/// コンストラクタ
/// </summary>
	My::CApplication::CApplication()
	: m_hWnd(NULL), m_dwExecLastTime(0), m_dwFPSLostTime(0), m_dwFrameCount(0)
{
	ZeroMemory(&m_wcex, sizeof(WNDCLASSEX));
	ZeroMemory(&m_msg, sizeof(MSG));
}
/// <summary>
/// デストラクタ
/// </summary>
	My::CApplication::~CApplication()
{
}
/// <summary>
/// ウィンドウ生成とDirectX初期化
/// </summary>
/// <param name="hInstance">アプリケーションのインスタンスハンドル</param>
/// <param name="nCmdShow">ウィンドウの表示状態</param>
/// <returns>初期化結果（成功: true / 失敗: false）true<</returns>
bool  My::CApplication::Init(HINSTANCE hInstance, int nCmdShow)
{
	CManager* instance = CManager::GetInstance();// マネージャーインスタンス取得

	// ウィンドウクラスの設定
	m_wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		::WindowProc,							// ウィンドウプロシャーシ
		0,									// ０にする（通常は使用しない）
		0,									// ０にする（通常は使用しない）
		hInstance,							// インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		// タスクバーアイコン
		LoadCursor(NULL, IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// クライアント領域の背景色
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL, IDI_APPLICATION)		// ファイルのアイコン
	};

	// ウィンドウクラスの登録
	RegisterClassEx(&m_wcex);

	// ウィンドウのクライアント領域サイズの指定
	RECT rect = { 0,0,SCREEN_W,SCREEN_H };

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//	ウィンドウを生成
	m_hWnd = CreateWindowEx(0,	// 拡張ウィンドウスタイル
		CLASS_NAME,				// ウィンドウクラスの名前
		WINDOW_NAME,			// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,	// ウィンドウスタイル
		0,//CW_USEDEFAULT,		// ウィンドウの左上のx座標
		0,//CW_USEDEFAULT,		// ウィンドウの左上のy座標
		(rect.right - rect.left),	// ウィンドウの幅
		(rect.bottom - rect.top),	// ウィンドウの高さ
		NULL,
		NULL,
		hInstance,
		NULL);

	// ウィンドウの表示と更新
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	// 初期化処理（DirectXやサブシステムなど）
	if (FAILED(instance->Init(hInstance, m_hWnd, TRUE)))
	{
		return false; // 初期化失敗時は終了
	}

	// タイマー分解能をミリ秒単位に（CPUに負荷はかかるが精度向上）
	timeBeginPeriod(1);

	// 各タイミング変数を初期化
	m_dwExecLastTime = timeGetTime();	// 最後に処理した時刻を取得
	m_dwFPSLostTime = timeGetTime();	// 最後にFPSを計測した時刻を初期化

	return true;
}
/// <summary>
/// 終了処理
/// </summary>
void  My::CApplication::Uninit()
{
	My::CManager::GetInstance()->Uninit();
	UnregisterClass(CLASS_NAME, m_wcex.hInstance);
	_CrtDumpMemoryLeaks();
}
/// <summary>
/// メインry－プ処理
/// </summary>
void  My::CApplication::Run()
{
	My::CManager* instance = CManager::GetInstance();
	//----------------------------------------------
	// メインループ
	//----------------------------------------------
	while (1)
	{
		// メッセージがあれば処理（非ブロッキング）
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE) != 0)
		{

			if (m_msg.message == WM_QUIT)
			{
				break;	// アプリケーション終了要求
			}
			else
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
		// DirectXの処理
		else
		{
			// 時間取得（フレーム制御用）
			DWORD dwCuppentTime = timeGetTime();

			// FPS計測（0.5秒おきに更新）
			if ((dwCuppentTime - m_dwFPSLostTime) >= 500)
			{
				// FPSを計測
				g_nCountFPS = (m_dwFrameCount * 1000) / (dwCuppentTime - m_dwFPSLostTime);
				m_dwFPSLostTime = dwCuppentTime;
				m_dwFrameCount = 0;

			}
			// 一定間隔（例：60FPSなら約16.66ms）で更新・描画処理
			if ((dwCuppentTime - m_dwExecLastTime) >= (1000 / FPS))
			{// 60分の1秒経過

//  遅延補正付きの更新を使用するかどうか
#if IS_DELAY_UPDATE
			// ==============================
			// 遅延補正付きの更新処理ループ
			// ==============================
				while ((dwCuppentTime - m_dwExecLastTime) >= (1000 / FPS)) // 16.66ms 経過していたら
				{
					// 更新処理 (遅れていたら追いつくまで実行)
					instance->Update();

					// 次のフレーム実行時間に進める（16.66ms分進める）
					m_dwExecLastTime += (1000 / FPS);

					// フレーム数カウント（FPS計測用）
					m_dwFrameCount++;
				}
#else
			// 更新処理
				instance->Update();
#endif
				// 描画処理 (1回だけ)
				instance->Draw();
			}

		}
		// グローバルな終了要求フラグが立っていたらループを抜ける
		if (CManager::GetEnd() == true)
		{
			break;
		}
	}
}
/// <summary>
/// キーボード入力（キー押下）に対する処理
/// </summary>
/// <param name="key">押されたキーの仮想キーコード（WPARAM）</param>
void  My::CApplication::OnKeyDown(WPARAM key)
{
	if (key == VK_ESCAPE)
	{
		int result = MessageBox(m_hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (result == IDYES)
		{
			CManager::GetInstance()->Uninit();
			DestroyWindow(m_hWnd);
		}
	}
}