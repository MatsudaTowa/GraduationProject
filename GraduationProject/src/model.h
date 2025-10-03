//=============================================
//
//モデル管理[model.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _MODEL_H_ //これが定義されてないとき

#define _MODEL_H_
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief モデル管理クラス */
	class CModel
	{
	public:
		static constexpr int MAX_MODEL = 256; //モデルの数

		//モデル情報構造体定義
		struct MODEL_INFO
		{
			LPD3DXMESH pMesh; //メッシュ情報
			LPD3DXBUFFER pBuffMat; //マテリアル情報
			DWORD dwNumMat; //マテリアル数;
			const char* ModelName; //モデルネーム保存用
		};

		CModel();
		~CModel();
		void Unload(); //モデル破棄
		int Regist(const char* pModel);
		MODEL_INFO GetModelInfo(int nIdx); //モデル情報取得

	private:
		MODEL_INFO m_ModelInfo[MAX_MODEL]; //モデル情報
		int m_nNumAll; //モデル総数
	};
}
#endif