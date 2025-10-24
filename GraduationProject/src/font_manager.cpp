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
// 登録
//================================
void My::CFontManager::Regist(StringData font, D3DXVECTOR3 first_pos)
{
	
	//敵の情報を登録
	m_apManager.push_back(font);
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
