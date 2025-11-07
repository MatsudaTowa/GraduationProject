#include "target_arrow.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\TargetArrow.png";
}

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CTargetArrow::CTargetArrow(int nPriority):CObject2D(nPriority)
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CTargetArrow::~CTargetArrow()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CTargetArrow::Init()
{
	// 位置設定
	SetPos({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f ,0.0f });

	// サイズの倍率
	float size_mag = 1.7f;

	// サイズ設定
	SetSize({ 10* size_mag,10* size_mag });

	// カラー設定
	SetColor(COLOR_WHITE);

	//テクスチャ登録
	CTexture* pTexture = GET_TEXTURE;
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));	// テクスチャ設定

	//頂点設定
	SetVtx();
	SetAngleLength();


	CObject2D::Init();

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CTargetArrow::Uninit()
{
	CObject2D::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CTargetArrow::Update()
{
	
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CTargetArrow::Draw()
{
	CObject2D::Draw();
}

//===========================================================================================================
// 生成
//===========================================================================================================
My::CTargetArrow* My::CTargetArrow::Create(int attacker, int target)
{
	CTargetArrow* pTA = new CTargetArrow;

	// 矢印の角度を算出
	pTA->m_attacker = SetTargetPos(pTA->m_attacker, attacker);	// 攻撃者の位置
	pTA->m_target = SetTargetPos(pTA->m_target, target);		// 被攻撃者の位置

	pTA->Init(); 

	return pTA;
}

D3DXVECTOR2 My::CTargetArrow::SetTargetPos(D3DXVECTOR2& target, int targetnum)
{
	switch (targetnum)
	{
	case CInputMouse::UP:
		target = up;
		break;

	case CInputMouse::DOWN:
		target = down;
		break;

	case CInputMouse::LEFT:
		target = right;
		break;

	case CInputMouse::RIGHT:
		target = left;
		break;
	}

	return target;
}

//===========================================================================================================
// 角度と長さを設定する
//===========================================================================================================
void My::CTargetArrow::SetAngleLength()
{
	// 角度の設定
	D3DXVECTOR3 rot = VEC3_RESET_ZERO;	// 初期化
	float angle = FLOAT_ZERO;	// 初期化
	angle = atan2f(GetSize().x,GetSize().y);	// 設定
	rot.z = atan2f(m_target.y - m_attacker.y, m_target.x - m_attacker.x);	// 設定
	
	// 向きを設定
	rot.z += D3DX_PI*0.5f;
	SetRot(rot);

	// サイズを代入
	float dx = GetSize().x;
	float dy = GetSize().y;

	// 長さを設定
	float length = FLOAT_ZERO;	// 初期化
	length = sqrtf(dx * dx + dy * dy) * 0.5f;	// 設定

	//頂点設定
	SetVtx(angle, length);
}
