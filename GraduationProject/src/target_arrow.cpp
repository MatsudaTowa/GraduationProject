#include "target_arrow.h"

namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\arrow_square.png";
	const std::string TRIANGLE_TEX_NAME = "data\\TEXTURE\\arrow_triangle.png";
}

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CTargetArrow::CTargetArrow(int nPriority):CObject2D_Anim(nPriority),
m_target(VEC2_RESET_ZERO),
m_basesize(VEC2_RESET_ZERO),
m_attacker(VEC2_RESET_ZERO),
m_triangle(nullptr)
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
  	D3DXVECTOR3 pos = { m_attacker.x,m_attacker.y,0.0f };

	SetPos(pos);

	//// 位置設定
	//SetPos({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f ,0.0f });

	// サイズの倍率
	float size_mag = 5.0f;

	// サイズ設定
	SetSize({ 5* size_mag,1 });

	// ベースサイズ
	m_basesize = GetSize();

	// カラー設定
	SetColor(COLOR_WHITE);

	//テクスチャ登録
	CTexture* pTexture = GET_TEXTURE;
	CObject2D_Anim::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));	// テクスチャ設定

	//頂点設定
	SetVtx();

	SetAnimFrame(1);

	CObject2D_Anim::Init();

	// 三角生成
	if (m_triangle == nullptr)
	{
		m_triangle = new CObject2D(5);
		m_triangle->BindTexture(pTexture->GetAddress(pTexture->Regist(&TRIANGLE_TEX_NAME)));
		m_triangle->SetSize({ 50,50 });
		m_triangle->SetColor(COLOR_WHITE);
		m_triangle->SetVtx();
	}	

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CTargetArrow::Uninit()
{
	if (m_triangle != nullptr)
	{
		m_triangle->SetisDelete(true);
	}

	CObject2D_Anim::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CTargetArrow::Update()
{
	D3DXVECTOR2 size = GetSize();
	size.y += 5.0f;
	SetSize(size);

	if (GetSize().y >= MAX_SIZE)
	{
		SetSize(m_basesize);
	}

	// 角度と長さを設定
	SetAngleLength();

	// 線上に位置を設定
	SetOnTheLinePos();
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CTargetArrow::Draw()
{
	CObject2D_Anim::Draw();
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

	if (pTA->m_target.x > SCREEN_WIDTH * 0.5f)
	{
		pTA->m_attacker.x += -130.0f;
	}
	else if(pTA->m_target.x < SCREEN_WIDTH * 0.5f)
	{
		pTA->m_attacker.x += 130.0f;
	}

	pTA->Init(); 

	return pTA;
}

//===========================================================================================================
// ターゲットの
//===========================================================================================================
D3DXVECTOR2 My::CTargetArrow::SetTargetPos(D3DXVECTOR2& target, int targetnum)
{
	switch (targetnum)
	{
	case CInputMouse::UP:
		target = arrow_up;
		break;

	case CInputMouse::DOWN:
		target = arrow_down;
		break;

	case CInputMouse::LEFT:
		target = arrow_right;
		break;

	case CInputMouse::RIGHT:
		target = arrow_left;
		break;
	}

	return target;
}

//===========================================================================================================
// 線上に位置を設定する(線形補間)
//===========================================================================================================
void My::CTargetArrow::SetOnTheLinePos()
{
	// 倍率の係数
	float a = 0.55f;

	// 位置・サイズの取得
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR2 size = GetSize();

	// 計算結果
	D3DXVECTOR2 result = { 0.0f,0.0f };
	D3DXVECTOR2 result2 = { 0.0f,0.0f };

	// 割合(サイズと最大サイズ)
	float ratio = size.y / MAX_SIZE;

	// 線形補間
	result.x = std::lerp(m_attacker.x, m_target.x, ratio * -a);
	result.y = std::lerp(m_attacker.y, m_target.y, ratio *  a);

	// 線形補間
	result2.x = std::lerp(m_attacker.x, m_target.x, ratio * -1.2f);
	result2.y = std::lerp(m_attacker.y, m_target.y, ratio * 1.2f);

	// 位置の設定
	pos = { result.x, result.y, 0.0f };
	SetPos(pos);

	m_triangle->SetPos({ result2.x,result2.y,0.0f });
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
	length = sqrtf(dx * dx + dy * dy);	// 設定

	//頂点設定
	SetVtx(angle, length);

	// TODO : これはこのクラスを使うべきかぁ
	if (m_triangle != nullptr)
	{
		// サイズを代入
		float tri_dx = m_triangle->GetSize().x;
		float tri_dy = m_triangle->GetSize().y;

		float tri_angle = atan2f(m_triangle->GetSize().x, m_triangle->GetSize().y);

		// 長さを設定
		float tri_length = FLOAT_ZERO;	// 初期化
		tri_length = sqrtf(tri_dx * tri_dx + tri_dy * tri_dy);	// 設定

		m_triangle->SetVtx(tri_angle, tri_length);
		m_triangle->SetRot(rot);
	}
}
