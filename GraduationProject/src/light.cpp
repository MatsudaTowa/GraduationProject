//=============================================
//
//3DTemplate[light.cpp]
//Author Matsuda Towa
//
//=============================================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//=============================================
//コンストラクタ
//=============================================
My::CLight::CLight()
{
	//ライト情報のクリア
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));
}

//=============================================
//デストラクタ
//=============================================
My::CLight::~CLight()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CLight::Init()
{
	//CRenderer* pRender = CManager::GetRenderer();
	//LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();
	LPDIRECT3DDEVICE9 pDevice = GET_RENDERER->GetDevice();

	D3DXVECTOR3 vecDir; //設定方向ベクトル

	for (int nCnt = 0; nCnt < MAX_LIGHT; nCnt++)
	{
		//ライトの種類を設定
		m_aLight[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光の設定
		m_aLight[nCnt].Diffuse = COLOR_WHITE;

		if (nCnt == 0)
		{
			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
		}
		else if (nCnt == 1)
		{
			//ライトの方向を設定
			vecDir = D3DXVECTOR3(0.2f, 0.8f, 0.4f);
		}
		else if (nCnt == 2)
		{
			//ライトの方向を設定
			vecDir = D3DXVECTOR3(-0.7f, -0.8f, -0.4f);
		}

		D3DXVec3Normalize(&vecDir, &vecDir); //正規化

		m_aLight[nCnt].Direction = vecDir;

		//ライトを設定
		pDevice->SetLight(nCnt, &m_aLight[nCnt]);

		//ライトを有効化
		pDevice->LightEnable(nCnt, TRUE);
	}
	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CLight::Uninit()
{
}

//=============================================
//更新
//=============================================
void My::CLight::Update()
{
}
