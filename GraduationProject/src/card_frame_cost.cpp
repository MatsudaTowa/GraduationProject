//===========================================================================================================================================================
// 
// card_frame_base.cppのヘッダー [card_frame_cost.cpp]
// Author : souma umeda
// 
//===========================================================================================================================================================
#include "card_frame_cost.h"
#include <format>

//===========================================================================================================================================================
// コンストラクタ
//===========================================================================================================================================================
My::FrameCost::FrameCost(int nPriority):CCardFrame(nPriority),
m_pFontManager(nullptr)
{
}

//===========================================================================================================================================================
// 初期化
//===========================================================================================================================================================
HRESULT My::FrameCost::Init()
{
	CCardFrame::Init();
	if (m_pFontManager == nullptr)
	{
		m_pFontManager = new CFontManager;
		m_pFontManager->Init();
		std::wstring wtxt = std::format(L"{}", GetCard()->GetBaseStatus().nCost);
		D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //スクリーン座標に変換
		const wchar_t* txt = wtxt.c_str();
		screen_pos.y -= 80.0f;
		screen_pos.x -= 100.0f;
		m_pFontManager->Regist(txt, screen_pos, { 800.0f,100.0f }, 25.0f, 10.0f, 0, 5, COLOR_BLACK);
	}
	return S_OK;
}

//===========================================================================================================================================================
// 終了
//===========================================================================================================================================================
void My::FrameCost::Uninit()
{
	if (m_pFontManager != nullptr)
	{
		m_pFontManager->Uninit();
		delete m_pFontManager;
		m_pFontManager = nullptr;
	}
	CCardFrame::Uninit();
}

//===========================================================================================================================================================
// 更新
//===========================================================================================================================================================
void My::FrameCost::Update()
{
	CCardFrame::Update();

	if (m_pFontManager != nullptr)
	{
	 	std::vector<CFont*> list = m_pFontManager->GetList();
		for (auto& itr : list)
		{
			if (itr == nullptr) { continue; }
			D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), GetPos()); //スクリーン座標に変換
			screen_pos.y -= 80.0f;
			screen_pos.x -= 40.0f;
			itr->SetPos(screen_pos);
			// 山札時はコストを表示しないように TODO:今後はここの条件式を見直す必要あり
			if (GetCard()->GetCurrentZone() != CCard::DECK) 
			{ 
				itr->SetisDraw(true);  
			}
			else if (GetCard()->GetCurrentZone() == CCard::DECK)
			{
				itr->SetisDraw(false);
			}
		}
	}
}
