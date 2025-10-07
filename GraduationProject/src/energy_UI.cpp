//=============================================
//
//エナジーUI処理「energy_UI.cpp」
// Author松田永久
//
//=============================================
#include "energy_UI.h"

namespace
{
	const int NUM_DIGIT = 1;										//桁数
	const float DIGIT_SHIFT = 30.0f;								//桁ごとに座標をずらす
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(20.0f, 30.0f);		//数字のサイズ
}

//=============================================
// コンストラクタ
//=============================================
My::CEnergy_UI::CEnergy_UI():
	m_num_pos(VEC3_RESET_ZERO)
{
	m_pNumber.clear();
}

//=============================================
// デストラクタ
//=============================================
My::CEnergy_UI::~CEnergy_UI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CEnergy_UI::Init()
{
	for (int nCnt = 0; nCnt < NUM_DIGIT; nCnt++)
	{
		CNumber_2D* pNumber = CNumber_2D::Create(m_num_pos, NUMBER_SIZE, CNumber_2D::TEX_001);
		m_pNumber.push_back(pNumber);

		//座標をずらす
		m_num_pos.x -= DIGIT_SHIFT;
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CEnergy_UI::Uninit()
{
	for (auto& itr : m_pNumber)
	{
		if (itr == nullptr)
		{
			continue;
		}
		itr->Uninit();
		itr = nullptr;
	}
	m_pNumber.clear();
	delete this;
}

//=============================================
// 生成
//=============================================
My::CEnergy_UI* My::CEnergy_UI::Create(D3DXVECTOR3 pos)
{
	CEnergy_UI* ui = new CEnergy_UI;
	if (ui == nullptr)
	{
		return nullptr;
	}
	ui->SetNumPos(pos);
	ui->Init();
	return ui;
}

//=============================================
// エナジーの数字設定
//=============================================
void My::CEnergy_UI::SetEnergyNumber(int energy)
{
	//桁数取得
	int num_digit = m_pNumber.size();

	std::vector<int> pos_texU;

	//現在計算してる桁
	int nDigit = 1;
	int nCnt;
	for (nCnt = 0; nCnt < num_digit; nCnt++)
	{
		pos_texU.push_back(energy / nDigit % 10);

		//桁を進める
		nDigit *= 10;
	}

	EraseDigit(pos_texU, num_digit);
	AddDigit(pos_texU, num_digit, energy);

	for (nCnt = num_digit - 1; nCnt >= 0; nCnt--)
	{
		//テクスチャの座標計算用変数
		float fMinTexU, fMaxTexU;

		fMinTexU = pos_texU[nCnt] * 0.1f;
		fMaxTexU = fMinTexU + 0.1f;

		m_pNumber[nCnt]->SetNumber(fMinTexU, fMaxTexU, COLOR_WHITE);
	}

	pos_texU.clear();
}

//=============================================
// 桁追加
//=============================================
void My::CEnergy_UI::AddDigit(std::vector<int>& pos_texU, int& num_digit, int energy)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//桁を増やす
		if (num_digit <= 0 || energy <= 0)
		{
			return;
		}
		CNumber_2D* pNumber;

		pNumber = CNumber_2D::Create(m_num_pos, NUMBER_SIZE, 1);
		//座標をずらす
		m_num_pos.x -= DIGIT_SHIFT;
		m_pNumber.push_back(pNumber);

		++num_digit;

		pos_texU.clear();
		int nDigit = 1;
		for (int nCnt = 0; nCnt < num_digit; nCnt++)
		{
			pos_texU.push_back(energy / nDigit % 10);

			//桁を進める
			nDigit *= 10;
		}
	}
}

//=============================================
// 桁削除
//=============================================
void My::CEnergy_UI::EraseDigit(std::vector<int> pos_texU, int& num_digit)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//桁を減らす
		if (num_digit - 1 == 0)
		{
			return;
		}
		//座標をずらす
		m_num_pos.x += DIGIT_SHIFT;
		CNumber_2D* pNumber = m_pNumber[num_digit - 1];
		pNumber->Uninit();
		pNumber = nullptr;

		m_pNumber.erase(m_pNumber.end() - 1);

		--num_digit;

		if (num_digit <= 0)
		{
			return;
		}
	}
}
