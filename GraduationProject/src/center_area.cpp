//=============================================
//
//�����G���A[center_area.h]
//Author Matsuda Towa
//
//=============================================
#include "center_area.h"
#include "active_manager.h"
namespace
{
	const std::string TEX_NAME = "data\\TEXTURE\\cancel.png";
	const D3DXCOLOR DEFAULT_COLOR = { 0.2f,0.2f,0.2f,1.0f };
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CCenterArea::CCenterArea(int nPriority):CObject2D(nPriority),
m_isSelect(false)
{
}

//=============================================
// �f�X�g���N�^
//=============================================
My::CCenterArea::~CCenterArea()
{
}

//=============================================
// ������
//=============================================
HRESULT My::CCenterArea::Init()
{
	CObject2D::Init();

	SetPos({SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF,0.0f});

	SetSize({ CInputMouse::CENTER_RADIUS,CInputMouse::CENTER_RADIUS});

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	//�e�N�X�`���o�^
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));//�e�N�X�`���ݒ�

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();

	return S_OK;
}

//=============================================
// �I��
//=============================================
void My::CCenterArea::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// �X�V
//=============================================
void My::CCenterArea::Update()
{
	SetVtx();

	if (!CGameManager::GetInstance()->GetAreaManager()->GetActive())
	{
		SetColor(COLOR_NONE);
		return;
	}

	if (!m_isSelect)
	{
		SetColor(DEFAULT_COLOR);
		return;
	}

	SetColor(COLOR_WHITE);

	//TODO:�J�[�h�𗣂�����ɕύX�\��
	if (GET_INPUT_MOUSE->GetTrigger(0))
	{
		CGameManager::GetInstance()->ChangeState(new CDuel);
	}
}

//=============================================
// �`��
//=============================================
void My::CCenterArea::Draw()
{
	CObject2D::Draw();
}

//=============================================
// ����
//=============================================
My::CCenterArea* My::CCenterArea::Create()
{
	CCenterArea* area = new CCenterArea;
	if (area == nullptr) { return nullptr; }

	area->Init();
	return area;
}
