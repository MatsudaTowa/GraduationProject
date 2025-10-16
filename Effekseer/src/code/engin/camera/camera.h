//============================================
//
// カメラ[camera.h]
// Author:Uedakou
// 
//============================================
#ifndef _CUMERA_H_
#define _CUMERA_H_
//#include "../../system/pch.h"
namespace My
{
#define s_bCameraDataDraw (true)	// カメラ情報を表示するかどうか

	class CText;
	class CCamera
	{
	public:	// 定数

		enum class ProjectionType {
			Perspective = 0,	// 透視投影
			Orthographic,		// 並行投影
			MAX
		};

	public:	// 関数
		CCamera();
		~CCamera();
		HRESULT Init();		//初期化
		void Uninit();		// 終了
		void Update();		// 更新
		void SetCamera();	// 描画

		// 設定
		void SetNomal() { m_posV = {}; m_posR = {}; m_rot = {}; }	// カメラノーマライズ

		void SetPosV(D3DXVECTOR3 pos) { m_posV = pos; CameraSetR(); }	// カメラ位置設定
		void SetPosVX(float x) { m_posV.x = x; CameraSetR(); }	// カメラ位置設定
		void SetPosVY(float y) { m_posV.y = y; CameraSetR(); }	// カメラ位置設定
		void SetPosVZ(float z) { m_posV.z = z; CameraSetR(); }	// カメラ位置設定

		void SetRot(D3DXVECTOR3 rot) { m_rot = rot; CameraSetR(); }// カメラ向き設定
		void SetRotX(float x) { m_rot.x = x; CameraSetR(); }	// カメラ向き設定
		void SetRotY(float y) { m_rot.y = y; CameraSetR(); }	// カメラ向き設定
		void SetRotZ(float z) { m_rot.z = z; CameraSetR(); }	// カメラ向き設定

		// 取得
		D3DXVECTOR3 GetPosV() { return m_posV; CameraSetR(); }	// カメラ位置取得
		D3DXVECTOR3 GetPosR() { return m_posR; CameraSetR(); }	// カメラ注視点位置取得
		D3DXVECTOR3 GetRot() { return m_rot; CameraSetR(); }	// カメラ向き取得

		//　加算
		void AddPosVandRot(D3DXVECTOR3 pos, D3DXVECTOR3 rot) { m_posV += pos; m_rot += rot; CameraSetR(); }	// カメラ位置と向き加算

		void AddPosV(D3DXVECTOR3 pos) { m_posV += pos; CameraSetR(); }	// カメラ位置加算
		void AddPosVX(float x) { m_posV.x += x; CameraSetR(); }	// カメラ位置加算
		void AddPosVY(float y) { m_posV.y += y; CameraSetR(); }	// カメラ位置加算
		void AddPosVZ(float z) { m_posV.z += z; CameraSetR(); }	// カメラ位置加算

		void AddRot(D3DXVECTOR3 rot) { m_rot += rot; CameraSetR(); }	// カメラ向き加算
		void AddRotX(float x) { m_rot.x += x; CameraSetR(); }	// カメラ向き加算
		void AddRotY(float y) { m_rot.y += y; CameraSetR(); }	// カメラ向き加算
		void AddRotZ(float z) { m_rot.z += z; CameraSetR(); }	// カメラ向き加算

		bool IsCameraControllreFpllow() { return m_bCumeraController; }// 現在カメラをコントロールするか 

		void DrawCamera();		// カメラ情報描画
	private:	// 関数
		void Controller();	// 操作
		void CameraSetR();	// 視点と向きから注視点を生成

	private:
		// カメラタイプ基底ストラテジー
		class ProjectionStrategy {
		public:
			ProjectionStrategy() {};
			virtual void Projection(D3DXMATRIX* mtx) = 0;	// 投影
		};
		// 透視投影処理
		class ProjectionPerspectiveStrategy : public ProjectionStrategy {
		public:
			ProjectionPerspectiveStrategy();
			virtual void Projection(D3DXMATRIX* mtx);	// 投影
		private:
			float m_fFov;	// 視野角
			float m_fNear;	// 近クリップ面(ニア)
			float m_fFar;	// 遠クリップ面(ファー)
		public:
			static constexpr float PERSPECTIVE_FOV = 45.0f;;		// 視野角
			static constexpr float PERSPECTIVE_NEAR = 50.0f;	// 近クリップ面
			static constexpr float PERSPECTIVE_FAR = 10000.0f;	// 遠クリップ面
		};
		// 並行投影
		class ProjectionOrthographicStrategy : public ProjectionStrategy {
		public:
			ProjectionOrthographicStrategy();
			virtual void Projection(D3DXMATRIX* mtx);	// 投影
		private:

			float m_fNear;	// 近クリップ面(ニア)
			float m_fFar;	// 遠クリップ面(ファー)
		public:

			static constexpr float PERSPECTIVE_NEAR = 50.0f;	// 近クリップ面
			static constexpr float PERSPECTIVE_FAR = 10000.0f;		// 遠クリップ面
		};

	private:	// 変数
		CCamera::ProjectionType m_CameraType;			// カメラが遠近投影か正投影か
		bool m_bCumeraController;	// カメラのコントロールができるか
		float m_fLeng;				// 視点と注視点の距離
		bool m_bRotTarget;			// ターゲットに向けるか

		float m_fMoveSpeed;	// 移動速度
		float m_fMoveFastSpeed;// シフト移動速度

		D3DXVECTOR3 m_posV;			// カメラ位置
		D3DXVECTOR3 m_vecU;			// 上側
		D3DXVECTOR3 m_rot;			// 向き
		D3DXVECTOR3 m_posR;			// 注視点位置

		D3DXMATRIX m_mtxProjection;	//プロジェクション行列
		D3DXMATRIX m_mtxView;		//ビューマトリックス

		D3DXVECTOR3 m_rotTarget;	// 向き(90度毎)

		ProjectionStrategy* m_pCameraStrategy;	// 投影

		static constexpr  float POS_CAMERA = 100.0f;			// 視点と注視点の距離
		static constexpr bool CAMERA_CONTROLLER = false;	// カメラでコントローラーをするか
		static constexpr float MOVE_SPEED = 10.0f;			// 移動速度
		static constexpr float MOVE_SPEED_FAST = 50.0f;		// シフト移動速度
		static constexpr float ROT_ERRER = 0.005f;			// ターゲット向き時の誤差
		static constexpr ProjectionType CAMERA_TYPE = CCamera::ProjectionType::Perspective;// 投影方式

		static const D3DXVECTOR3 CAMERA_V;		// カメラ位置取得
		static const D3DXVECTOR3 CAMERA_ROT;	// カメラ位置取得
		static const D3DXVECTOR3 CAMERA_U;		// 上方向
	};
}
#endif // !_CUMERA_H_