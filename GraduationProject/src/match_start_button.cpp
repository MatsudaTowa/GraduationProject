//=============================================
//
//マッチ開始ボタン[match_start_button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "match_start_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CMatchStartButton::CMatchStartButton(int nPriority):CObject2D(nPriority),
m_font_manager(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CMatchStartButton::~CMatchStartButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CMatchStartButton::Init()
{
	CObject2D::Init();

	SetColor(COLOR_WHITE);

	if (m_font_manager == nullptr)
	{
		m_font_manager = new CFontManager;
		m_font_manager->Init();

		const wchar_t* txt = L"START";
		D3DXVECTOR3 pos = GetPos();
		m_font_manager->Regist(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}

	SetVtx();

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CMatchStartButton::Uninit()
{
	if (m_font_manager != nullptr)
	{
		m_font_manager->Uninit();
		delete m_font_manager;
		m_font_manager = nullptr;
	}

	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CMatchStartButton::Update()
{
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CMatchStartButton::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CMatchStartButton* My::CMatchStartButton::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CMatchStartButton* pButton = new CMatchStartButton;
	if (pButton == nullptr) { return nullptr; }
	pButton->SetPos(pos);
	pButton->SetSize(size);

	pButton->Init();
	return pButton;
}
