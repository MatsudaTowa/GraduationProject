//=============================================
//
//準備できてるかのUI「ready_UI.cpp」
// Author松田永久
//
//=============================================
#include "ready_UI.h"

//=============================================
// コンストラクタ
//=============================================
My::CReadyUI::CReadyUI():m_pReady_txt(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CReadyUI::~CReadyUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CReadyUI::Init(CActiveSceneCharacter* character)
{
	if (m_pReady_txt == nullptr)
	{
		D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換
		m_pReady_txt = CReadyTxt::Create(screen_pos, { 100.0f,40.0f });
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CReadyUI::Uninit()
{
	if (m_pReady_txt != nullptr)
	{
		m_pReady_txt->Uninit();
		m_pReady_txt = nullptr;
	}
}

//=============================================
// 更新
//=============================================
void My::CReadyUI::Update()
{
}

//=============================================
// UI設定
//=============================================
void My::CReadyUI::SetCurrentReady_UI(CActiveSceneCharacter* character)
{
	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), character->GetPos()); //スクリーン座標に変換
	if (m_pReady_txt != nullptr)
	{
		m_pReady_txt->SetPos(screen_pos);
	}
}

//=============================================
// リセット
//=============================================
void My::CReadyUI::Reset()
{
}
