//=============================================
//
//テクスチャ管理[texture.cpp]
//Author Matsuda Towa
//
//=============================================
#include "texture.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 */
My::CTexture::CTexture()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;
	}
}

/**
 * @brief デストラクタ
 */
My::CTexture::~CTexture()
{
}

/**
 * @brief テクスチャ破棄
 */
void My::CTexture::Unload()
{
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}

/**
 * @brief テクスチャ登録
 * @param [in]テクスチャのファイルパス
 */
int My::CTexture::Regist(const std::string pTex)
{
	int nIdx = 0;
	for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
	{
		if (m_apTexture[nCnt] == nullptr)
		{
			LPDIRECT3DDEVICE9 pDevice = GET_RENDERER->GetDevice();

			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pTex.c_str(),
				&m_apTexture[nCnt]);
			
			//引数のファイルパスを保存
			m_texName[nCnt] = pTex;
			nIdx = nCnt;	//番号の保存
			break;
		}
		else if (m_texName[nCnt] ==pTex)
		{//引数のテクスチャが存在するなら

			//番号を代入してbreak
			nIdx = nCnt;
			break;
		}
	}
 	return nIdx;
}

/**
 * @brief テクスチャアドレス取得
 * @param [in]番号
 * @return テクスチャアドレス
 */
LPDIRECT3DTEXTURE9 My::CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];
}
