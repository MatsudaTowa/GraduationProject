//***************************************************************************************************
//
// imgui(main.h)
// Author : Kasai Keisuke
//
//***************************************************************************************************

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#ifndef _MAIN_H_
#define _MAIN_H_

// include
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"						// 入力処理に必要
#include "Xinput.h"						// ジョイパッド処理に必要

// ライブラリのリンク
#pragma comment(lib,"d3d9.lib")											// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")										// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")										// DirectXコンポーネント
#pragma comment(lib,"dinput8.lib")										// 入力処理に必要
#pragma comment(lib,"xinput.lib")										// ジョイパッド処理に必要
// マクロ定義
//#define CLASS_NAME	"windowClass"										// ウィンドウクラスの名前
//#define WINDOW_NAME	"2Dゲーム基盤"										// ウィンドウの名前(キャプションに表示)

#define CLASS_NAME	L"windowClass"										// ウィンドウクラスの名前
#define WINDOW_NAME	L"ImGuiEditer"										// ウィンドウの名前(キャプションに表示)

#define SCREEN_WIDTH	(1920)											// ウィンドウの幅
#define SCREEN_HEIGHT	(1080)											// ウィンドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 2Dの頂点フォーマット(頂点座標 | 頂点カラー)

// 頂点情報[2D]の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	float rhw;			// 座標変換係数
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_2D;

#endif