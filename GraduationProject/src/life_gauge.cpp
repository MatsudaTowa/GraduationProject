//=====================================================================================================================================
//
// 体力表示用ポリゴン「life_gauge.cpp」
// Author : 笹生一颯
//
//=====================================================================================================================================
#include "life_gauge.h"
#include "life_frame.h"
namespace
{
	constexpr int SEGMENT_NUM = 100;
	constexpr float LIFE_75 = 0.75;
	constexpr float LIFE_50 = 0.5;
	constexpr float LIFE_25 = 0.25;
}

float My::CLife_Gauge::m_fLife = FLOAT_ZERO;
float My::CLife_Gauge::m_fLife_Max = FLOAT_ZERO;
float My::CLife_Gauge::m_fRaito = FLOAT_ZERO;

//===============================================================================================
//コンストラクタ
//===============================================================================================
My::CLife_Gauge::CLife_Gauge(int nPriority) : CObject2D_Trapezoid(nPriority),
m_fTrapeRad(FLOAT_ZERO),
m_nTrapeSeg(INT_ZERO),
m_nTrapeSegNum(INT_ZERO),
m_nDrawSegNum(INT_ZERO)
{
}

//===============================================================================================
//デストラクタ
//===============================================================================================
My::CLife_Gauge::~CLife_Gauge()
{
}

//===============================================================================================
//初期化
//===============================================================================================
HRESULT My::CLife_Gauge::Init()
{
	SetVtx_S();

	return S_OK;
}

//===============================================================================================
//終了
//===============================================================================================
void My::CLife_Gauge::Uninit()
{
	CObject2D::Uninit();
}

//===============================================================================================
//更新
//===============================================================================================
void My::CLife_Gauge::Update()
{
	//ここに入れたら複数回更新するの忘れてた。（すっごい減るからバグを疑った）
	//【デバッグ専用】ダメージ反映・回復反映・最大変化
	//インプット取得
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetKeyboard();

	//HP増加
	if (pKeyboard->GetTrigger(DIK_L))
	{
		ApplyHeal(0.01f);
	}

	//HP減少
	else if (pKeyboard->GetTrigger(DIK_K))
	{
		ApplyDamage(0.01f);
	}

	////最大増加
	//else if (pKeyboard->GetTrigger(DIK_8))
	//{
	//	IncreaseMax(FLOAT_ONE);
	//}

	////最大減少
	//else if (pKeyboard->GetTrigger(DIK_9))
	//{
	//	DecreaseMax(FLOAT_ONE);
	//}


	//割合計算
	m_fRaito = (m_fLife / m_fLife_Max);

	//表示していいポリゴンの計算（１つで5％）
	m_nDrawSegNum = SEGMENT_NUM - (m_fRaito * SEGMENT_NUM);

	//割合に応じた色変化
	if (m_fRaito > LIFE_75)
	{//水色
		ChangeColor({ FLOAT_ZERO, FLOAT_ONE, FLOAT_ONE, FLOAT_ONE });
	}

	else if (m_fRaito <= LIFE_75 && m_fRaito >= LIFE_50)
	{//緑色
		ChangeColor({ FLOAT_ZERO, FLOAT_ONE, FLOAT_ZERO, FLOAT_ONE });
	}

	else if (m_fRaito <= 0.5 && m_fRaito >= LIFE_25)
	{//黄色
		ChangeColor({ FLOAT_ONE, FLOAT_ONE, FLOAT_ZERO, FLOAT_ONE });
	}

	else if (m_fRaito < LIFE_25)
	{//赤色
		ChangeColor({ FLOAT_ONE, FLOAT_ZERO, FLOAT_ZERO, FLOAT_ONE });
	}
}

//===============================================================================================
//描画
//===============================================================================================
void My::CLife_Gauge::Draw()
{
	if (m_nTrapeSegNum >= m_nDrawSegNum)
	{
		CObject2D::Draw();
	}
}

//===============================================================================================
//生成（四角）
//===============================================================================================
My::CLife_Gauge* My::CLife_Gauge::Create_S(D3DXVECTOR3 Pos, float Rad, int Segment, int Num)
{
	CLife_Gauge* pLife = nullptr;
	pLife = new CLife_Gauge(12);
	pLife->m_fTrapeRad = Rad;
	pLife->m_nTrapeSeg = Segment;
	pLife->m_nTrapeSegNum = Num;
	pLife->SetPos(Pos);//中心位置
	pLife->Init();

	return pLife;
}

//===============================================================================================
//台形の描画
//===============================================================================================
void My::CLife_Gauge::SetVtx_S()
{
	CObject2D::Init();
	D3DXVECTOR3 pos = GetPos();
	D3DXCOLOR col = GetColor();
	col = { FLOAT_ZERO, FLOAT_ONE, FLOAT_ONE, FLOAT_ONE };

	//分割した時の角度
	float anglestep = D3DX_PI * 2.0 / m_nTrapeSeg;
	//これも加えて12時から始まるように
	float angleoffset = -D3DX_PI / 2.0f;

	//これが何番目の物でどのくらいの角度の範囲があればいいかを計算
	//float angle_S = anglestep * m_nTrapeSegNum;
	//float angle_E = anglestep * (m_nTrapeSegNum + 1);（3時の方向に切れ目）

	float angle_S = anglestep * m_nTrapeSegNum + angleoffset;
	float angle_E = anglestep * (m_nTrapeSegNum + 1) + angleoffset;
	 
	//内外半径の設定
	float rad_I = m_fTrapeRad - 50.0f;//内側（とりあえず厚みは20）
	float rad_O = m_fTrapeRad;//外側

	//設定
	SetTrapeVtx_Radial(pos, col, rad_I, rad_O, angle_S, angle_E);
}

//===============================================================================================
// 体力UIまとめて生成
//===============================================================================================
void My::CLife_Gauge::CreateLifeUI(float GameHP)
{
	CLife_frame::Create({ 260.0f,600.0f,0.0f }, {245,120});

	for (int i = 0; i < SEGMENT_NUM; i++)
	{
		CLife_Gauge::Create_S({ 137,600,0 }, 110, SEGMENT_NUM, i);
	}
	CLife_Gauge::DefMax(GameHP);
}

//------------------------------------------------------------------------------------------------------------------------//
//===============================================================================================
// 
//===============================================================================================