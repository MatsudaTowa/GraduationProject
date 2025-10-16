//============================================
//
// 光源[light.cpp]
// Author:Uedakou
// 
//============================================
#include "light.h"	// ライト
#include "../../system/manager.h"	// 全体マネージャー
#include <strsafe.h>


namespace My
{
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CLight::CLight()
	{
		// ライト構造体をゼロ初期化
		//ZeroMemory(&m_light, sizeof(D3DLIGHT9));
	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CLight::~CLight()
	{
	}
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>正常終了ならS_OK</returns>
	HRESULT CLight::Init()
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();// デバイスへのポインタ

		// ライトの方向ベクトル（一時的に使用）
		D3DXVECTOR3 vecDir = { 0.0f, 0.0f,0.0f };

		// ライトの情報をクリアする
		ZeroMemory(&m_light, sizeof(D3DLIGHT9));

		for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
		{
			// ライトの種類をディレクショナルライトに設定
			m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

			// 拡散光の色（白）を設定
			m_light[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// ライトの方向ベクトルを設定（各ライトごとに異なる方向）
			switch (nCntLight)
			{
			case 0:
				// ライトの方向設定
				vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				break;
			case 1:
				// ライトの方向設定
				vecDir = D3DXVECTOR3(-0.2f, 0.8f, 0.4f);
				break;
			case 2:
				// ライトの方向設定
				vecDir = D3DXVECTOR3(0.8f, -0.8f, -0.4f);
				break;
			}

			// 方向ベクトルを正規化
			D3DXVec3Normalize(&vecDir, &vecDir);

			// ライトに方向を設定
			m_light[nCntLight].Direction = vecDir;

			// ライトを設定する
			pDevice->SetLight(nCntLight, &m_light[nCntLight]);

			// ライトを有効にする
			pDevice->LightEnable(nCntLight, TRUE);

			// 次のループ用にベクトルを初期化（実は不要、上書きされるため）
			vecDir = { 0.0f, 0.0f,0.0f };
		}

		return S_OK;
	}
	void CLight::Uninit()
	{}
	void CLight::Update()
	{}
}