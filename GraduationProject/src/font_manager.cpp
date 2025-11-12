//================================
//
//フォントのマネージャー[font_manager.cpp]
//Author:towa matsuda
// 
//================================
#include "font_manager.h"

namespace
{

}

//================================
// コンストラクタ
//================================
My::CFontManager::CFontManager()
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
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();
}

//================================
// 登録 TODO;文字列の長さに応じてフォントサイズやずらす値を決めたいよね
//================================
void My::CFontManager::Regist(const wchar_t* text, D3DXVECTOR3 first_pos, float size, float txt_shift, int thickness, int idx)
{
	for (int i = INT_ZERO; i < wcslen(text); ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, size, thickness, idx, text[i]);
		m_Font.push_back(font);

		first_pos.x += txt_shift;
	}
}

void My::CFontManager::SetText(const wchar_t* text, D3DXVECTOR3 first_pos, float size, float txt_shift, int thickness, int idx)
{
	for (auto& itr : m_Font)
	{
		if (itr == nullptr) { continue; }
		itr->Uninit();
		itr = nullptr;
	}
	m_Font.clear();

	for (int i = INT_ZERO; i < wcslen(text); ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, size, thickness, idx, text[i]);
		m_Font.push_back(font);

		first_pos.x += txt_shift;
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
