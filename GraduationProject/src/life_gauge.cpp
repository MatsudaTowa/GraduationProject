//=====================================================================================================================================
//
// 体力表示用ポリゴン「life_gauge.cpp」
// Author : 笹生一颯
//
//=====================================================================================================================================
#include "life_gauge.h"
#include "life_frame.h"
#include "life_changer.h"
namespace
{
	constexpr int SEGMENT_NUM = 100;//分割数
	constexpr float LIFE_75 = 0.75;//7.5割
	constexpr float LIFE_50 = 0.5;//5割
	constexpr float LIFE_25 = 0.25;//2.5割
}

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
	//インスタンス取得
	CLife_changer* pLife = CLife_changer::GetInstance();

	//割合計算
	float fRaito = pLife->GetLifeRaito();

	//表示していいポリゴンの計算（1つで1％）
	m_nDrawSegNum = SEGMENT_NUM - (fRaito * SEGMENT_NUM);

	//割合に応じた色変化
	if (fRaito > LIFE_75)
	{//水色
		ChangeColor({ FLOAT_ZERO, FLOAT_ONE, FLOAT_ONE, FLOAT_ONE });
	}

	else if (fRaito <= LIFE_75 && fRaito >= LIFE_50)
	{//緑色
		ChangeColor({ FLOAT_ZERO, FLOAT_ONE, FLOAT_ZERO, FLOAT_ONE });
	}

	else if (fRaito <= 0.5 && fRaito >= LIFE_25)
	{//黄色
		ChangeColor({ FLOAT_ONE, FLOAT_ONE, FLOAT_ZERO, FLOAT_ONE });
	}

	else if (fRaito < LIFE_25)
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
	float anglestep = static_cast<float>(D3DX_PI * 2.0 / m_nTrapeSeg);
	//これも加えて12時から始まるように
	float angleoffset = -D3DX_PI / 2.0f;

	//これが何番目の物でどのくらいの角度の範囲があればいいかを計算
	//float angle_S = anglestep * m_nTrapeSegNum;
	//float angle_E = anglestep * (m_nTrapeSegNum + 1);（3時の方向に切れ目）

	float angle_S = anglestep * m_nTrapeSegNum + angleoffset;
	float angle_E = anglestep * (m_nTrapeSegNum + 1) + angleoffset;
	 
	//内外半径の設定
	float rad_I = m_fTrapeRad - 30.0f;//内側（とりあえず厚みは20）
	float rad_O = m_fTrapeRad;//外側

	//設定
	SetTrapeVtx_Radial(pos, col, rad_I, rad_O, angle_S, angle_E);
}

//===============================================================================================
// 体力UIまとめて生成
//===============================================================================================
void My::CLife_Gauge::CreateLifeUI(float GameHP, float Rad)
{
	//インスタンス取得
	CLife_changer* pLife = CLife_changer::GetInstance();

	CLife_frame::Create({ 140.0f,600.0f,0.0f }, {Rad * 1.25f,Rad *1.25f});

	for (int i = 0; i < SEGMENT_NUM; i++)
	{
		CLife_Gauge::Create_S({ 140.0f,600.0f,0.0f }, Rad, SEGMENT_NUM, i);
	}
	pLife->DefMax(GameHP);
}

//------------------------------------------------------------------------------------------------------------------------//
//===============================================================================================
// 
//===============================================================================================