//=============================================
//
//フォント表示[font.cpp]
//Author Matsuda Towa
//
//=============================================
#include "font.h"

namespace
{
	const D3DXVECTOR2 FONT_SIZE = { 300.0f,300.0f };
}

//=============================================
// コンストラクタ
//=============================================
My::CFont::CFont(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CFont::~CFont()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CFont::Init()
{
    SetColor(COLOR_RED);

    SetPos({ 500.0f,500.0f,0.0f });

    SetSize(FONT_SIZE);

    // ① テクスチャ作成
    LPDIRECT3DTEXTURE9 pTex = nullptr;
    GET_DEVICE->CreateTexture(
        (UINT)FONT_SIZE.x, (UINT)FONT_SIZE.y,
        1,
        D3DUSAGE_DYNAMIC,
        D3DFMT_A8R8G8B8,
        D3DPOOL_DEFAULT,
        &pTex,
        NULL
    );

    // ② GDIフォントの準備
    HDC hDC = CreateCompatibleDC(NULL);
    HFONT hFont = CreateFont(
        32, 0, 0, 0,
        FW_NORMAL, FALSE, FALSE, FALSE,
        SHIFTJIS_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        ("HG創英角ｺﾞｼｯｸUB")
    );
    SelectObject(hDC, hFont);

    // ③ フォントビットマップ取得
    GLYPHMETRICS gm;
    int bmpW, bmpH, level;
    BYTE* pBmp = GetFontBitmap(hDC, ('あ'), gm, bmpW, bmpH, level); // ←ここでAを取得
    if (!pBmp) return E_FAIL;

    // ④ サーフェイスロック
    D3DLOCKED_RECT lockRect;
    pTex->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
    FillMemory(lockRect.pBits, lockRect.Pitch * (int)FONT_SIZE.y, 0);

    // ⑤ テクスチャへフォント情報の書き込み
    for (int y = 0; y < bmpH; y++)
    {
        for (int x = 0; x < bmpW; x++)
        {
            BYTE val = pBmp[x + bmpW * y];
            DWORD Trans = (255 * val) / (level - 1);
            DWORD color = 0x00ffffff | (Trans << 24);
            memcpy((BYTE*)lockRect.pBits + lockRect.Pitch * y + 4 * x, &color, sizeof(DWORD));
        }
    }

    // ⑥ アンロック
    pTex->UnlockRect(0);

    // GDIクリーンアップ
    DeleteObject(hFont);
    DeleteDC(hDC);
    delete[] pBmp;

    // ⑦ DirectX描画用セットアップ
    BindTexture(pTex);
    SetVtx();

    return S_OK;
}

void My::CFont::Uninit()
{
	CObject2D::Uninit();
}

void My::CFont::Update()
{
	SetVtx();
}

void My::CFont::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = GET_DEVICE;
	LPDIRECT3DTEXTURE9 tex = GetTexture();
	// テクスチャセット
	pDevice->SetTexture(0, tex);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); // 板ポリのα値を利用

	CObject2D::Draw();

	// レンダリングステート
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

BYTE* My::CFont::GetFontBitmap(HDC hDC, TCHAR c, GLYPHMETRICS& gm, int& bmpWidth, int& bmpHeight, int& level)
{
	level = GGO_GRAY8_BITMAP; //高品質のビットマップを使用

	 // 必要なバッファサイズを取得
	MAT2 mat = { {0,1}, {0,0}, {0,0}, {0,1} };
	DWORD bufSize = GetGlyphOutline(hDC, c, level, &gm, 0, NULL, &mat);
	if (bufSize == GDI_ERROR) { return nullptr; }

	// バッファ確保
	BYTE* pBmp = new BYTE[bufSize];
	GetGlyphOutline(hDC, c, level, &gm, bufSize, pBmp, &mat);

	// 横幅を4の倍数にそろえる
	bmpWidth = gm.gmBlackBoxX + (4 - (gm.gmBlackBoxX % 4)) % 4;
	bmpHeight = gm.gmBlackBoxY;

	return pBmp;
}
