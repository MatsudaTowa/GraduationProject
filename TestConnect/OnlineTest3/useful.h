//=============================================
//
//実用的な処理をまとめたもの[useful.h]
//Author Matsuda Towa
//
//============================================= 

//２重インクルード防止
#ifndef _USEFUL_H_ //これが定義されてないとき
#define _USEFUL_H_

#include "main.h"

//0初期化用
#define INT_ZERO (0)
#define FLOAT_ZERO (0.0f)
#define INT_ONE (1)
#define FLOAT_ONE (1.0f)

//一秒
#define ONE_SECOND (60)

//半分
#define HALF	(0.5f)

//カラー初期化
#define COLOR_NONE (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))
#define COLOR_BLACK (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))
#define COLOR_WHITE (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define COLOR_RED (D3DXCOLOR(1.0f,0.0f,0.0f,1.0f))
#define COLOR_GREEN (D3DXCOLOR(0.0f,1.0f,0.0f,1.0f))
#define COLOR_BLUE (D3DXCOLOR(0.0f,0.0f,1.0f,1.0f))
#define COLOR_YELLOW (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))
#define COLOR_ORANGE (D3DXCOLOR(1.0f,0.5f,0.0f,1.0f))
#define COLOR_PURPLE (D3DXCOLOR(1.0f,0.0f,1.0f,1.0f))
#define COLOR_BLOWN (D3DXCOLOR(0.7f,0.3f,0.0f,1.0f))

//頂点3D初期化
#define VEC3_RESET_ZERO (D3DXVECTOR3(0.0f,0.0f,0.0f))
#define VEC3_RESET_ONE (D3DXVECTOR3(1.0f,1.0f,1.0f))

//頂点2D初期化
#define VEC2_RESET_ZERO (D3DXVECTOR2(0.0f,0.0f))
#define VEC2_RESET_ONE (D3DXVECTOR2(1.0f,1.0f))

//半円
#define HALF_PI	(D3DX_PI * HALF)
//1/4円
#define QRTR_PI	(D3DX_PI * (HALF * HALF))

//文字数最大数
#define MAX_TXT (256)

//放射状にランダム
#define CICRLE_RAND  ((float)(rand() % 629 - 314) / 1e2f)

// マネージャー関係
#define GET_MANAGER				(My::CManager::GetInstance())								// マネージャーインスタンス取得
#define GET_HWND				(GET_MANAGER->GetHwnd())									// ハンドルワンド取得
#define GET_CAMERA(idx)			(GET_MANAGER->GetCamera(idx))								// カメラ取得
#define ERASE_CAMERA(idx)		(GET_MANAGER->EraseCamera(idx))								// 指定されたカメラ削除
#define CREATE_CAMERA(camera)	(GET_MANAGER->CreateCamera(camera))							// カメラ生成
#define GET_CAMER_SIZE			(GET_MANAGER->GetCameraSize())								// カメラサイズ
#define CLEAR_CAMERA			(GET_MANAGER->ClearCamera())								// カメラ全部削除
#define GET_CAMERA_IDX			(GET_MANAGER->GetCureentViewCamera())						// 映しているカメラの番号取得
#define SET_CAMERA_IDX(idx)		(GET_MANAGER->SetCureentViewCamera(idx))					// 映しているカメラの番号設定
#define GET_LIGHT				(GET_MANAGER->GetLight())									// ライト取得
#define GET_MODEL				(GET_MANAGER->GetModel())									// モデル取得
#define GET_TEXTURE				(GET_MANAGER->GetTexture())									// テクスチャ取得
#define GET_FADE				(GET_MANAGER->GetFade())									// フェード取得
#define GET_INPUT_KEYBOARD		(GET_MANAGER->GetKeyboard())								// キーボード情報取得
#define GET_INPUT_MOUSE			(GET_MANAGER->GetMouse())									// マウス情報取得
#define GET_INPUT_PAD			(GET_MANAGER->GetPad())										// パッド情報取得
#define GET_RENDERER			(GET_MANAGER->GetRenderer())								// レンダラー情報取得
#define GET_FONT				(GET_MANAGER->GetFont())									// フォント取得
#define GET_DEVICE				(GET_MANAGER->GetRenderer()->GetDevice())					// デバイス情報取得
#define GET_SOUND				(GET_MANAGER->GetSound())									// サウンド取得
#define GET_COLISION			(GET_MANAGER->GetColision())								// 当たり判定取得
#define GET_FONT_MANAGER		(GET_MANAGER->GetFontManager())								// フォントマネージャー取得

#endif