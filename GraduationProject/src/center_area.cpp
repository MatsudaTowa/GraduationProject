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
	const std::string TEX_NAME = "data\\TEXTURE\\play.png";
}

//=============================================
// �R���X�g���N�^
//=============================================
My::CCenterArea::CCenterArea(int nPriority):CObject2D(nPriority)
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
	SetColor(COLOR_WHITE);
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
