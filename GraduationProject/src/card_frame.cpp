#include "card_frame.h"
#include "card_frame_cost.h"
#include "card_frame_name.h"
#include "card_frame_illust.h"
#include "card_frame_type.h"
#include "card_frame_text.h"
#include "active_scene_state.h"
#include "active_scene_manager.h"
#include "enemy_state.h"
// 静的メンバ初期化

/**
* @brief カードの基本サイズ
*/
const float My::CCardFrame::CARD_WIDTH = 70.0f;
const float My::CCardFrame::CARD_HEIGHT = 110.0f;

/**
* @brief カードフレームの情報構造体の配列
*/
My::CCardFrame::CardFrameInfo My::CCardFrame::m_FrameInfo[My::CCardFrame::FRAMETYPE::FRAMETYPE_MAX] = {
	{"data/TEXTURE/cardframe/bg.png",		{0.0f,0.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	true},
	{"data/TEXTURE/illust/",	{0.0f,-40.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	false},
	{"data/TEXTURE/cardframe/text.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	false},
	{"data/TEXTURE/cardframe/name.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	false},
	{"data/TEXTURE/cardframe/type.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	true},
	{"data/TEXTURE/cardframe/cost.png",	{0.0f,0.0f,0.0f},	{CARD_WIDTH,CARD_HEIGHT},	false}
};

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCardFrame::CCardFrame(int priority): CObject2D(priority),
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
	if (m_type != FRAMETYPE_ILLUST)
	{
		BindTexture(pTex->GetAddress(pTex->Regist(m_FrameInfo[m_type].pass)));
	}

	// 見えない位置に設定
	SetPos({ -5000, -5000 ,-5000 });

	// サイズ設定
	SetSize({CARD_WIDTH,CARD_HEIGHT});

	// カラー設定
	SetColor(SetColorCard());

	//頂点設定
	SetVtx();

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCardFrame::Uninit()
{
	CObject2D::Uninit();
}

//===========================================================================================================
// 更新
//===========================================================================================================
void My::CCardFrame::Update()
{
	CCard::ZONE current_zone = GetCard()->GetCurrentZone();
	CActiveSceneCharacterState* state = nullptr;

	std::list<CActiveSceneCharacter*> charcter_list = CActiveSceneManager::GetInstance()->GetCharacterList();
	for (auto& itr : charcter_list)
	{
		if (m_pParent->GetUserId() != itr->GetPlayerIdx()) continue;

		state = itr->GetState();
		break;
	}

	//CActiveSceneCharacterState* state = itr->GetPlayerUI()->GetCemeteryButton()->GetCharacter()->GetState();

	CDuelCharacter* duel_state = dynamic_cast<CDuelCharacter*>(state);

	// 山札時はコストを表示しないように TODO:今後はここの条件式を見直す必要あり
	if (current_zone == CCard::DECK)
	{
		SetisDraw(false);
		GetCard()->SetisDraw(false);
		return;
	}
	else if (current_zone == CCard::CEMETERY)
	{
		SetisDraw(duel_state->GetIsCemeteryView());
	}
	else
	{
		SetisDraw(true);
		GetCard()->SetisDraw(true);
	}

	D3DXVECTOR3 screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX), m_pParent->GetPos()); //スクリーン座標に変換

	D3DXVECTOR3 offsetpos = screen_pos + m_FrameInfo[m_type].offset;
	D3DXVECTOR3 rot = m_pParent->GetRot();

	SetPos(offsetpos);
	SetRot(rot);

	//// 親のスケールからサイズを設定している
	//D3DXVECTOR2 size = {
	//	CARD_WIDTH* m_pParent->GetSize().x,
	//	CARD_HEIGHT* m_pParent->GetSize().z,
	//};

	// サイズ設定
	//SetSize(m_FrameInfo[m_type].size);
	SetVtx();
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

	CObject2D::Draw();

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
	CCardFrame* pCardFrame = nullptr;
	switch (type)
	{
	case FRAMETYPE_ILLUST:
		pCardFrame = new CCardFrameIllust(28);
		break;
	case FRAMETYPE_NAME:
		pCardFrame = new CCardFrameName(28);
		break;
	case FRAMETYPE_TYPE:
		pCardFrame = new CCardFrameType(28);
		break;
	case FRAMETYPE_COST:
		pCardFrame = new CCardFrameCost(28);
		break;
	default:
		pCardFrame = new CCardFrame(28);
		break;
	}

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

