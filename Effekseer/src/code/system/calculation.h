//============================================
//
// 計算用[calculation.h]
// Author:Uedakou
// 
//============================================
#ifndef _CALCULATION_H_
#define _CALCULATION_H_
//********************************************
// インクルード
//********************************************
//#include "manager.h"	// 全体マネージャー
//#include "pch.h"	// プリコンパイルヘッダ
//***************************************
// 計算用定義
//***************************************

#define TAU (D3DX_PI * 2)	// τ

namespace My
{
	// トランスフォーム
	typedef struct {
		D3DXVECTOR3 pos;	// 位置
		D3DXVECTOR3 rot;	// 向き
		D3DXVECTOR3 scl;	// 大きさ(倍率)
	}Transform;

	// トランスフォーム
	class X : public Transform
	{
	public:
		X() {}
		X(D3DXVECTOR3 Vec3pos, D3DXVECTOR3 Vec3rot, D3DXVECTOR3 Vec3siz) {
			pos = Vec3pos;
			rot = Vec3rot;
			scl = Vec3siz;
		}
		X& operator+= (const X& transform)
		{
			pos.x += transform.pos.x;
			pos.y += transform.pos.y;
			pos.z += transform.pos.z;

			rot.x += transform.rot.x;
			rot.y += transform.rot.y;
			rot.z += transform.rot.z;

			scl.x += transform.scl.x;
			scl.y += transform.scl.y;
			scl.z += transform.scl.z;
			return *this;
		}
		X& operator-= (const X& transform)
		{
			pos.x -= transform.pos.x;
			pos.y -= transform.pos.y;
			pos.z -= transform.pos.z;

			rot.x -= transform.rot.x;
			rot.y -= transform.rot.y;
			rot.z -= transform.rot.z;

			scl.x -= transform.scl.x;
			scl.y -= transform.scl.y;
			scl.z -= transform.scl.z;
			return *this;
		}
		X& operator*= (const X& transform)
		{
			pos.x *= transform.pos.x;
			pos.y *= transform.pos.y;
			pos.z *= transform.pos.z;

			rot.x *= transform.rot.x;
			rot.y *= transform.rot.y;
			rot.z *= transform.rot.z;

			scl.x *= transform.scl.x;
			scl.y *= transform.scl.y;
			scl.z *= transform.scl.z;
			return *this;
		}
		X& operator/= (const X& transform)
		{
			pos.x /= transform.pos.x;
			pos.y /= transform.pos.y;
			pos.z /= transform.pos.z;

			rot.x /= transform.rot.x;
			rot.y /= transform.rot.y;
			rot.z /= transform.rot.z;

			scl.x /= transform.scl.x;
			scl.y /= transform.scl.y;
			scl.z /= transform.scl.z;
			return *this;
		}
	};
	//***************************************
	// 計算
	//***************************************
	float RadianToAngle(float radian);	// ラジアンからアングルへの変換
	float AngleToRadian(float fAngle);	// アングルからラジアンへの変換
}

#endif // !_CALCULATION_H_