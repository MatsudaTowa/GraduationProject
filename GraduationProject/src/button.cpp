//=============================================
//
// ボタン[button.cpp]
//Author Matsuda Towa
//
//=============================================
#include "button.h"
#include "active_scene_manager.h"

//=============================================
// コンストラクタ
//=============================================
My::CButton::CButton(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CButton::~CButton()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CButton::Init()
{
	CObject2D::Init();
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CButton::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CButton::Update()
{
	CObject2D::Update();
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CButton::Draw()
{
	CObject2D::Draw();
}

//=============================================
// マウスとの接触判定
//=============================================
bool My::CButton::ProcessMouseEvent()
{
	if (typeid(*CActiveSceneManager::GetInstance()->GetState()) == typeid(CCardCast))
	{
		return false;
	}
	return GET_COLISION->Check2DPolygonColision(GET_INPUT_MOUSE->GetMousePos(), { 3.0f,3.0f }, {GetPos().x,GetPos().y,0.0f }, GetSize());
}
