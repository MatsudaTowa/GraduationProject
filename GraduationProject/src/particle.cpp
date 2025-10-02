//=============================================
//
//パーティクル[particle.cpp]
//Author Matsuda Towa
//
//=============================================
#include "particle.h"

//=============================================
// コンストラクタ
//=============================================
My::CParticle::CParticle(int nPriority):CBillboard_Anim(nPriority),
m_life(INT_ZERO),
m_startlife(INT_ZERO),
m_startcolor(VEC3_RESET_ZERO),
m_move(VEC3_RESET_ZERO)
{
}

//=============================================
// デストラクタ
//=============================================
My::CParticle::~CParticle()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CParticle::Init()
{
	CBillboard_Anim::Init();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CParticle::Uninit()
{
	CBillboard_Anim::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CParticle::Update()
{
	--m_life;
	D3DXVECTOR3 pos = GetPos();
	pos += m_move;
	SetPos(pos);

	D3DXVECTOR3 size = GetSize();

	//現在のサイズに応じてアルファ値変更
	float current_alpha = (m_life * m_startcolor.a) / m_startlife;
	D3DXCOLOR col = GetColor();
	col.a = current_alpha;

	//代入
	SetColor(col);
	SetSize(size);

	if (m_life <= 0)
	{
		Uninit();
	}
}

//=============================================
// 描画
//=============================================
void My::CParticle::Draw()
{
	CBillboard_Anim::Draw();
}
