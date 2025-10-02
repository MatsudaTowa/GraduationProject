//=============================================
//
//3DTemplate[main.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MAIN_H_ //これが定義されてないとき

#define _MAIN_H_
#include "renderer.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib") //描画処理に必要
#pragma comment(lib,"d3dx9.lib") //↑の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")
#pragma	comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//頂点情報(2D)の構造体を定義
struct VERTEX_2D
{
	D3DXVECTOR3 pos; //頂点座標
	float rhw; //座標変換用係数（1.0f）
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ
};

//頂点情報(3D)の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ
};

int GetFps();//fps取得
#endif // !_MAIN_H_ //これが定義されてないとき