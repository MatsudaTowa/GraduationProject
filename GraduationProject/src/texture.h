//=============================================
//
//テクスチャ管理[texture.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _TEXTURE_H_ //これが定義されてないとき

#define _TEXTURE_H_
#include "main.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief テクスチャ管理クラス */
	class CTexture
	{
	public:
		static constexpr int MAX_TEX = 256; //テクスチャの数
		CTexture();
		~CTexture();
		void Unload(); //テクスチャ破棄
		int Regist(const std::string* pTex);
		LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	private:
		LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX]; //テクスチャへのポインタ
		std::string* m_texName[MAX_TEX]; //テクスチャネーム保存用
		int m_nNumAll; //テクスチャ総数
	};
}

#endif