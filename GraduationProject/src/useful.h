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
#include "manager.h"
#include "camera.h"

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

//インクリメント関数
template <std::floating_point T>  // float, double など浮動小数点型のみ
void Increment(T& value)
{
    value += 1.0;
}

auto Rundom(auto min, auto max)
{
	return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

/**
* @brief スクリーン座標をワールド座標に変換
* @param [in][out]交点のポインタ
* @param [in]スクリーンX座標
* @param [in]スクリーンY座標
* @param [in]射影空間でのZ値（0～1)
* @param [in]スクリーンの幅
* @param [in]スクリーンの高さ
* @param [in]ビューマトリックス
* @param [in]プロジェクションマトリックス
*/
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pout,
	float Sx,
	float Sy,
	float fZ,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief XZ平面とスクリーン座標の交点算出関数
* @param [in][out]交点のポインタ
* @param [in]ヒットする対象のオブジェクト座標
* @param [in]スクリーンX座標
* @param [in]スクリーンY座標
* @param [in]スクリーンの幅
* @param [in]スクリーンの高さ
* @param [in]ビューマトリックス
* @param [in]プロジェクションマトリックス
*/
D3DXVECTOR3* CalcScreenToXZ(
	D3DXVECTOR3* pout,
	D3DXVECTOR3 hit_obj,
	float Sx,
	float Sy,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief XY平面とスクリーン座標の交点算出関数
* @param [in][out]交点のポインタ
* @param [in]ヒットする対象のオブジェクト座標
* @param [in]スクリーンX座標
* @param [in]スクリーンY座標
* @param [in]スクリーンの幅
* @param [in]スクリーンの高さ
* @param [in]ビューマトリックス
* @param [in]プロジェクションマトリックス
*/
D3DXVECTOR3* CalcScreenToXY(
	D3DXVECTOR3* pout,
	D3DXVECTOR3 hit_obj,
	float Sx,
	float Sy,
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* View,
	D3DXMATRIX* Prj);

/**
* @brief ターゲットに到達したか
* @param [in]到達閾値
* @param [in]スピード
* @param [in]方向
* @param [in]距離
* @param [in]移動量
* @return 到達したか
*/
bool isTargetReached(const float threshold,float speed, float& angle, float& distance, D3DXVECTOR3& move);

/**
 * @brief ワールド座標に変換
 * @param [in]位置
 * @param [in]レイを当てる位置
 * @return 変換後の値
 */
D3DXVECTOR3 ConvertToWorldPoint(My::CCamera* camera, D3DXVECTOR3 pos,D3DXVECTOR3 hit_pos);
/**
 * @brief スクリーン座標に変換
 * @param [in]位置
 * @return 変換後の値
 */
D3DXVECTOR3 ConvertToScreenPos(My::CCamera* camera,D3DXVECTOR3 pos);

#endif