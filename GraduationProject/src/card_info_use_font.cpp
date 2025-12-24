//=============================================
//
//カード情報の背景[card_info_usefont.cpp]
//Author Matsuda Towa
//
//=============================================
#include "card_info_use_font.h"

//=============================================
// コンストラクタ
//=============================================
My::CCardInfoUseFont::CCardInfoUseFont(int nPriority):CCardInfoBase(nPriority),
m_pFontManager(nullptr),
m_offsetpos(VEC3_RESET_ZERO)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCardInfoUseFont::~CCardInfoUseFont()
{
	if (m_pFontManager != nullptr)
	{
		m_pFontManager->Uninit();
		delete m_pFontManager;
		m_pFontManager = nullptr;
	}
}

//=============================================
// 
//=============================================
HRESULT My::CCardInfoUseFont::Init()
{
	CObject2D::Init();
	return S_OK;
}

//=============================================
// 
//=============================================
void My::CCardInfoUseFont::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 
//=============================================
void My::CCardInfoUseFont::Update()
{
	//if (m_pFontManager != nullptr)
	//{
	//	D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換

	//	offset_pos.x += m_offsetpos.x;
	//	offset_pos.y += m_offsetpos.y;

	//	m_pFontManager->UpdatePos(offset_pos);
	//	std::vector<CFont*> list = m_pFontManager->GetList();
	//	for (auto& itr : list)
	//	{
	//		if (itr == nullptr) { continue; }

	//		itr->SetisDraw(GetisDraw());
	//	}
	//}
}

//=============================================
// 
//=============================================
void My::CCardInfoUseFont::Draw()
{
	CObject2D::Draw();
}
