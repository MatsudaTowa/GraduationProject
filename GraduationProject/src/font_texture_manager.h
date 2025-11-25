//================================
//
//フォントのマネージャー[font_texture_manager]
//Author:towa matsuda
// 
//================================
#ifndef _FONT_TEXTURE_MANAGER_H_ //このマクロ定義がされていなかったら
#define _FONT_TEXTURE_MANAGER_H_ //２重インクルード防止のマクロ定義

//ヘッダーのインクルード
#include "main.h"
#include "font.h"

namespace My
{
	//エネミーマネージャーのクラス
	class CFontTextureManager
	{
	public:
		static constexpr int MAX_TEX = 256; //テクスチャの数

		struct FontTextureStuts
		{
			int font_idx;
			WCHAR text;
			int size;               // サイズ
			int thickness;          // 太さ

			bool operator<(const FontTextureStuts& other) const
			{
				return std::tie(font_idx, text, size, thickness)
					< std::tie(other.font_idx, other.text, other.size, other.thickness);
			}
		};

		/**
		 * @brief 自分自身のインスタンスを生成し取得
		 */
		static CFontTextureManager* GetInstance()
		{
			//静的インスタンス
			static CFontTextureManager instance;
			return &instance;
		}

		CFontTextureManager();
		~CFontTextureManager();
		void Unload(); //テクスチャ破棄
		FontTextureStuts Regist(int font_idx, WCHAR text, int size, int thickness);
		LPDIRECT3DTEXTURE9 GetAddress(const FontTextureStuts& key);

	private:
		// フォントビットマップを取得する関数
		BYTE* GetFontBitmap(HDC hDC, WCHAR c, GLYPHMETRICS& gm, int& bmpWidth, int& bmpHeight, int& level);
		std::map<FontTextureStuts, LPDIRECT3DTEXTURE9> m_tex;
	};
}

#endif