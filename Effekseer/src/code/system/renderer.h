//===========================================
// 
// レンダラー[renderer.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _RCNDRERE_H_
#define _RCNDRERE_H_
#include "main.h"

#define B_SORT true
namespace My
{
	class CText;
	// レンダラー
	class CRenderer
	{
	public:
		CRenderer();
		~CRenderer();
		HRESULT Init(HWND nWnd, BOOL bWindow);	// 初期化処理
		void Uninit();	// 終了処理
		void Update();	// 更新処理
		void Draw();	// 描画処理
		void DrawFPS(void);	// FPS描画処理
		LPDIRECT3DDEVICE9 GetDevice();	// Direct3Dデバイス取得
	private:
		LPDIRECT3D9 m_pD3D;				// Direct3D
		LPDIRECT3DDEVICE9 m_pD3DDevice;	// Direct3Dデバイス
		D3DXCOLOR m_BGColor;	// 背景色
		static const D3DXCOLOR s_BGColor;	// 背景色
	};
}
#endif // !_RCNDRERE_H_
