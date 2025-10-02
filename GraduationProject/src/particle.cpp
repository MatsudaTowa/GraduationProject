//=============================================
//
//�p�[�e�B�N��[particle.cpp]
//Author Matsuda Towa
//
//=============================================
#include "particle.h"

//=============================================
// �R���X�g���N�^
//=============================================
My::CParticle::CParticle(int nPriority):CBillboard_Anim(nPriority),
m_life(INT_ZERO),
m_startlife(INT_ZERO),
m_startcolor(VEC3_RESET_ZERO),
m_move(VEC3_RESET_ZERO)
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CParticle::~CParticle()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CParticle::Init()
{
	CBillboard_Anim::Init();
	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CParticle::Uninit()
{
	CBillboard_Anim::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CParticle::Update()
{
	--m_life;
	D3DXVECTOR3 pos = GetPos();
	pos += m_move;
	SetPos(pos);

	D3DXVECTOR3 size = GetSize();

	//���݂̃T�C�Y�ɉ����ăA���t�@�l�ύX
	float current_alpha = (m_life * m_startcolor.a) / m_startlife;
	D3DXCOLOR col = GetColor();
	col.a = current_alpha;

	//���
	SetColor(col);
	SetSize(size);

	if (m_life <= 0)
	{
		Uninit();
	}
}

//=============================================
// �`��
//=============================================
void My::CParticle::Draw()
{
	CBillboard_Anim::Draw();
}
