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
m_thickness()
{
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
// 登録 TODO;文字列の長さに応じてフォントサイズやずらす値を決めたいよね
//================================
void My::CFontManager::Regist(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col)
{
	//基準のサイズを設定
	m_base_size = base_size;
	m_base_text_shift = base_txt_shift;
	m_font_area = font_area;
	D3DXVECTOR3 save_first_pos = first_pos;

	unsigned int text_size = wcslen(text);

	D3DXVECTOR2 text_shift = { m_base_text_shift * (text_size - 1) + m_base_size ,m_base_size};

	D3DXVECTOR2 poly_size = { m_font_area.x / text_shift.x,m_font_area.y / text_shift.y };

	for (unsigned int i = INT_ZERO; i < text_size; ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, m_base_size, thickness, idx, text[i]);
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

void My::CFontManager::SetText(const wchar_t* text, D3DXVECTOR3 first_pos, D3DXVECTOR2 font_area, float base_size, float base_txt_shift, int thickness, int idx, D3DXCOLOR col)
{
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();

	//基準のサイズを設定
	m_base_size = base_size;
	m_base_text_shift = base_txt_shift;
	m_font_area = font_area;
	D3DXVECTOR3 save_first_pos = first_pos;

	unsigned int text_size = wcslen(text);

	D3DXVECTOR2 text_shift = { m_base_text_shift * (text_size - 1) + m_base_size ,m_base_size };

	D3DXVECTOR2 poly_size = { m_font_area.x / text_shift.x,m_font_area.y / text_shift.y };

	for (unsigned int i = INT_ZERO; i < text_size; ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, m_base_size, thickness, idx, text[i]);
		font->SetColor(col);

		m_Font.push_back(font);

		first_pos.x += m_base_text_shift;
		if (first_pos.x >= save_first_pos.x + font_area.x)
		{
			first_pos.x = save_first_pos.x;
			first_pos.y += m_base_text_shift;
		}
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
