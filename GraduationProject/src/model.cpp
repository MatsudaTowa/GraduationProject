//=============================================
//
//モデル管理[model.cpp]
//Author Matsuda Towa
//
//=============================================
#include "model.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 */
My::CModel::CModel():
	m_ModelInfo(),			//モデル情報構造体
	m_nNumAll(INT_ZERO)		//モデル総数
{
}

/**
 * @brief デストラクタ
 */
My::CModel::~CModel()
{
}

/**
 * @brief モデル破棄
 */
void My::CModel::Unload()
{
	for (int nCnt = INT_ZERO; nCnt < MAX_MODEL; nCnt++)
	{
		if (m_ModelInfo[nCnt].pBuffMat != nullptr)
		{//マテリアル破棄
			m_ModelInfo[nCnt].pBuffMat->Release();
			m_ModelInfo[nCnt].pBuffMat = nullptr;
			m_ModelInfo[nCnt].dwNumMat = INT_ZERO;
		}
		if (m_ModelInfo[nCnt].pMesh != nullptr)
		{//メッシュ破棄
			m_ModelInfo[nCnt].pMesh->Release();
			m_ModelInfo[nCnt].pMesh = nullptr;
		}

		if (m_ModelInfo[nCnt].ModelName != nullptr)
		{//モデルネーム破棄
			m_ModelInfo[nCnt].ModelName = nullptr;
		}
	}
	m_nNumAll = 0;
}

/**
 * @brief モデル登録
 * @param [in]モデルのファイルパス
 */
int My::CModel::Regist(const char* pModel)
{
	int nIdx = INT_ZERO;
	for (int nCnt = INT_ZERO; nCnt < MAX_MODEL; nCnt++)
	{
		if (m_ModelInfo[nCnt].pBuffMat == nullptr
			&& m_ModelInfo[nCnt].pMesh == nullptr)
		{
			LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;

			//Xファイルの読み込み
			D3DXLoadMeshFromX(pModel,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_ModelInfo[nCnt].pBuffMat,
				NULL,
				&m_ModelInfo[nCnt].dwNumMat,
				&m_ModelInfo[nCnt].pMesh);

			//引数のファイルパスを保存
			m_ModelInfo[nCnt].ModelName = pModel;
			nIdx = nCnt;	//番号の保存
			m_nNumAll++;	//総数のカウントアップ
			break;
		}
		if (strcmp(m_ModelInfo[nCnt].ModelName, pModel) == INT_ZERO)
		{//引数のモデルが存在するなら
		
			//番号を代入してbreak
			nIdx = nCnt;
			break;
		}
	}
	return nIdx;
}

/**
 * @brief モデル取得
 * @param [in]番号
 */
My::CModel::MODEL_INFO My::CModel::GetModelInfo(int nIdx)
{
	return m_ModelInfo[nIdx];
}

