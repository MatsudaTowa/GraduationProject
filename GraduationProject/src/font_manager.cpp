//================================
//
//フォントのマネージャー[font_manager.cpp]
//Author:towa matsuda
// 
//================================
#include "font_manager.h"
#include <algorithm>
namespace
{

}

//================================
// コンストラクタ
//================================
My::CFontManager::CFontManager():
m_base_size(),
m_base_text_shift(),
m_isLineBreak(),
m_thickness(),
m_text(nullptr),
m_align()
{
	m_Font.clear();
}

//================================
// コンストラクタ
//================================
My::CFontManager::CFontManager(TextAlign align):
m_base_size(),
m_base_text_shift(),
m_thickness(),
m_text(nullptr),
m_align()
{
	m_align = align;
	m_Font.clear();
}

//================================
// デストラクタ
//================================
My::CFontManager::~CFontManager()
{
}

//================================
// 初期化
//================================
HRESULT My::CFontManager::Init()
{
	return S_OK;
}

//================================
// 終了
//================================
void My::CFontManager::Uninit()
{

}

//================================
// 登録
//================================
void My::CFontManager::Regist(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col)
{
	//基準のサイズを設定
	m_base_size = base_size;
	m_base_text_shift = base_txt_shift;
	m_font_area = font_area;
	m_text = text;

	D3DXVECTOR3 save_first_pos = first_pos;

	unsigned int text_size = wcslen(m_text);

	D3DXVECTOR2 text_shift = { m_base_text_shift * (text_size - 1) + m_base_size ,m_base_size};

	D3DXVECTOR2 poly_size = { m_font_area.x / text_shift.x,m_font_area.y / text_shift.y };

	for (unsigned int i = INT_ZERO; i < text_size; ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, m_base_size, thickness, idx, m_text[i]);
		font->SetColor(col);

		m_Font.push_back(font);

		first_pos.x += m_base_text_shift;
		if (first_pos.x >=save_first_pos.x + font_area.x)
		{
			first_pos.x = save_first_pos.x;
			first_pos.y += m_base_text_shift;
		}
	}
}

//================================
// 登録(文字の長さからサイズや長さ調整)
//================================
void My::CFontManager::RegistAdjustFontSize(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col, bool isLinebrake)
{
	m_base_size = base_size;
	m_base_text_shift = base_txt_shift;
	m_font_area = font_area;
	m_text = text;
	m_isLineBreak = isLinebrake;

	unsigned int text_size = wcslen(m_text);

	// 横幅に収めるための文字サイズと文字間の調整
	float adjusted_base_size = base_size;
	float adjusted_shift_x = base_txt_shift;

	//何文字でエリアが埋まるか
	int num_text = m_font_area.x / m_base_size;

	if (!isLinebrake && text_size > num_text)
	{
		float total_width = base_txt_shift * (text_size - 1) + base_size;
		if (total_width > font_area.x)
		{
			float scale = font_area.x / total_width;
			adjusted_base_size = base_size * scale;
			adjusted_shift_x = base_txt_shift * scale;
			m_base_text_shift = adjusted_shift_x;
		}
	}

	D3DXVECTOR2 text_shift = { m_base_text_shift * (text_size - 1) + m_base_size ,m_base_size };

	D3DXVECTOR3 text_pos = first_pos;

	// 文字数を合わせた分のサイズ
	float total_wide = text_shift.x * m_Font.size();
	switch (m_align)
	{
	case RIGHT:
		text_pos.x = (first_pos.x) + (m_font_area.x - total_wide);
		break;
	case CENTER:
		text_pos.x = (first_pos.x) + (m_font_area.x - total_wide) * HALF;
		break;
	case LEFT:
		text_pos.x = (first_pos.x);
		break;
	default:
		assert(false);
		break;
	}
	D3DXVECTOR3 save_first_pos = text_pos;
	for (unsigned int i = INT_ZERO; i < text_size; ++i)
	{
		CFont* font = nullptr;

		font = CFont::Create(text_pos, adjusted_base_size, thickness, idx, m_text[i]);
		font->SetColor(col);

		m_Font.push_back(font);

		bool retflag;
		TextShift(text_pos, text_shift, save_first_pos);
	}
}

//================================
// テキスト動かし
//================================
void My::CFontManager::TextShift(D3DXVECTOR3& text_pos, D3DXVECTOR2& text_shift, D3DXVECTOR3& save_first_pos)
{
	text_pos.x += text_shift.x;

	if (!m_isLineBreak)
	{
		return;
	}
	if (text_pos.x > save_first_pos.x)
	{
		text_pos.x = (save_first_pos.x + m_base_text_shift);
		text_pos.y += m_base_text_shift;
	}
}

//================================
// テキスト設定
//================================
void My::CFontManager::SetText(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col)
{
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();

	Regist(text, first_pos, font_area, base_size, base_txt_shift, thickness, idx, col);
}

//================================
// テキスト設定
//================================
void My::CFontManager::SetTextAdjustFontSize(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col, bool isLinebrake)
{
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();

	RegistAdjustFontSize(text, first_pos, font_area, base_size, base_txt_shift, thickness, idx, col,isLinebrake);
}

void My::CFontManager::UpdatePos(D3DXVECTOR3 first_pos)
{
	D3DXVECTOR3 text_pos = first_pos;

	// 文字数を合わせた分のサイズ
	float total_wide = m_base_text_shift * m_Font.size();

	D3DXVECTOR2 text_shift = { (float)m_base_text_shift ,(float)m_base_text_shift };

	switch (m_align)
	{
	case RIGHT:
		//右詰め
		text_pos.x = (first_pos.x) + fabsf(m_font_area.x - total_wide);
		break;
	case CENTER:
		//中央ぞろえ
		text_pos.x = (first_pos.x) + fabsf(m_font_area.x - total_wide) * HALF;
		break;
	case LEFT:
		//左詰め
		text_pos.x = first_pos.x;
		break;
	default:
		assert(false);
		break;
	}
	D3DXVECTOR3 save_first_pos = text_pos;

	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->SetPos(text_pos);
		TextShift(text_pos, text_shift, save_first_pos);
	}
}

//================================
// 全削除
//================================
void My::CFontManager::Release()
{
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();
}
