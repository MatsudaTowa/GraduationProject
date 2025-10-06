//=============================================
//
//���C�tUI�����ulife_UI.cpp�v
// Author���c�i�v
//
//=============================================
#include "life_UI.h"

namespace
{
	const int NUM_DIGIT = 1;										//����
	const float DIGIT_SHIFT = 30.0f;								//�����Ƃɍ��W�����炷
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(20.0f, 30.0f);		//�����̃T�C�Y
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CLife_UI::CLife_UI():
	m_num_pos(VEC3_RESET_ZERO)
{
	m_pNumber.clear();
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CLife_UI::~CLife_UI()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CLife_UI::Init()
{
	for (int nCnt = 0; nCnt < NUM_DIGIT; nCnt++)
	{
		CNumber_2D* pNumber = CNumber_2D::Create(m_num_pos, NUMBER_SIZE, CNumber_2D::TEX_001);
		m_pNumber.push_back(pNumber);

		//���W�����炷
		m_num_pos.x -= DIGIT_SHIFT;
	}
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CLife_UI::Uninit()
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
// ����
//=============================================
My::CLife_UI* My::CLife_UI::Create(D3DXVECTOR3 pos)
{
	CLife_UI* ui = new CLife_UI;
	if(ui == nullptr)
	{
		return nullptr;
	}
	ui->SetNumPos(pos);
	ui->Init();
	return ui;
}

//=============================================
// �����ݒ�
//=============================================
void My::CLife_UI::SetNumber(int life)
{
	//�����擾
	int num_digit = m_pNumber.size();

	std::vector<int> pos_texU;

	//���݌v�Z���Ă錅
	int nDigit = 1;
	int nCnt;
	for (nCnt = 0; nCnt < num_digit; nCnt++)
	{
		pos_texU.push_back(life / nDigit % 10);

		//����i�߂�
		nDigit *= 10;
	}

	EraseDigit(pos_texU, num_digit);
	AddDigit(pos_texU, num_digit, life);

	for (nCnt = num_digit - 1; nCnt >= 0; nCnt--)
	{
		//�e�N�X�`���̍��W�v�Z�p�ϐ�
		float fMinTexU, fMaxTexU;

		fMinTexU = pos_texU[nCnt] * 0.1f;
		fMaxTexU = fMinTexU + 0.1f;

		m_pNumber[nCnt]->SetNumber(fMinTexU, fMaxTexU, COLOR_WHITE);
	}

	pos_texU.clear();
}

//=============================================
// ���ǉ�
//=============================================
void My::CLife_UI::AddDigit(std::vector<int>& pos_texU, int& num_digit, int life)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//���𑝂₷
		if (num_digit <= 0 || life <= 0)
		{
			return;
		}
		CNumber_2D* pNumber;

		pNumber = CNumber_2D::Create(m_num_pos, NUMBER_SIZE, 1);
		//���W�����炷
		m_num_pos.x -= DIGIT_SHIFT;
		m_pNumber.push_back(pNumber);

		++num_digit;

		pos_texU.clear();
		int nDigit = 1;
		for (int nCnt = 0; nCnt < num_digit; nCnt++)
		{
			pos_texU.push_back(life / nDigit % 10);

			//����i�߂�
			nDigit *= 10;
		}
	}
}

//=============================================
// ���폜
//=============================================
void My::CLife_UI::EraseDigit(std::vector<int> pos_texU, int& num_digit)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//�������炷
		if (num_digit - 1 == 0 )
		{
			return;
		}
		//���W�����炷
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
