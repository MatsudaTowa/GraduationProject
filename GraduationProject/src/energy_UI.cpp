//=============================================
//
//�G�i�W�[UI�����uenergy_UI.cpp�v
// Author���c�i�v
//
//=============================================
#include "energy_UI.h"

namespace
{
	const int NUM_DIGIT = 1;										//����
	const float DIGIT_SHIFT = 30.0f;								//�����Ƃɍ��W�����炷
	const D3DXVECTOR2 NUMBER_SIZE = D3DXVECTOR2(20.0f, 30.0f);		//�����̃T�C�Y
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CEnergy_UI::CEnergy_UI():
	m_num_pos(VEC3_RESET_ZERO)
{
	m_pNumber.clear();
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CEnergy_UI::~CEnergy_UI()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CEnergy_UI::Init()
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
// ����
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
// �G�i�W�[�̐����ݒ�
//=============================================
void My::CEnergy_UI::SetEnergyNumber(int energy)
{
	//�����擾
	int num_digit = m_pNumber.size();

	std::vector<int> pos_texU;

	//���݌v�Z���Ă錅
	int nDigit = 1;
	int nCnt;
	for (nCnt = 0; nCnt < num_digit; nCnt++)
	{
		pos_texU.push_back(energy / nDigit % 10);

		//����i�߂�
		nDigit *= 10;
	}

	EraseDigit(pos_texU, num_digit);
	AddDigit(pos_texU, num_digit, energy);

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
void My::CEnergy_UI::AddDigit(std::vector<int>& pos_texU, int& num_digit, int energy)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//���𑝂₷
		if (num_digit <= 0 || energy <= 0)
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
			pos_texU.push_back(energy / nDigit % 10);

			//����i�߂�
			nDigit *= 10;
		}
	}
}

//=============================================
// ���폜
//=============================================
void My::CEnergy_UI::EraseDigit(std::vector<int> pos_texU, int& num_digit)
{
	if (pos_texU[num_digit - 1] == INT_ZERO)
	{//�������炷
		if (num_digit - 1 == 0)
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
