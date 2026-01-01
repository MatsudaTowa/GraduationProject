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
m_offsetpos(VEC3_RESET_ZERO)
{
	m_FontManagerVector.clear();
}

//=============================================
// デストラクタ
//=============================================
My::CCardInfoUseFont::~CCardInfoUseFont()
{
	for (auto& itr : m_FontManagerVector)
	{
		if (itr == nullptr) { continue; }

		itr->Uninit();
		delete itr;
		itr = nullptr;		
	}
	m_FontManagerVector.clear();
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
	for (auto& itr : m_FontManagerVector)
	{
		if (itr == nullptr) { continue; }
		D3DXVECTOR3 offset_pos = GetPos(); //スクリーン座標に変換

		offset_pos.x += m_offsetpos.x;
		offset_pos.y += m_offsetpos.y;

		itr->UpdatePos(offset_pos);
		std::vector<CFont*> list = itr->GetList();
		for (auto& card_itr : list)
		{
			if (card_itr == nullptr) { continue; }

			card_itr->SetisDraw(GetisDraw());
		}
	}
	CCardInfoBase::Update();
}

//=============================================
// 
//=============================================
void My::CCardInfoUseFont::Draw()
{
	CObject2D::Draw();
}
