//=============================================
//
//プリコンパイル[precompile.h]
//Author Matsuda Towa
//
//=============================================
#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_
#include <windows.h>
#include <string.h>
#include<Mmsystem.h>
#include <assert.h>
#include<stdio.h>
#include <string>
#include <vector>
#include <list>
#include <random>
#include<iostream>
#include <algorithm>
#include <stdlib.h>
#include <crtdbg.h>
#include <fstream>
#include <cmath>

//#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#define DIRECTINPUT_VERSION (0x0800) //警告対処用
#include "dinput.h" //入力処理に必要
#include "d3dx9.h" //描画処理に必要
#include "xaudio2.h"
#include "Xinput.h"
#include "useful.h"

//マクロ定義
#define SCREEN_WIDTH	(1920) //ウィンドウの幅
#define SCREEN_HEIGHT	(1080) //ウィンドウの高さ
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE |D3DFVF_TEX1) //座標とカラーとテクスチャ
#define FVF_VERTEX_3D	(D3DFVF_XYZ |D3DFVF_NORMAL| D3DFVF_DIFFUSE |D3DFVF_TEX1) //座標とカラーとテクスチャ


#endif // PRECOMPILE_H_

