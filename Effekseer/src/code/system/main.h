//===========================================
// 
// メイン[main.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _MAIN_H_	// このマクロ定義されてないなら
#define _MAIN_H_	// 二重インクルード防止のマクロ

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		// 描画に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9,lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポーネント使用に必要
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// Dインプット
#pragma comment(lib,"xinput.lib")	// Xインプット



#define IS_DELAY_UPDATE (false)	// 遅延付き更新をするかどうか初期設定

namespace My
{

	class CApplication
	{
	public:
		CApplication();
		~CApplication();

		bool Init(HINSTANCE hInstance, int nCmdShow); // 初期化処理
		void Uninit();	// 解放処理
		void Run();		// メインループ
		void OnKeyDown(WPARAM key);	// キーボード入力に対する処理

	private:
		HWND m_hWnd;				// アプリケーションのウィンドウハンドル
		WNDCLASSEX m_wcex;			// ウィンドウクラス情報構造体
		MSG m_msg;					// メッセージ構造体
		DWORD m_dwExecLastTime;		// 最後に処理を行った時間（ms）
		DWORD m_dwFPSLostTime;		// 最後にFPSを計測した時間（ms）
		DWORD m_dwFrameCount;		// フレーム数カウント（FPS計測用）
		bool m_siDelayUpdate;		// 遅延付き更新をするかどうか
	};
	int GetFPS();	// FPS取得
}
#endif // _MAIN_H_