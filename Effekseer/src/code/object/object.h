//============================================
//
// オブジェクト[object.h]
// Author:Uedakou
// 
//============================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
//#include "../system/pch.h"	//プリコンパイルヘッダ
#include "../system/calculation.h"	// 計算用

namespace My
{
	// オブジェクト
	class CObject
	{
	public:
		CObject();
		CObject(const int nPriority);	// 優先度付き
		virtual ~CObject();
		virtual HRESULT Init() = 0;	// 初期化
		virtual void Uninit() = 0;	// 終了
		virtual void Update() = 0;	// 更新
		virtual void Draw() = 0;	// 描画
		void Release();				// 自分自身の解放

		// 全体処理
		static void ReleaseScene();	// 全オブジェクト解放
		static void ReleaseAll();	// 全オブジェクト解放
		static void ReleaseDeathFlag();	// 全オブジェクト解放
		static void UpdateAll();	// 全オブジェクト更新
		static void DrawAll();		// 全オブジェクト描画
		static void GetAllObject(CObject* object[MAX_PRIORITY]);	// オブジェクト取得
		static CObject* GetMyObject(int nPriorty) { return m_pTop[nPriorty]; };	// オブジェクト取得(優先度毎)
		static void Sort();

		static void CalculateDistanceToCamera();	// カメラからの距離でソート
		void CalculateDistance(D3DXVECTOR3 pos);	/// カメラからの距離を計算
		// リスト
		virtual void SetNext(CObject* pNext) { m_pNext = pNext; }	// 次設定
		virtual void SetPrev(CObject* pPrev) { m_pPrev = pPrev; }	// 前設定
		virtual CObject* GetNext() { return m_pNext; }//	次取得
		virtual CObject* GetPrev() { return m_pPrev; }//	次取得

		// 位置
		virtual void SetX(X x) { m_x = x; }
		virtual void SetX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 siz) {
			m_x.pos = pos;
			m_x.rot = rot;
			m_x.scl = siz;
		}
		virtual void SetPos(const D3DXVECTOR3 pos) { m_x.pos = pos; }	// 位置設定
		virtual void SetPosX(const float x) { m_x.pos.x = x; }	// 位置設定
		virtual void SetPosY(const float y) { m_x.pos.y = y; }	// 位置設定
		virtual void SetPosZ(const float z) { m_x.pos.z = z; }	// 位置設定
		virtual void SetRot(const D3DXVECTOR3 rot) { m_x.rot = rot; }	// 向き設定
		virtual void SetRotX(const float x) { m_x.rot.x = x; }	// 向き設定
		virtual void SetRotY(const float y) { m_x.rot.y = y; }	// 向き設定
		virtual void SetRotZ(const float z) { m_x.rot.z = z; }	// 向き設定
		virtual void SetScl(const D3DXVECTOR3 siz) { m_x.scl = siz; }	// 大きさ設定
		virtual void SetSclX(const float x) { m_x.scl.x = x; }	// 大きさ設定
		virtual void SetSclY(const float y) { m_x.scl.y = y; }	// 大きさ設定
		virtual void SetSclZ(const float z) { m_x.scl.z = z; }	// 大きさ設定

