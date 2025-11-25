//================================
//
//フォントのテクスチャマネージャー[font_texture_manager]
//Author:towa matsuda
// 
//================================
#include "font_texture_manager.h"
#include <tchar.h>
#include <string>

namespace
{
	struct FontData
	{
		const LPCSTR FONT_PATH;
		const LPCSTR FONT_NAME;
	};

	const int FONT_SIZE = 256.0; //ポリゴンサイズじゃなくてフォントのサイズ。これを低くするとガビガビになる
	const int NUM_FONT = 7;

	FontData FONT_DATA[NUM_FONT]
	{
		{"data\\FONT\\GenEiChikugoMin3-R.ttf","源暎ちくご明朝 v3 Regular"},
		{"data\\FONT\\MOBO-ExtraLight.otf","モボ-ExtraLight"},
		{"data\\FONT\\oshigo.otf","推しゴ"},
		{"data\\FONT\\Kazesawa-Bold.ttf","Kazesawa Bold"},
		{"data\\FONT\\GenEiLateMin_v2.ttc","源暎ラテミン v2"},
		{"data\\FONT\\H7GBKHeavy.TTF","AR FangXinShuH7GBK HV"},
		{"data\\FONT\\ja-jp.ttf","SDK_JP_Web"},
	};
}
//================================
// 
//================================
My::CFontTextureManager::CFontTextureManager()
{
}

//================================
// 
//================================
My::CFontTextureManager::~CFontTextureManager()
{
}

//================================
// 
//================================
void My::CFontTextureManager::Unload()
{
	for (auto& itr : m_tex)
	{
		if (itr.second)
			itr.second->Release();
	}
	m_tex.clear();
}

//================================
// 
//================================
My::CFontTextureManager::FontTextureStuts My::CFontTextureManager::Regist(int font_idx, WCHAR text, int size, int thickness)
{
    FontTextureStuts key{ font_idx,text, size, thickness };

    // 既に登録済みならkeyを返す
    auto itr = m_tex.find(key);
    if (itr != m_tex.end()) 
    {
        return key;
    }

    // 未作成 → DirectX テクスチャ生成
//テクスチャ作成
	LPDIRECT3DTEXTURE9 pTex = nullptr;
	GET_DEVICE->CreateTexture(
		(UINT)FONT_SIZE, (UINT)FONT_SIZE,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&pTex,
		NULL
	);

	//GDIフォントの準備
	HDC hDC = CreateCompatibleDC(NULL);

	// フォント登録（外部ファイル）
	AddFontResourceEx(_T(FONT_DATA[key.font_idx].FONT_PATH), FR_PRIVATE, NULL);

	//第1引数:フォントサイズ
	//第2引数:引き伸ばし
	//第3引数:文字送りの方向とX軸との角度
	//第4引数:ベースラインとX軸との角度
	//第5引数:文字の太さ
	//第6引数:斜体指定
	//第7引数:下線指定
	//第8引数:打消し指定
	//第9引数:キャラクタセット
	//第10引数:出力制度
	//第11引数:クリッピングの精度
	//第12引数:出力品質
	//第13引数:ピッチとファミリ
	//第14引数:フォント名
	HFONT hFont = CreateFont(
		FONT_SIZE, 0, 0, 0,                    // ← 少し大きめで綺麗に
		thickness, FALSE, FALSE, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		CLEARTYPE_NATURAL_QUALITY,              // ←ClearTypeで滑らかに
		FIXED_PITCH | FF_MODERN,
		_T(FONT_DATA[key.font_idx].FONT_NAME)                 // フォントのパスではなくフォントの内部のフォント名で
	);
	SelectObject(hDC, hFont);

	//使用フォントの情報取得
	TEXTMETRIC tm;
	GetTextMetrics(hDC, &tm);
	//文字のベースラインから一番上まで
	int ascent = tm.tmAscent;
	//文字のベースラインから一番下まで
	//int descent = tm.tmDescent;

	int baseline = ascent; // ベースラインはフォント上部からの距離

	// ④ 文字ビットマップ取得
	GLYPHMETRICS gm;
	int bmpW, bmpH, level;
	BYTE* pBmp = GetFontBitmap(hDC, key.text, gm, bmpW, bmpH, level);
	if (!pBmp)
	{
		assert(false);
	}

	// ⑤ サーフェイスロック
	D3DLOCKED_RECT lockRect;
	pTex->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
	FillMemory(lockRect.pBits, lockRect.Pitch * FONT_SIZE, 0);

	// ⑥ ベースライン補正計算
	// gmptGlyphOrigin.y はベースラインからの相対位置（上方向が正）
	// baselineとの差を取ることで、文字を統一的に配置
	int yOffset = baseline - gm.gmptGlyphOrigin.y;
	int xOffset = gm.gmptGlyphOrigin.x;

	// ⑦ テクスチャへフォント情報の書き込み
	for (int y = 0; y < bmpH; y++)
	{
		for (int x = 0; x < bmpW; x++)
		{
			int destX = x + xOffset;
			int destY = y + yOffset;

			if (destY < 0 || destY >= FONT_SIZE || destX < 0 || destX >= FONT_SIZE) continue;

			BYTE val = pBmp[x + bmpW * y];
			BYTE alpha = (BYTE)(255 * val / 64);
			DWORD color = 0x00ffffff | (alpha << 24);
			memcpy((BYTE*)lockRect.pBits + lockRect.Pitch * destY + 4 * destX, &color, sizeof(DWORD));
		}
	}

	// ⑧ アンロック
	pTex->UnlockRect(0);

	// ⑨ クリーンアップ
	DeleteObject(hFont);
	DeleteDC(hDC);
	delete[] pBmp;

	// ④ map に登録
	m_tex[key] = pTex;

	// ⑤ key を返す
	return key;
}

//=============================================
// ビットマップ取得（Unicode対応版）
//=============================================
BYTE* My::CFontTextureManager::GetFontBitmap(HDC hDC, WCHAR c, GLYPHMETRICS& gm, int& bmpWidth, int& bmpHeight, int& level)
{
	level = GGO_GRAY8_BITMAP; // 高品質グレースケール

	MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} }; // スケーリングを1:1に
	DWORD bufSize = GetGlyphOutlineW(hDC, c, level, &gm, 0, NULL, &mat);
	if (bufSize == GDI_ERROR || bufSize == 0) return nullptr;

	BYTE* pBmp = new BYTE[bufSize];
	GetGlyphOutlineW(hDC, c, level, &gm, bufSize, pBmp, &mat);

	// 横幅を4の倍数に揃える
	bmpWidth = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
	bmpHeight = gm.gmBlackBoxY;

	return pBmp;
}

//================================
// 
//================================
LPDIRECT3DTEXTURE9 My::CFontTextureManager::GetAddress(const FontTextureStuts& key)
{
	auto it = m_tex.find(key);
	if (it != m_tex.end())
	{
		return it->second;
	}
	return nullptr;
}
