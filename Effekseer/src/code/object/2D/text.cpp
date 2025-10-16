//===========================================
// 
// テキスト[text.cop]
// Auther:UedaKou
// 
//===========================================
#include "text.h"				// テキストオブジェクト
#include <strsafe.h>			// StringCbPrintfを使用するにのに必要
//#include "../../system/pch.h"		// プリコンパイル
#include "../../system/manager.h"// ゲーム全体を管理するクラス

namespace My
{
	// フォントの種類
	string CText::m_aFontList[Max] = {
		{"Terminal"},
	};

	const int	CText::s_nHyde = 18;	// 文字高さ	
	const UINT	CText::s_nWide = 0;	// 文字幅
	const UINT	CText::s_nBold = 0;	// 文字太さ
	const UINT	CText::s_nMip = 0;		// ミップレベル	
	const bool	CText::s_bItalic = FALSE;	// 文字が斜体か
	const DWORD	CText::s_nFont = SHIFTJIS_CHARSET;		// フォント
	const DWORD	CText::s_nSize = OUT_DEFAULT_PRECIS;	// 文字サイズ(変更不可)
	const DWORD	CText::s_nPitch = DEFAULT_PITCH;		// ピッチ
	const CText::Type	CText::s_type = Type::Terminal;		// 書体を含む文字列
	const RECT	CText::s_Space = { 0, 0, SCREEN_H , SCREEN_W };	// 表示領域
	const D3DXCOLOR CText::s_pCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 色
	const string	CText::s_aText = "テキストを入力してください";		// テキスト

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CText::CText()
	{
		m_pFont = nullptr;		// フォント
		m_nHyde = s_nHyde;		// 文字高さ
		m_nWide = s_nWide;		// 文字幅
		m_nBold = s_nBold;		// 文字太さ
		m_nMip = s_nMip;		// ミップレベル
		m_bItalic = s_bItalic;	// 文字が斜体か
		m_nFont = s_nFont;		// フォント
		m_nSize = s_nSize;		// 文字サイズ(変更不可)
		m_nPitch = s_nPitch;	// ピッチ
		m_type = s_type;		// 書体名を含む文字列
		m_Space = s_Space;		// 領域
		m_pCol = s_pCol;		// 文字色
		m_aText = s_aText;		// 表示テキスト
	}
	/// <summary>
	/// コンストラクタ優先度付き
	/// </summary>
	/// <param name="nPriorithi">優先度</param>
	CText::CText(int nPriorithi) :
		CObject(nPriorithi)
	{
		m_pFont = nullptr;		// フォント
		m_nHyde = s_nHyde;		// 文字高さ
		m_nWide = s_nWide;		// 文字幅
		m_nBold = s_nBold;		// 文字太さ
		m_nMip = s_nMip;		// ミップレベル
		m_bItalic = s_bItalic;	// 文字が斜体か
		m_nFont = s_nFont;		// フォント
		m_nSize = s_nSize;		// 文字サイズ(変更不可)
		m_nPitch = s_nPitch;	// ピッチ
		m_type = s_type;		// 書体名を含む文字列
		m_Space = s_Space;		// 領域
		m_pCol = s_pCol;		// 文字色
		m_aText = s_aText;		// 表示テキスト

	}
	/// <summary>
	/// デストラクタ
	/// </summary>
	CText::~CText()
	{
		// フォント解放
		if (m_pFont != nullptr) {
			m_pFont->Release();
			m_pFont = nullptr;
		}
	}
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>true : 初期化成功</returns>
	HRESULT CText::Init()
	{
		// 設定を反映
		SetData();

		return S_OK;	// 成功を返す
	}
	/// <summary>
	/// 終了処理
	/// </summary>
	void CText::Uninit()
	{
	}
	/// <summary>
	/// 更新
	/// </summary>
	void CText::Update()
	{
	}
	/// <summary>
	/// 描画
	/// </summary>
	void CText::Draw()
	{
		// テキストが空でなければ描画処理を行う
		if (!m_aText.empty())
		{
			// フォントオブジェクトが有効であれば描画
			if (m_pFont != nullptr)
			{
				m_pFont->DrawText(nullptr, m_aText.c_str(), -1, &m_Space, DT_LEFT, m_pCol);
			}
		}
	}
	/// <summary>
	/// DirectX フォントオブジェクトを作成し、テキスト描画に必要な設定を行います。
	/// </summary>
	void CText::SetData()
	{
		CManager* instance = CManager::GetInstance();	// インスタンス
		LPDIRECT3DDEVICE9 pDevice = instance->GetRenderer()->GetDevice();	// デバイス
		// デバッグ表示用フォントの生成
		HRESULT hr = D3DXCreateFont(pDevice,	// デバイス
			m_nHyde,						// 型(int)	論理単位での文字の高さ。
			m_nWide,						// 型(UINT)	論理単位での文字の幅。
			m_nBold,						// 型(UINT)	書体の太さ。 1 つの例は太字です。
			m_nMip,							// 型(UInt)	ミップマップ レベルの数。
			m_bItalic,						// 型(BOOL)		斜体フォントの場合は True、それ以外の場合は false。
			m_nFont,						// 型(DWORD)	フォントの文字セット。
			m_nSize,						// 型(DWORD)	Windows が目的のフォント サイズと特性を実際のフォントと照合する方法を指定します。 たとえば、OUT_TT_ONLY_PRECISを使用して、常に TrueType フォントを取得できるようにします。
			DEFAULT_QUALITY,				// 型(DWORD)	Windows が目的のフォントと実際のフォントを一致させる方法を指定します。 ラスター フォントにのみ適用され、TrueType フォントには影響しません。
			m_nPitch,						// 型(DWORD)	ピッチとファミリ インデックス。
			m_aFontList[m_type].c_str(),	// 種類(LPCTSTR)	書体名を含む文字列。 コンパイラ設定で Unicode が必要な場合、データ型 LPCTSTR は LPCWSTR に解決されます。 それ以外の場合、文字列データ型は LPCSTR に解決されます。
			&m_pFont);						// 種類(LPD3DXFONT*)	作成されたフォント オブジェクトを表す ID3DXFont インターフェイスへのポインターを返します。

		// hr が失敗を示していたら
		if (FAILED(hr)) {
			// エラー処理
			OutputDebugStringA("エラー: SetData - フォント生成に失敗しました。\n");
			Beep(1200, 300);
			m_pFont = nullptr;
		}
	}
	/// <summary>
	/// 全描画用のフォント設定を一括で再設定します。
	/// </summary>
	/// <param name="nH">		文字の高さ</param>		
	/// <param name="nW">		文字の幅</param>		
	/// <param name="nBold">	書体の太さ</param>	
	/// <param name="nMip">		ミップマップ レベルの数</param>		
	/// <param name="bItalic">	斜体フォントの場合は True、それ以外の場合は false</param>	
	/// <param name="nFont">	フォントの文字セット</param>	
	/// <param name="nSize">	Windows が目的のフォント サイズと特性を実際のフォントと照合する方法を指定します</param>	
	/// <param name="nPitch">	ピッチとファミリ インデックス</param>	
	/// <param name="type">		書体名を含む文字列</param>	
	void CText::SetAll(int nH, UINT nW, UINT nBold, UINT nMip, bool bItalic, DWORD nFont, DWORD nSize, DWORD nPitch, Type type)
	{
		m_nHyde = nH;		// 文字高さ
		m_nWide = nW;		// 文字幅
		m_nBold = nBold;	// 文字太さ
		m_nMip = nMip;		// ミップレベル
		m_bItalic = bItalic;// 文字が斜体か
		m_nFont = nFont;	// フォント
		m_nSize = nSize;	// 文字サイズ(変更不可)
		m_nPitch = nPitch;	// ピッチ
		m_type = type;		// 書体名を含む文字列
		SetData();
	}
	/// <summary>
	/// 固定テキストを設定
	/// 空にするとPrint 描画可能
	/// </summary>
	/// <param name="text"> 設定したいテキスト</param>
	void CText::SetText(const char* text)
	{
		m_aText = text;
	}
	/// <summary>
	/// テキスト設定
	/// </summary>
	/// <param name="text">設定したいテキスト</param>
	void CText::SetText(string text)
	{
		m_aText = text;
	}
	/// <summary>
	/// フォント取得
	/// </summary>
	/// <returns>現在フォント</returns>
	LPD3DXFONT CText::GetFont()
	{
		return m_pFont;
	}
	//===============================================
	// 生成
	//===============================================
	/// <summary>
	/// 新しいテキストオブジェクトを生成します。
	/// </summary>
	/// <returns>生成されたオブジェクトのポインタ</returns>
	CText* CText::create()
	{
		CText* pTex = new CText();	// オブジェクト生成

		// 初期化処理
		pTex->Init();

		return pTex;
	}
	/// <summary>
	/// 指定したサイズやスタイルで新しいテキストオブジェクトを生成します。
	/// </summary>
	/// <param name="nHyde">文字の大きさ</param>
	/// <param name="nWide">文字の幅</param>
	/// <param name="nBold">文字の太さ</param>
	/// <param name="nMip">ミップレベル</param>
	/// <param name="bItalic">斜体ならtrue違うならfalse</param>
	/// <returns>生成されたオブジェクトのポインタ</returns>
	CText* CText::create(int nHyde, UINT nWide, UINT nBold, UINT nMip, bool bItalic)
	{
		CText* pTex = new CText();	// オブジェクト生成

		pTex->m_nHyde = nHyde;		// 文字高さ
		pTex->m_nWide = nWide;		// 文字幅
		pTex->m_nBold = nBold;		// 文字太さ
		pTex->m_nMip = nMip;		// ミップレベル
		pTex->m_bItalic = bItalic;	// 文字が斜体か

		// 初期化処理
		pTex->Init();

		return pTex;
	}
	/// <summary>
	/// 詳細なフォント設定を指定して新しいテキストオブジェクトを生成します。
	/// </summary>
	/// <param name="nHyde">文字の大きさ</param>
	/// <param name="nWide">文字の幅</param>
	/// <param name="nBold">文字の太さ</param>
	/// <param name="nMip">ミップレベル</param>
	/// <param name="bItalic">斜体ならtrue違うならfalse</param>
	/// <param name="nFont">フォント文字セット</param>
	/// <param name="nSize">Windows が目的のフォント サイズと特性を実際のフォントと照合する方法を指定します</param>
	/// <param name="nPitch">ピッチとインデックス</param>
	/// <param name="type">書体</param>
	/// <returns>生成されたオブジェクトのポインタ</returns>
	CText* CText::create(int nHyde, UINT nWide, UINT nBold, UINT nMip, bool bItalic, DWORD nFont, DWORD nSize, DWORD nPitch, Type type)
	{
		CText* pTex = new CText();	// オブジェクト生成

		pTex->m_nHyde = nHyde;		// 文字高さ
		pTex->m_nWide = nWide;		// 文字幅
		pTex->m_nBold = nBold;		// 文字太さ
		pTex->m_nMip = nMip;		// ミップレベル
		pTex->m_bItalic = bItalic;	// 文字が斜体か
		pTex->m_nFont = nFont;		// フォント
		pTex->m_nSize = nSize;		// 文字サイズ(変更不可)
		pTex->m_nPitch = nPitch;	// ピッチ
		pTex->m_type = type;		// 書体名を含む文字列

		// 初期化処理
		pTex->Init();

		return pTex;
	}
}