//=============================================
//
//中央エリア[center_area.h]
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
// コンストラクタ
//=============================================
My::CCenterArea::CCenterArea(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCenterArea::~CCenterArea()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCenterArea::Init()
{
	CObject2D::Init();

	SetPos({SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF,0.0f});

	SetSize({ CInputMouse::CENTER_RADIUS,CInputMouse::CENTER_RADIUS});

	D3DXCOLOR col = GetColor();
	col = COLOR_WHITE;
	SetColor(col);

	//テクスチャ登録
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CObject2D::BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));//テクスチャ設定

	SetTexPos(VEC2_RESET_ONE);

	SetVtx();

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCenterArea::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
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
// 描画
//=============================================
void My::CCenterArea::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CCenterArea* My::CCenterArea::Create()
{
	CCenterArea* area = new CCenterArea;
	if (area == nullptr) { return nullptr; }

	area->Init();
	return area;
}
