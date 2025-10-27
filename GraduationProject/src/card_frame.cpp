#include "card_frame.h"

// 静的メンバ初期化

/**
* @brief カードの基本サイズ
*/
const float My::CCardFrame::CARD_WIDTH = 10.0f;
const float My::CCardFrame::CARD_HEIGHT = 17.0f;

/**
* @brief カードフレームまでの相対パス
*/
std::string My::CCardFrame::FramePass = "../asetto/card_frame/";

/**
* @brief カードフレームの情報構造体の配列
*/
My::CCardFrame::CardFrameInfo My::CCardFrame::m_FrameInfo[My::CCardFrame::FRAMETYPE::FRAMETYPE_MAX] = {
	{"../asetto/cardframe/bg.png",		{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	true},
	{"../asetto/cardframe/illust.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	false},
	{"../asetto/cardframe/text.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	false},
	{"../asetto/cardframe/name.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	false},
	{"../asetto/cardframe/type.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	true},
	{"../asetto/cardframe/cost.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,1.0f,CARD_HEIGHT},	false}
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
	SetSize({CARD_WIDTH,1.0f,CARD_HEIGHT});

	// カラー設定
	SetColor(SetColorCard());

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
	D3DXVECTOR3 rot = m_pParent->GetRot();

	//SetPos(offsetpos);
	SetPos(m_pParent->GetPos());
	SetRot({0,0,0});
	SetRot(rot);

	// 親のスケールからサイズを設定している
	D3DXVECTOR3 size = {
		CARD_WIDTH* m_pParent->GetSize().x,
		3.0f * m_pParent->GetSize().y,
		CARD_HEIGHT* m_pParent->GetSize().z,
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
	CRenderer* pRender = GET_RENDERER;
	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// Zバッファに書き込まない
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// αブレンド
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	CObject3D::Draw();

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zの比較方法
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	// Zバッファに書き込む
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//===========================================================================================================
// 生成
//===========================================================================================================
My::CCardFrame* My::CCardFrame::Create(FRAMETYPE type, CCard* pObj)
{
	CCardFrame* pCardFrame = new CCardFrame(10);

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
void My::CCardFrame::SetParent(CCard* pObj)
{
	m_pParent = pObj;
}

//===========================================================================================================
// カードタイプに合わせて色を変える
//===========================================================================================================
D3DXCOLOR My::CCardFrame::SetColorCard()
{
	if (!m_FrameInfo[m_type].IsChangeable)
		return COLOR_WHITE;

	// 親のカードタイプを取得
	CCard::CARDTYPE_ CardType = m_pParent->GetCardType();

	// 色を代入する変数
	D3DXCOLOR color = COLOR_WHITE;

	// カードのタイプによって色を変える
	switch (CardType)
	{
	case CCard::CARDTYPE_::TYPE_ATTACK:
		color = COLOR_RED;
		break;

	case CCard::CARDTYPE_::TYPE_DEFFENCE:
		color = COLOR_BLUE;
		break;

	case CCard::CARDTYPE_::TYPE_ASSIST:
		color = COLOR_PURPLE;
		break;
	}

	return color;
}