		// トランスフォーム加算
		virtual void AddX(X x) { m_x += x; }
		virtual void AddX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 siz) {
			m_x.pos += pos;
			m_x.rot += rot;
			m_x.scl += siz;
		}														// トランスフォーム設定
		virtual void AddPos(const D3DXVECTOR3 pos) { m_x.pos += pos; }	// 位置設定
		virtual void AddPosX(const float x) { m_x.pos.x += x; }	// 位置設定
		virtual void AddPosY(const float y) { m_x.pos.y += y; }	// 位置設定
		virtual void AddPosZ(const float z) { m_x.pos.z += z; }	// 位置設定
		virtual void AddRot(const D3DXVECTOR3 rot) { m_x.rot += rot; }	// 向き設定
		virtual void AddRotX(const float x) { m_x.rot.x += x; }	// 向き設定
		virtual void AddRotY(const float y) { m_x.rot.y += y; }	// 向き設定
		virtual void AddRotZ(const float z) { m_x.rot.z += z; }	// 向き設定
		virtual void AddScl(const D3DXVECTOR3 siz) { m_x.scl += siz; }	// 大きさ設定
		virtual void AddSclX(const float x) { m_x.scl.x += x; }	// 大きさ設定
		virtual void AddSclY(const float y) { m_x.scl.y += y; }	// 大きさ設定
		virtual void AddSclZ(const float z) { m_x.scl.z += z; }	// 大きさ設定
		// トランスフォーム取得
		virtual X GetX() { return m_x; }					// トランスフォーム設定
		virtual D3DXVECTOR3 GetPos() { return m_x.pos; }	// 位置設定
		virtual float GetPosX() { return m_x.pos.x; }	// 位置設定
		virtual float GetPosY() { return m_x.pos.y; }	// 位置設定
		virtual float GetPosZ() { return m_x.pos.z; }	// 位置設定
		virtual D3DXVECTOR3 GetRot() { return m_x.rot; }	// 向き設定
		virtual float GetRotX() { return m_x.rot.x; }	// 向き設定
		virtual float GetRotY() { return m_x.rot.y; }	// 向き設定
		virtual float GetRotZ() { return m_x.rot.z; }	// 向き設定
		virtual D3DXVECTOR3 GetScl() { return m_x.scl; }	// 大きさ設定
		virtual float GetSclX() { return m_x.scl.x; }	// 大きさ設定
		virtual float GetSclY() { return m_x.scl.y; }	// 大きさ設定
		virtual float GetSclZ() { return m_x.scl.z; }	// 大きさ設定

		// 処理
		virtual void SetDistance(float fDistance) { m_fDistance = fDistance; }	// 見目らからの距離
		virtual float GetDistance() { return m_fDistance; }	// 見目らからの距離

		// フラグ
		virtual void SetNormalUpdate(bool bUpdate) { m_bAllUpdate = bUpdate; }		// 全体で更新するか設定
		virtual void SetPoseUpdate(bool bUpdate) { m_bPoseUpdate = bUpdate; }		// ポーズ中更新するか設定
		virtual void SetNormalDraw(bool bDraw) { m_bAllDraw = bDraw; }				// 全体で描画するか設定
		virtual void SetPoseDraw(bool bDraw) { m_bPoseDraw = bDraw; }				// ポーズ中描画するか設定
		virtual void SetReleaseScene(bool bRelease) { m_bReleaseScene = bRelease; }	// シーンでリリースするか

		virtual bool IsAllUpdate() { return m_bAllUpdate; }			// 全体で更新するか
		virtual bool IsPoseUpdate() { return m_bPoseUpdate; }		// ポーズ中更新するか
		virtual bool IsAllDraw() { return m_bAllDraw; }				// 全体で描画するか
		virtual bool IsPoseDraw() { return m_bPoseDraw; }			// ポーズ中描画するか
		virtual bool IsReleaseScene() { return m_bReleaseScene; }	// シーンでリリースするか

		virtual void DeathFlag() { m_bDeath = true; }	// 自分自身の死亡フラグ
		virtual bool IsDeathFlag() { return m_bDeath; }		// 自分自身の死亡フラグ

	protected:
	private:
		// リスト
		static CObject* m_pTop[MAX_PRIORITY];	// トップ
		static CObject* m_pCur[MAX_PRIORITY];	// 最後尾
		CObject* m_pNext;						// 次
		CObject* m_pPrev;						// 前
		static int m_nNumObject[MAX_PRIORITY];	// オブジェクト数
		int m_ID;	// オブジェクトID
		int m_nPriority;
		// 変数

		X m_x;	// トランスフォーム
		float m_fDistance;	// カメラからの距離

		bool m_bAllUpdate;						// 全体で更新するか
		bool m_bPoseUpdate;						// ポーズ中更新するか
		bool m_bAllDraw;						// 全体で描画するか
		bool m_bPoseDraw;						// ポーズ中描画するか
		bool m_bReleaseScene;					// シーンでリリースするか

		bool m_bDeath;							// 死  亡フラグ
		static constexpr int s_nDefalut_Priority = 3;	// 優先度未設定時の優先度
	};
}
#endif // !_OBJECT_H_