//============================================
//
// 計算用[calculation.h]
// Author:Uedakou
// 
//============================================
#include "calculation.h"

namespace My
{
	//=======================================
	// 計算用処理
	//=======================================

	/// <summary>
	/// ラジアン（弧度法）からアングル（度数法）への変換を行います。
	/// </summary>
	/// <param name="radian">ラジアン値（弧度法の角度）</param>
	/// <returns>アングル値（度数法）</returns>
	float RadianToAngle(float radian)
	{
		return(360.0f / (D3DX_PI * 2)) * radian;
	}
	/// <summary>
	/// アングル（度数法）からラジアン（弧度法）への変換を行います。
	/// </summary>
	/// <param name="fAngle">アングル値（度数法）</param>
	/// <returns>ラジアン値（弧度法の角度）</returns>
	float AngleToRadian(float fAngle)
	{
		return(D3DX_PI * 2 / 360.0f) * fAngle;
	}
}