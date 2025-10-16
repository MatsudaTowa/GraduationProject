//===========================================
// 
// フォグ[fog.h]
// Auther:UedaKou
// 
//===========================================
#include "fog.h"		// フォグ処理
#include "../../system/manager.h"	// ゲーム全体を管理するクラス

namespace My
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CFog::CFog()
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// フォグ有効設定
		pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		// フォグカラー
		pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		// フォグパラメータ
		m_type = EXP;
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

		// 距離指定

		m_fFogStartPos = 100.0f;
		m_fFogEndPos = 10000.0f;
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStartPos));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEndPos));

		// 密度指定
		m_m_fFogDensity = 0.002f;
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_m_fFogDensity));
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CFog::~CFog()
	{

	}
	/// <summary>
	/// フォグ設定
	/// </summary>
	/// <param name="type">フォグの種類</param>
	void CFog::SetFogType(FogType type)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// フォグパラメータ
		m_type = type;
		switch (type)
		{
		case EXP:
			pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
			break;
		case LINEAR:
			pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
			break;
		default:
			break;
		}
	}
	/// <summary>
	/// 線形フォグの開始距離と終了距離を設定します。
	/// </summary>
	/// <param name="fStart">フォグが始まる距離</param>
	/// <param name="fEnd">フォグが完全にかかる距離</param>
	void CFog::SetFogLinear(float fStart, float fEnd)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// 距離指定
		m_fFogStartPos = fStart;
		m_fFogEndPos = fEnd;
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStartPos));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEndPos));

	}
	/// <summary>
	/// 指数フォグの密度を設定します。
	/// フォグがどれだけ急激に濃くなるかを制御します。
	/// </summary>
	/// <param name="fDensity">フォグの密度（通常は0.0～1.0）</param>
	void CFog::SetFogDensity(float fDensity)
	{
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		// 密度指定
		m_m_fFogDensity = fDensity;
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_m_fFogDensity));
	}
	/// <summary>
	/// フォグの生成
	/// </summary>
	/// <returns>生成したポインター</returns>
	CFog* CFog::create()
	{
		return new CFog;
	}
}