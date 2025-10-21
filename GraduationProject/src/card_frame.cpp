#include "card_frame.h"

// 静的メンバ初期化

/**
* @brief カードフレームまでの相対パス
*/
std::string My::CCardFrame::FramePass = "../asetto/card_frame/";

/**
* @brief カードフレームの情報構造体の配列
*/
My::CCardFrame::CardFrameInfo My::CCardFrame::m_FrameInfo[My::CCardFrame::FRAMETYPE::FRAMETYPE_MAX] = {
	{"../asetto/card_frame/card_frame.png",		{0,0,0},	{10,10,10}},
	{"../asetto/card_frame/card_illustframe.png",{0,0,0},	{30,10,10}},
	{"../asetto/card_frame/card_textframe.png",	{0,0,0},	{30,10,10}},
	{"../asetto/card_frame/card_nameframe.png",	{0,0,0},	{30,10,10}},
	{"../asetto/card_frame/card_typeframe.png",	{0,0,0},	{30,30,10}},
	{"../asetto/card_frame/card_costframe.png",	{-20,20,6},	{5,5,5}}
};

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardFrame::CCardFrame(int priority): CObject3D(priority),
m_pParent(nullptr),
m_type(FRAMETYPE::FRAMETYPE_BASE)
{
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCardFrame::~CCardFrame()
{
}

//===========================================================================================================
// 初期化
//===========================================================================================================
HRESULT My::CCardFrame::Init()
{
	// テクスチャ設定
	CTexture* pTex = GET_TEXTURE;
	BindTexture(pTex->GetAddress(pTex->Regist(&m_FrameInfo[m_type].pass)));

	// 見えない位置に設定
	SetPos({ -5000, -5000 ,-5000 });

	// サイズ設定
	SetSize(m_FrameInfo[m_type].size);
	//頂点設定
	SetVtx(D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCardFrame::Uninit()
{
	CObject3D::Uninit();
}

//===========================================================================================================
// 更新
//===========================================================================================================
void My::CCardFrame::Update()
{
	D3DXVECTOR3 offsetpos = m_pParent->GetPos() + m_FrameInfo[m_type].offset;

	SetPos(offsetpos);
	SetRot(m_pParent->GetRot());

	// 親のスケールからサイズを設定している
	D3DXVECTOR3 size = {
		m_FrameInfo[m_type].size.x * m_pParent->GetSize().x,
		m_FrameInfo[m_type].size.y * m_pParent->GetSize().y,
		m_FrameInfo[m_type].size.z * m_pParent->GetSize().z,
	};

	// サイズ設定
	SetSize(size);

	//頂点設定
	SetVtx(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
}

//===========================================================================================================
// 描画
//===========================================================================================================
void My::CCardFrame::Draw()
{
	CObject3D::Draw();
}

//===========================================================================================================
// 生成
//===========================================================================================================
My::CCardFrame* My::CCardFrame::Create(FRAMETYPE type, CObject3D* pObj)
{
	CCardFrame* pCardFrame = new CCardFrame();

	// タイプ設定
	pCardFrame->m_type = type;

	// 親設定
	pCardFrame->m_pParent = pObj;

	// 初期化設定
	pCardFrame->Init();

	return pCardFrame;
}

//===========================================================================================================
// 親設定
//===========================================================================================================
void My::CCardFrame::SetParent(CObject3D* pObj)
{
	m_pParent = pObj;
}





My::CFrameBase::CFrameBase()
{
}

My::CFrameBase::~CFrameBase()
{
}

HRESULT My::CFrameBase::Init()
{
	m_CardFrameInfo = {
		"card_frame.png",	{0,0,0},	{10,10,10}
	};
	CCardFrame::Init();
	return S_OK;
}

void My::CFrameBase::Uninit()
{
	CCardFrame::Uninit();
}

void My::CFrameBase::Update()
{
	CCardFrame::Update();
}

void My::CFrameBase::Draw()
{
	CCardFrame::Draw();
}

My::CFrameBase* My::CFrameBase::Create(FRAMETYPE type, CObject3D* pObj)
{
	CFrameBase* pCardFrame = new CFrameBase();

	//// テクスチャ設定
	//CTexture* pTex = GET_TEXTURE;

	//// ファイルパス設定( 相対ファイルパス ＋ Textureパス )
	//std::string str = pCardFrame->FramePass + pCardFrame->m_FrameInfo[type].pass;
	//pCardFrame->BindTexture(pTex->GetAddress(pTex->Regist(&str)));

	// タイプ設定
	pCardFrame->SetType(type);

	// 親設定
	pCardFrame->SetParent(pObj);

	// 初期化設定
	pCardFrame->Init();

	return pCardFrame;
}




My::CFrameillust::CFrameillust()
{
}

My::CFrameillust::~CFrameillust()
{
}

HRESULT My::CFrameillust::Init()
{
	m_CardFrameInfo = {
		"card_illustframe.png",	{0,10,0},	{10,10,10}
	};
	CCardFrame::Init();
	return S_OK;
}

void My::CFrameillust::Uninit()
{
	CCardFrame::Uninit();
}

void My::CFrameillust::Update()
{
	CCardFrame::Update();
}

void My::CFrameillust::Draw()
{
	CCardFrame::Draw();
}

My::CFrameillust* My::CFrameillust::Create(FRAMETYPE type, CObject3D* pObj)
{
	CFrameillust* pCardFrame = new CFrameillust();

	//// テクスチャ設定
	//CTexture* pTex = GET_TEXTURE;

	//// ファイルパス設定( 相対ファイルパス ＋ Textureパス )
	//std::string str = pCardFrame->FramePass + pCardFrame->m_FrameInfo[type].pass;
	//pCardFrame->BindTexture(pTex->GetAddress(pTex->Regist(&str)));

	// タイプ設定
	pCardFrame->SetType(type);

	// 親設定
	pCardFrame->SetParent(pObj);

	// 初期化設定
	pCardFrame->Init();

	return pCardFrame;
}
