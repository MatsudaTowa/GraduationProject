//============================================
//
// カメラ[camera.cpp]
// Author:Uedakou
// 
//============================================
#include "camera.h"		// カメラクラス
#include "../../system/manager.h"	// マネージャー
#include <strsafe.h>	// StringCbPrintfを使用するにのに必要
#include "../../object/object.h"	// オブジェクト
#include "../../object/2D/text.h"	// テキスト

namespace My
{
	// カメラ定数
	//const CCamera::ProjectionType CCamera::CAMERA_TYPE = CCamera::ProjectionType::Perspective;	// 投影方式
	//const bool CCamera::CAMERA_CONTROLLER = false;	// カメラでコントローラーをするか
	const D3DXVECTOR3 CCamera::CAMERA_V = { 0.0f, 40.0f, -POS_CAMERA };	// カメラ位置取得
	const D3DXVECTOR3 CCamera::CAMERA_ROT = { 1.5f, 0.0f, 0.0f };	// カメラ向き取得
	const D3DXVECTOR3 CCamera::CAMERA_U = { 0.0f, 1.0f, 0.0f };			// 上方向


	/// <summary>
	/// コンストラクタ
	/// </summary>
	CCamera::CCamera()
	{
		m_CameraType = CAMERA_TYPE;					// カメラ遠近投影
		m_bCumeraController = CAMERA_CONTROLLER;	// プレイヤー追従

		m_posV = CAMERA_V;		// 視点
		m_vecU = CAMERA_U;		// 上方向
		m_rot = CAMERA_ROT;		// 向き
		m_fLeng = POS_CAMERA;	// 視点と注視点の距離

		m_posR.x = cos(m_rot.x) + sin(m_rot.y) * m_fLeng;	// 注視点
		m_posR.z = cos(m_rot.x) + cos(m_rot.y) * m_fLeng;	// 注視点
		m_posR.y = sin(m_rot.x) * m_fLeng;	// 注視点


		m_rotTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き(目標)
		m_bRotTarget = false;						// 補正回転中可動か

		m_fMoveSpeed = MOVE_SPEED;			// 通常時カメラ速度
		m_fMoveFastSpeed = MOVE_SPEED_FAST;	// 高速時カメラ速度

		m_pCameraStrategy = nullptr;	// 描画ストラテジー
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CCamera::~CCamera()
	{
		Uninit();
	}
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>初期化成功状況を返す</returns>
	HRESULT CCamera::Init()
	{
		// m_pCameraStrategyがnullpreならストラテジーを生成
		if (!m_pCameraStrategy)
		{
			m_pCameraStrategy = new ProjectionPerspectiveStrategy();	// 描画ストラテジー
		}

		return S_OK;
	}
	/// <summary>
	/// 終了処理
	/// </summary>
	void CCamera::Uninit()
	{
		// m_pCameraStrategyがnullpreでなければストラテジーを解放
		if (m_pCameraStrategy)
		{
			delete m_pCameraStrategy;	// 描画ストラテジー
			m_pCameraStrategy = nullptr;	// 描画ストラテジー
		}

	}
	/// <summary>
	/// 更新処理
	/// </summary>
	void CCamera::Update()
	{
		CManager* pManager = CManager::GetInstance();	// 全体マネージャー取得
		//CInputKeyboard* pKey = pManager->GetInKey();	// キーボード入力取得
#if _DEBUG
		//if (pKey->GetTrigger(DIK_F2))
		//{
		//	m_bCumeraController = m_bCumeraController ? false : true;
		//}
#endif // _Debug
		if (m_bCumeraController)
		{
			// 入力処理
			Controller();

			// 四方補正回転処理
			if (m_bRotTarget == true)
			{
				// 現在の回転角と目標の回転角の差分を計算
				float fRotY = (m_rotTarget.y - m_rot.y);	// 目標との差分

				// 差分が ±π（180度）を超える場合、TAU（360度）補正で最短回転に調整
				if (fRotY >= D3DX_PI)
				{
					fRotY -= TAU; // -360度して逆回転
				}
				else if (fRotY <= -D3DX_PI)
				{
					fRotY += TAU; // +360度して逆回転
				}

				// 差分の10%だけ回転（イージングのような緩やかな補正）
				m_rot.y += fRotY * 0.1f;

				// 回転角を -π ～ π の範囲に収める（オーバーフロー対策）
				if (m_rot.y >= D3DX_PI)
				{
					m_rot.y -= TAU;
				}
				else if (m_rot.y <= -D3DX_PI)
				{
					m_rot.y += TAU;
				}

				// 回転完了の判定（差分が一定以下なら完了とみなす）
				if (m_rot.y - m_rotTarget.y <= ROT_ERRER &&
					m_rot.y - m_rotTarget.y >= ROT_ERRER)
				{
					m_rot.y = m_rotTarget.y;	// 目標角にピタリと合わせる
					m_bRotTarget = false;		// 補正終了
				}
			}
		}
		CameraSetR();

		// デバッグビルド時のみ有効
#if _DEBUG
	// カメラ情報を表示するフラグが有効な場合
#if s_bCameraDataDraw
	// デバッグテキスト描画用オブジェクトを取得
		CText* pDebugText = pManager->GetDebugText();

		char aStr[MAX_TXT];
		sprintf_s(aStr, sizeof(aStr),
			"CameraV Pos:X%f Y%f Z%f\n"
			"CameraR Pos:X%f Y%f Z%f\n"
			"Camera Rot:X%f Y%f Z%f\n",
			m_posV.x, m_posV.y, m_posV.z,
			m_posR.x, m_posR.y, m_posR.z,
			m_rot.x, m_rot.y, m_rot.z);

		// 整形した文字列をデバッグテキストに表示
		pDebugText->PrintText(aStr);
#endif // s_bCameraDataDraw
#endif // _DEBUG
	}
	/// <summary>
	/// 描画処理
	/// </summary>
	void CCamera::SetCamera()
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイスへのポインタ

		// プロジェクションマトリクスの初期化
		D3DXMatrixIdentity(&m_mtxProjection);
#if 0
		// プロジェクションマトリクスを作成
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),	// 視野角（FOV）
			(float)SCREEN_W / (float)SCREEN_H,	// アスペクト比（横 / 縦）
			10,		// 近クリップ面
			10000);	// 遠クリップ面
