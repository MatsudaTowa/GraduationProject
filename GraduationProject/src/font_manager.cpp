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
	m_apManager.clear();
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
	for (auto& i : m_apManager)
	{
		for (auto& j : i.pFont)
		{
			if (j == nullptr) { continue; }
			j->Uninit();
			j = nullptr;
		}
		i.text = NULL;
	}
	m_apManager.clear();
}

//================================
// 登録 TODO;文字列の長さに応じてフォントサイズやずらす値を決めたいよね
//================================
void My::CFontManager::Regist(const wchar_t* text, D3DXVECTOR3 first_pos, float size, float txt_shift, int thickness, int idx)
{
	StringData data;
	std::vector<CFont*> font_list;
	font_list.clear();

	for (int i = INT_ZERO; i < wcslen(text); ++i)
	{
		CFont* font = nullptr;
		font = CFont::Create(first_pos, size, thickness, idx, text[i]);
		font_list.push_back(font);

		first_pos.x += txt_shift;
	}

	data.text = text;
	data.pFont = font_list;

	//敵の情報を登録
	m_apManager.push_back(data);
}

//================================
// 全削除
//================================
void My::CFontManager::Release()
{
	for (auto& i : m_apManager)
	{
		for (auto& j : i.pFont)
		{
			if (j == nullptr) { continue; }
			j->Uninit();
			j = nullptr;
		}
		i.text = NULL;
	}
	m_apManager.clear();
}

//================================
// 削除　NOTE:必ずフォントのUninitを呼んでから
//================================
void My::CFontManager::Remove(StringData font)
{
	//サイズが0なら抜ける
	if (m_apManager.size() == 0)
	{
		return;
	}

	// textとポインタが一致するものを削除
	m_apManager.remove_if([&](const StringData& data)
	{
		return data.text == font.text;
	});
}
