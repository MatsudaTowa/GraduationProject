//===========================================================================================================================================================
// 
// card_frame_name.cpp [card_frame_name.cpp]
// Author : matsuda towa
// 
//===========================================================================================================================================================
#include "card_frame_name.h"

namespace
{
	D3DXVECTOR3 OFFSET = {-15.0f,9.0f,0.0f };
}

//===========================================================================================================================================================
// コンストラクタ
//===========================================================================================================================================================
My::CCardFrameName::CCardFrameName(int nPriority):CCardFrameUseFont(nPriority)
{
}

//===========================================================================================================================================================
// 初期化
//===========================================================================================================================================================
HRESULT My::CCardFrameName::Init()
{
	CCardFrameUseFont::Init();

	CFontManager* pFontmanager = GetFontManager();
	if (pFontmanager == nullptr)
	{
		pFontmanager = new CFontManager(CFontManager::CENTER);
		pFontmanager->Init();
		SetFontManager(pFontmanager); 
	}
	if (pFontmanager != nullptr)
	{
		D3DXVECTOR3 offset_pos = GetPos(); 
		D3DXVECTOR2 size = GetSize();
		std::string card_name = GetCard()->GetBaseStatus().name;
		std::wstring wtxt;
		int len = MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, nullptr, 0);
		wtxt.resize(len - 1);
		MultiByteToWideChar(CP_UTF8, 0, card_name.c_str(), -1, &wtxt[0], len);
		D3DXVECTOR3 offset = { OFFSET.x ,OFFSET.y,OFFSET.z };
		offset_pos.x += offset.x;
		offset_pos.y += offset.y;

		pFontmanager->RegistAdjustFontSize(wtxt.c_str(), offset_pos, { 50.0,50.0f}, 10.0f, 12.0f, 0, 5, COLOR_BLACK,false);
		SetOffSetPos({ offset });
	}
	return S_OK;
}

//===========================================================================================================================================================
// 終了
//===========================================================================================================================================================
void My::CCardFrameName::Uninit()
{
	CCardFrameUseFont::Uninit();
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::CCardFrameName::Update()
{
	CCardFrameUseFont::Update();
}