#else
		m_pCameraStrategy->Projection(&m_mtxProjection);
#endif // 0


		//switch (m_CameraType)
		//{
		//case ProjectionType::Perspective:
		//	// 遠近投影
		//	Projection()
		//	break;
		//case ProjectionType::Orthographic:
		//	// 正投影
		//	break;
		//default:
		//	break;
		//}
#if _DEBUG


#endif // _DEBUG
	// プロジェクションマトリクスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

		// ビューマトリクスの初期化
		D3DXMatrixIdentity(&m_mtxView);
		// ビューマトリクスの作成
		D3DXMatrixLookAtLH(&m_mtxView,
			&m_posV,
			&m_posR,
			&m_vecU);
		// ビューマトリクスの設定
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
	}
	/// <summary>
	/// カメラコントローラー
	/// </summary>
	void CCamera::Controller()
	{
		//// 全体マネージャー取得
		//CManager* pManager = CManager::GetInstance();
		////キーボード入力取得
		//CInputKeyboard* pKey = pManager->GetInKey();

		////----------------------------------------------
		//// 左回転キー（Q）入力
		////----------------------------------------------
		//if (pKey->GetTrigger(DIK_Q))
		//{
		//	// SHIFT 同時押しなら「四方補正回転」
		//	if (pKey->GetRepeat(DIK_LSHIFT))
		//	{
		//		m_bRotTarget = true;	// 四方補正モード有効化

		//		// 現在の角度に基づいて90度単位の補正角を決定
		//		if (m_rot.y <= 0.0f + 0.005f &&
		//			m_rot.y > -(D3DX_PI * 0.5f) + 0.005f)
		//		{
		//			m_rotTarget.y = -(D3DX_PI * 0.5f);
		//		}
		//		else if (m_rot.y <= -(D3DX_PI * 0.5f) + 0.005f &&
		//			m_rot.y > -D3DX_PI + 0.005f)
		//		{
		//			m_rotTarget.y = -D3DX_PI;
		//		}
		//		else if (m_rot.y <= -D3DX_PI + 0.005f ||
		//			m_rot.y > (D3DX_PI * 0.5f) + 0.005f)
		//		{
		//			m_rotTarget.y = D3DX_PI * 0.5f;
		//		}
		//		else if (m_rot.y <= (D3DX_PI * 0.5f) + 0.005f &&
		//			m_rot.y > 0.0f + 0.005f)
		//		{
		//			m_rotTarget.y = 0.0f;
		//		}
		//	}
		//	// 通常の左回転（トリガー）
		//	else
		//	{
		//		m_bRotTarget = false;	// 四方補正モード無効化
		//		m_rot.y -= 0.05f;
		//		if (m_rot.y <= -D3DX_PI)
		//		{
		//			m_rot.y += TAU;
		//		}
		//	}
		//}
		//// 左回転の継続（ホールド）
		//else if (pKey->GetRepeat(DIK_Q) &&
		//	m_bRotTarget == false)
		//{
		//	m_rot.y -= 0.05f;
		//	if (m_rot.y <= -D3DX_PI)
		//	{
		//		m_rot.y += TAU;
		//	}
		//}
		////----------------------------------------------
		//// 右回転キー（E）入力
		////----------------------------------------------
		//else if (pKey->GetTrigger(DIK_E))
		//{
		//	// SHIFT 同時押しなら「四方補正回転」
		//	if (pKey->GetRepeat(DIK_LSHIFT))
		//	{
		//		m_bRotTarget = true;	// 四方補正回転モードON

		//			// 現在の角度に基づいて90度単位の補正角を決定
		//		if (m_rot.y >= 0.0f - 0.005f &&
		//			m_rot.y < (D3DX_PI * 0.5f) - 0.005f)
		//		{
		//			m_rotTarget.y = D3DX_PI * 0.5f;
		//		}
		//		else if (m_rot.y >= D3DX_PI * 0.5f - 0.005f &&
		//			m_rot.y < D3DX_PI - 0.005f)
		//		{
		//			m_rotTarget.y = D3DX_PI;
		//		}
		//		else if (m_rot.y >= D3DX_PI - 0.005f ||
		//			m_rot.y < -(D3DX_PI * 0.5f) - 0.005f)
		//		{
		//			m_rotTarget.y = -D3DX_PI * 0.5f;
		//		}
		//		else if (m_rot.y >= -(D3DX_PI * 0.5f) - 0.005f &&
		//			m_rot.y < 0.0f - 0.005f)
		//		{
		//			m_rotTarget.y = 0.0f;
		//		}
		//	}
		//	// 通常の右回転（トリガー）
		//	else
		//	{
		//		m_bRotTarget = false;
		//		m_rot.y -= 0.05f;
		//		if (m_rot.y <= -D3DX_PI)
		//		{
		//			m_rot.y += TAU;
		//		}
		//	}
		//}
		//// 右回転の継続（ホールド）
		//else if (pKey->GetRepeat(DIK_E) &&
		//	m_bRotTarget == false)
		//{
		//	m_rot.y += 0.05f;
		//	if (m_rot.y >= D3DX_PI)
		//	{
		//		m_rot.y -= TAU;
		//	}
		//}
		////----------------------------------------------
		//// 上回転キー（R）ホールドで上に傾ける
		////----------------------------------------------
		//if (pKey->GetRepeat(DIK_R))
		//{
		//	// 上限制限（真上以上に向かない）
		//	if (m_rot.x < D3DX_PI)
		//	{
		//		m_rot.x -= 0.05f;
		//	}
		//}
		////----------------------------------------------
		//// 下回転キー（F）ホールドで下に傾ける
		////----------------------------------------------
		//else if (pKey->GetRepeat(DIK_F))
		//{
		//	// 下限制限（真下以下に向かない）
		//	if (m_rot.x > 0)
		//	{
		//		m_rot.x += 0.05f;
		//	}
		//}

		//// 移動速度（通常 or 高速）
		//float speed = (pKey->GetRepeat(DIK_LSHIFT)) ? m_fMoveFastSpeed : m_fMoveSpeed;

		//// 移動方向ベクトルの初期化
		//D3DXVECTOR3 moveVec(0, 0, 0);

		//// 方向フラグ
		//bool w = pKey->GetRepeat(DIK_W);
		//bool s = pKey->GetRepeat(DIK_S);
		//bool a = pKey->GetRepeat(DIK_A);
		//bool d = pKey->GetRepeat(DIK_D);

		////----------------------------------------------
		//// 8方向移動処理（視点回転に応じて方向を変換）
		////----------------------------------------------
		//if (w && a)
		//{   // 左前（-45度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y - D3DX_PI * 0.25f) * speed,
		//		0.0f,
		//		cosf(m_rot.y - D3DX_PI * 0.25f) * speed
		//	);
		//}
		//else if (w && d)
		//{   // 右前（+45度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y + D3DX_PI * 0.25f) * speed,
		//		0.0f,
		//		cosf(m_rot.y + D3DX_PI * 0.25f) * speed
		//	);
		//}
		//else if (w)
		//{   // 前
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y) * speed,
		//		0.0f,
		//		cosf(m_rot.y) * speed
		//	);
		//}
		//else if (s && a)
		//{   // 左後（-135度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y - D3DX_PI * 0.75f) * speed,
		//		0.0f,
		//		cosf(m_rot.y - D3DX_PI * 0.75f) * speed
		//	);
		//}
		//else if (s && d)
		//{   // 右後（+135度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y + D3DX_PI * 0.75f) * speed,
		//		0.0f,
		//		cosf(m_rot.y + D3DX_PI * 0.75f) * speed
		//	);
		//}
		//else if (s)
		//{   // 後
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y + D3DX_PI) * speed,
		//		0.0f,
		//		cosf(m_rot.y + D3DX_PI) * speed
		//	);
		//}
		//else if (a)
		//{   // 左（-90度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y - D3DX_PI * 0.5f) * speed,
		//		0.0f,
		//		cosf(m_rot.y - D3DX_PI * 0.5f) * speed
		//	);
		//}
		//else if (d)
		//{   // 右（+90度）
		//	moveVec += D3DXVECTOR3(
		//		sinf(m_rot.y + D3DX_PI * 0.5f) * speed,
		//		0.0f,
		//		cosf(m_rot.y + D3DX_PI * 0.5f) * speed
		//	);
		//}

		//// 移動を適用
		//m_posV += moveVec;

		////----------------------------------------------
		//// 上下移動（Shiftの有無で速度変更）
		////----------------------------------------------
		//float verticalSpeed = (pKey->GetRepeat(DIK_LSHIFT)) ? speed : 10.0f;

		//if (pKey->GetRepeat(DIK_SPACE))
		//{   // 上昇
		//	m_posV.y += verticalSpeed;
		//}
		//else if (pKey->GetRepeat(DIK_LCONTROL))
		//{   // 下降
		//	m_posV.y -= verticalSpeed;
		//}
	}
	/// <summary>
	/// カメラの注視点（m_posR）を、カメラの位置（m_posV）と回転角（m_rot）から算出して設定する。
	/// 回転角はxが縦（上下の傾き）、yが横（左右の回転）方向を表す。
	/// </summary>
	void CCamera::CameraSetR()
	{
		// カメラの向いている方向（回転角）から注視点の相対座標を計算
		// 角度は上方向を基準としたため、X軸に90度（PI/2）を引いている

		// 注視点のX座標
		m_posR.x = m_posV.x
			+ cosf(m_rot.x - (D3DX_PI * 0.5f))  // 上下の傾きによる水平方向成分（X方向）を係数に
			* sinf(m_rot.y)                    // 左右の角度でX成分
			* POS_CAMERA;                      // 視点距離
		// 注視点のY座標
		m_posR.y = m_posV.y
			+ sinf(m_rot.x - (D3DX_PI * 0.5f))  // 上下の傾きで高さ成分
			* POS_CAMERA;

		// 注視点のZ座標
		m_posR.z = m_posV.z
			+ cosf(m_rot.x - (D3DX_PI * 0.5f))  // 上下の傾きによる水平方向成分（Z方向）を係数に
			* cosf(m_rot.y)                    // 左右の角度でZ成分
			* POS_CAMERA;
	}
	/// <summary>
	/// 透視投影コンストラクタ
	/// </summary>
	CCamera::ProjectionPerspectiveStrategy::ProjectionPerspectiveStrategy()
	{
		// デフォルトの視野角（FOV）・近クリップ面・遠クリップ面の初期値を設定
		m_fFov = PERSPECTIVE_FOV;	// 視野角（例：60度など）
		m_fNear = PERSPECTIVE_NEAR;	// 近くの描画範囲（例：1.0f）
		m_fFar = PERSPECTIVE_FAR;	// 遠くの描画範囲（例：1000.0f）
	}
	/// <summary>
	/// 透視投影描画
	/// </summary>
	/// <param name="mtx"></param>
	void CCamera::ProjectionPerspectiveStrategy::Projection(D3DXMATRIX* mtx)
	{
		// プロジェクションマトリクスを作成
		D3DXMatrixPerspectiveFovLH(mtx,
			D3DXToRadian(m_fFov),	// 視野角（FOV）
			(float)SCREEN_W / (float)SCREEN_H,	// アスペクト比（横 / 縦）
			m_fNear,		// 近クリップ面
			m_fFar);	// 遠クリップ面
	}
	/// <summary>
	/// 並行投影コンストラクタ
	/// </summary>
	CCamera::ProjectionOrthographicStrategy::ProjectionOrthographicStrategy()
	{
		m_fNear = PERSPECTIVE_NEAR;
		m_fFar = PERSPECTIVE_FAR;
	}
	/// <summary>
	/// 並行投影描画
	/// </summary>
	/// <param name="mtx"></param>
	void CCamera::ProjectionOrthographicStrategy::Projection(D3DXMATRIX* mtx)
	{
		D3DXMatrixOrthoLH(
			mtx,
			(float)SCREEN_W,
			(float)SCREEN_H,
			m_fNear,
			m_fFar);
	}
}
