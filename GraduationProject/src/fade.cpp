//=============================================
//
//�t�F�[�h����[fade.cpp]
//Author Matsuda Towa
//
//=============================================
#include "fade.h"
#include"manager.h"

//=============================================
//�R���X�g���N�^
//=============================================
My::CFade::CFade(int nPriority):CObject2D(nPriority),
m_next_scene()			//�t�F�[�h���
{
}

//=============================================
//�f�X�g���N�^
//=============================================
My::CFade::~CFade()
{
}

//=============================================
//������
//=============================================
HRESULT My::CFade::Init()
{
	//�T�C�Y�擾
	D3DXVECTOR2 size = GetSize();

	size = D3DXVECTOR2(SCREEN_WIDTH, SCREEN_HEIGHT);

	//�T�C�Y����
	SetSize(size);

	//�J���[�擾
	D3DXCOLOR col = GetColor();

	col = D3DXCOLOR(FLOAT_ZERO, FLOAT_ZERO, FLOAT_ZERO, m_colorFade);

	//�J���[����
	SetColor(col);

	//���_�ݒ�
	SetVtx();

	return S_OK;
}

//=============================================
//�I��
//=============================================
void My::CFade::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
//�X�V
//=============================================
void My::CFade::Update()
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{//�t�F�[�h�C�����
			m_colorFade -= FADE_VALUE;
			if (m_colorFade <= FLOAT_ZERO)
			{
				m_colorFade = FLOAT_ZERO;
				m_fade = FADE_NONE; //�������ĂȂ���Ԃ�
			}
		}
		else if (m_fade == FADE_OUT)
		{//�t�F�[�h�A�E�g���
			m_colorFade += FADE_VALUE;
			if (m_colorFade >= FLOAT_ONE)
			{
				m_colorFade = FLOAT_ONE;
				m_fade = FADE_IN; //�t�F�[�h�C����Ԃ�
				CManager::GetInstance()->SetMode(m_next_scene);
			}
		}

		//�J���[�擾
		D3DXCOLOR col = GetColor();

		col = D3DXCOLOR(FLOAT_ZERO, FLOAT_ZERO, FLOAT_ZERO, m_colorFade);

		//�J���[����
		SetColor(col);

		//���_�J���[�̐ݒ�
		SetVtx();
	}

}

//=============================================
//�`��
//=============================================
void My::CFade::Draw()
{
	CObject2D::Draw();
}

//=============================================
//�t�F�[�h�̐ݒ�
//=============================================
void My::CFade::SetFade(CScene::MODE modeNext)
{
    m_next_scene = modeNext;
    m_fade = FADE_OUT;
}

//=============================================
//�t�F�[�h�̐F�擾(��ɃA���t�@�l)
//=============================================
float My::CFade::FadeColor(void)
{
    return m_colorFade;
}

//=============================================
//����
//=============================================
My::CFade* My::CFade::Create(D3DXVECTOR3 pos)
{
	CFade* pFade = new CFade;
	//CObject *pObject = Getobject();
	if (pFade != nullptr)
	{
		pFade->SetPos(pos); //pos�ݒ�

		pFade->m_colorFade = FLOAT_ZERO;

		pFade->SetType(OBJECT_TYPE_FADE); //�^�C�v�ݒ�

		pFade->Init();
	}
	return pFade;
}
