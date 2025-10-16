//===========================================
// 
// テキスト[text.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _TEXT_H_
#define _TEXT_H_
#include "../object.h"
//#include "../../system/pch.h"	// プリコンパイル

namespace My
{

	class CText :public CObject
	{
	public:
		typedef enum {
			Terminal = 0,// ターミナル
			Max,
		}Type;	// フォントタイプ
		CText();
		CText(int nPriorithi);
		virtual ~CText()		override;
		virtual HRESULT Init()		override;	// 初期化処理
		virtual void Uninit()	override;	// 終了処理
		virtual void Update()	override;	// 更新処理
		virtual void Draw()		override;	// テキストを固定表示


		// 表示設定
		void SetData();	// 設定反映
		void SetAll(int nH, UINT nW, UINT nBold, UINT nMip, bool bItalic, DWORD nFont, DWORD nSize, DWORD nPitch, Type type);	// 全設定
		void SetHyde(int nHyde) { m_nHyde = nHyde; SetData(); }		// 高さ
		void SetWide(int nWide) { m_nWide = nWide; SetData(); }		// 幅
		void SetBold(int nBold) { m_nBold = nBold; SetData(); }		// 斜体
		void SetMip(int nMip) { m_nMip = nMip; SetData(); }			// ミップマップ
		void SetItalic(bool bItalic) { m_bItalic = bItalic; SetData(); }// 斜体フォント
		void SetFont(UINT nFont) { m_nFont = nFont; SetData(); }	// フォント
		void SetSize(UINT nSize) { m_nSize = nSize; SetData(); }	// 大きさ
		void SetPitch(UINT nPitch) { m_nPitch = nPitch; SetData(); }// ピッチ
		void SetType(Type Type) { m_type = Type; SetData(); }		// 種類
		void SetCol(D3DXCOLOR pCol) { m_pCol = pCol; }				// 色
		void SetSpace(int nSpaceSW, int nSpaceSH, int nSpaceBW, int nSpaceBH) {	// 領域
			m_Space = { nSpaceSW, nSpaceSH, nSpaceBW, nSpaceBH };	//領域
		}
		void SetText(const char* text);	// テキスト設定
		void SetText(string text);		// テキスト設定

		void PrintText(string text) { m_aText += text; }	// 与えられた文字列を内部テキストに追加

		LPD3DXFONT GetFont();	// フォント取得

		void ClearText() { m_aText = ""; }	// テキストクリア
		// 生成
		static CText* create();
		static CText* create(int nHyde, UINT nWide, UINT nBold, UINT nMip, bool bItalic);
		static CText* create(int nHyde, UINT nWide, UINT nBold, UINT nMip, bool bItalic, DWORD nFont, DWORD nSize, DWORD nPitch, Type type);
	protected:
		static string m_aFontList[Max];	// フォント名

		int		m_nHyde;	// 文字高さ	
		UINT	m_nWide;	// 文字幅
		UINT	m_nBold;	// 文字太さ
		UINT	m_nMip;		// ミップレベル	
		bool	m_bItalic;	// 文字が斜体か
		DWORD	m_nFont;	// フォント
		DWORD	m_nSize;	// 文字サイズ(変更不可)
		DWORD	m_nPitch;	// ピッチ
		Type	m_type;		// 書体を含む文字列
		RECT	m_Space;
		D3DXCOLOR m_pCol;	// 色
		LPD3DXFONT m_pFont;// フォントのポインタ
		string	m_aText;	// テキスト
	public:
		static const int	s_nHyde;	// 文字高さ	
		static const UINT	s_nWide;	// 文字幅
		static const UINT	s_nBold;	// 文字太さ
		static const UINT	s_nMip;		// ミップレベル	
		static const bool	s_bItalic;	// 文字が斜体か
		static const DWORD	s_nFont;	// フォント
		static const DWORD	s_nSize;	// 文字サイズ(変更不可)
		static const DWORD	s_nPitch;	// ピッチ
		static const Type	s_type;		// 書体を含む文字列
		static const RECT	s_Space;
		static const D3DXCOLOR s_pCol;	// 色
		static const LPD3DXFONT s_pFont;// フォントのポインタ
		static const string	s_aText;	// テキスト
	};

}
#endif // !_TEXT_H_