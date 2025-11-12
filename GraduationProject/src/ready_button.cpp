//=============================================
//
//準備できているかの切り替えボタン[ready_button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "ready_button.h"

//=============================================
// コンストラクタ
//=============================================
My::CReadyButton::CReadyButton(int nPriority):CObject2D(nPriority),
m_font_manager(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CReadyButton::~CReadyButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CReadyButton::Init()
{
	CObject2D::Init();

	SetColor(COLOR_WHITE);

	if (m_font_manager == nullptr)
	{
		m_font_manager = new CFontManager;
		m_font_manager->Init();

		const wchar_t* txt = L"READY";
		D3DXVECTOR3 pos = GetPos();
		m_font_manager->Regist(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}

	SetVtx();

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CReadyButton::Uninit()
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
void My::CReadyButton::Update()
{
#ifdef _DEBUG
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_RIGHT))
	{
		const wchar_t* txt = L"CANCEL";
		D3DXVECTOR3 pos = GetPos();

		m_font_manager->SetText(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}
	if (GET_INPUT_KEYBOARD->GetTrigger(DIK_LEFT))
	{
		const wchar_t* txt = L"READY";
		D3DXVECTOR3 pos = GetPos();

		m_font_manager->SetText(txt, { pos.x - GetSize().x * 0.55f,pos.y,pos.z }, 30.0f, 35.0f, 50, 6);
	}
#endif // _DEBUG

	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CReadyButton::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CReadyButton* My::CReadyButton::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CReadyButton* pButton = new CReadyButton;
	if (pButton == nullptr) { return nullptr; }
	pButton->SetPos(pos);
	pButton->SetSize(size);
	
	pButton->Init();
	return pButton;

}
