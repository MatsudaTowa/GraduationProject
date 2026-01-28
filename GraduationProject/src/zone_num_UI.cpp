//=============================================
//
//ゾーンUI処理「zone_num_UI.cpp」
// Author松田永久
//
//=============================================
#include "zone_num_UI.h"

namespace
{
	const int NUM_DIGIT = 1;										//桁数
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(15.0f, 20.0f);		//数字のサイズ
}

My::CZoneNumUI::CZoneNumUI():
m_num_pos(VEC3_RESET_ZERO)
{
	m_pNumber.clear();
}

My::CZoneNumUI::~CZoneNumUI()
{
}

HRESULT My::CZoneNumUI::Init()
{
	for (int nCnt = 0; nCnt < NUM_DIGIT; nCnt++)
	{
		CZoneNum* pNumber = CZoneNum::Create(m_num_pos, NUMBER_SIZE, CNumber_2D::TEX_000);
		m_pNumber.push_back(pNumber);
	}
	return S_OK;
}

void My::CZoneNumUI::Uninit()
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

void My::CZoneNumUI::SetNumber(int num)
{
	//桁数取得
	int num_digit = m_pNumber.size();

	std::vector<int> pos_texU;

	//現在計算してる桁
	int nDigit = 1;
	int nCnt;
	for (nCnt = 0; nCnt < num_digit; nCnt++)
	{
		pos_texU.push_back(num / nDigit % 10);

		//桁を進める
		nDigit *= 10;
	}

	EraseDigit(pos_texU, num_digit);
	AddDigit(pos_texU, num_digit, num);

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

void My::CZoneNumUI::AddDigit(std::vector<int>& pos_texU, int& num_digit, int num)
{
	//弾く条件
	if (num < 10) return;						//エナジーが10未満なら抜ける
	if (num_digit <= 0 || num_digit > 1) return;	//桁数が0以下、2桁以上なら抜ける
		CZoneNum* pNumber;

		pNumber = CZoneNum::Create(m_num_pos, NUMBER_SIZE, CNumber_2D::TEX_000);
		m_pNumber.push_back(pNumber);

		++num_digit;

		pos_texU.clear();
		int nDigit = 1;
		for (int nCnt = 0; nCnt < num_digit; nCnt++)
		{
			pos_texU.push_back(num / nDigit % 10);

			//桁を進める
			nDigit *= 10;
		}
}

void My::CZoneNumUI::EraseDigit(std::vector<int> pos_texU, int& num_digit)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//桁を減らす
		if (num_digit - 1 == 0)
		{
			return;
		}
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
