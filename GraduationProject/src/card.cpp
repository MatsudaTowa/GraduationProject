//===========================================================================================================================================================
// 
// カードの処理 [card.cpp]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#include "card.h"
#include "game.h"
#include "card_state.h"
#include "card_frame.h"
#include <typeinfo>

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCard::CCard(int nPriority) :CObject3D(nPriority),
m_pState(nullptr),
m_IsChoice(false),
m_outpos({ 0,0,0 }),
m_StateNum(CCardState::CARD_NEUTRAL),
m_CardType(CARDTYPE_::TYPE_ATTACK),
m_IsChange(true),
m_target(CInputMouse::AREA::CENTER),
m_Cost(INT_ZERO),
m_AttackPower(INT_ZERO)
{
	//if (m_pTop == nullptr)
	//{// top が設定されていなかったら
	//	m_pPrev = nullptr;	// 
	//	m_pNext = nullptr;	// 
	//	m_pTop = this;		// 最初のポインタとして設定
	//	m_pCur = this;		// 最後尾のポインタとして設定
	//}
	//else
	//{
	//	m_pNext = nullptr;			// 次のポインタをヌルにしておく
	//	m_pPrev = m_pCur;			// この時点での最後尾を前のポインタに設定する
	//	m_pCur = this;				// 最後尾を自身に設定する
	//	m_pPrev->m_pNext = this;	// 前のポインタの次のポインタに自身を設定する
	//}
}

//===========================================================================================================
// デストラクタ
//===========================================================================================================
My::CCard::~CCard()
{
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCard::Init()
{
	CObject3D::Init();

	m_pState = new CCardStateNeutral();
	m_pState->Init();
	m_StateNum = CCardState::CARD_NEUTRAL;

	m_Param.power = Rundom(1, 10);
	m_Param.cost = Rundom(1, 10);

	// カードフレーム生成
	for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; i++)
	{
		CCardFrame::Create((CCardFrame::FRAMETYPE)i, this);
	}

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCard::Uninit()
{
	CObject3D::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCard::Update()
{
	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += -1.2f;

	//CardCastToMouse();
	
	m_pState->Update(this);

	SetRot(rot);
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CCard::Draw()
{
	//CObjectX::Draw();

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(GET_HWND, &mouse);
	float x = (float)mouse.x, y = (float)mouse.y;

	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\noutpos = %f , %f , %f \n"
		" cardpos = %f , %f , %f \n"
		" cardrot = %f , %f , %f \n"
		" mousepos = %f , %f \n\n"
		" cardstate : %d \n"
		"\n\n\n\n\n\n [ 操作方法 ] \n"
		" < ← → > でカード選択 \n"
		" < J > 選択したカードをキャスト \n"
		" < 1,2,3 > でステイ状態にする \n"
		" 時間が経過したらカードがトリガーされ消える \n",
		m_outpos.x, m_outpos.y, m_outpos.z,
		GetPos().x, GetPos().y, GetPos().z,
		GetRot().x, GetRot().y, GetRot().z
		,x, y,
		m_StateNum);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
}

//===========================================================================================================
// 生成処理
//===========================================================================================================
My::CCard* My::CCard::Create(CCard::CARDTYPE_ type)
{
	CCard* pCard = new CCard(5);

	// カードタイプの設定
	pCard->m_CardType = type;

	pCard->Init();

	return pCard;
}

//===========================================================================================================
// ステートを変更する
//===========================================================================================================
void My::CCard::ChangeState(CCardState* state)
{
	if (m_pState != nullptr)
	{
		//// 型名取得
		//const type_info& cur_state_id = typeid(m_pState);	// 現在のステートのID
		//const type_info& next_state_id = typeid(state);		// 次のステートのID

		//const char* cur_state_name = cur_state_id.name();
		//const char* next_state_name = next_state_id.name();

		delete m_pState;
		m_pState = state;

		m_pState->Init(this);
		m_pState->Init();
	}
}

//===========================================================================================================
// ステートを変更する
//===========================================================================================================
void My::CCard::ChangeState(CCardState::CARD_STATE state)
{
	if (m_pState != nullptr)
	{
		// 同じステートかチェンジできない場合ここを通らない
		if (m_StateNum == state || !m_IsChange)
			return;

		// 削除
		delete m_pState;
		m_pState = nullptr;

		switch (state)
		{
		case CCardState::CARD_NEUTRAL:
			m_pState = new CCardStateNeutral();
			break;

		case CCardState::CARD_PICKUP:
			m_pState = new CCardStatePickup();
			break;

		case CCardState::CARD_CAST:
			m_pState = new CCardStateCast();
			break;

		case CCardState::CARD_STAY:
			m_pState = new CCardStateStay();
			break;

		case CCardState::CARD_TRIGGER:
			m_pState = new CCardStateTrigger();
			break;

		default:
			assert(1);
			break;
		}

		// ステート番号も保存しておく
		m_StateNum = state;

		// 初期化
		m_pState->Init(this);
		m_pState->Init();
	}
}

//===========================================================================================================
// カードをマウスでキャスト
//===========================================================================================================
bool My::CCard::CardCastToMouse()
{
	// 選択状態とキャスト状態以外は通さない
	if (GetStateNum() != CCardState::CARD_PICKUP &&
		GetStateNum() != CCardState::CARD_CAST)
	{
		return false;
	}

	// カメラ取得
	CCamera* pCamera = GET_CAMERA(0);
	// マウス取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;
	// カード座標
	D3DXVECTOR3 pos = GetPos();

	// カード座標をスクリーン座標変換する
	D3DXVECTOR3 screenpos;
	//screenpos = ConvertToScreenPos(pCamera, pos);
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//クライアントサイズを取得
	RECT rc;
	GetClientRect(GET_HWND, &rc);
	float width = (float)(rc.right - rc.left);
	float height = (float)(rc.bottom - rc.top);
	
	D3DXMATRIX View, Proj;
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);	// ビュー行列の取得
	pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);	// 投影行列の取得

	D3DXVECTOR3 resultpos = {0,0,0};

	if (pMouse->GetPress(0))
	{
		// キャストステートにする
		ChangeState(CCardState::CARD_CAST);

		screenpos = pMouse->GetMousePos();
		CalcScreenToWorld(&resultpos, screenpos.x, screenpos.y, 1.0f, width, height, &View, &Proj);
		resultpos.y += 20.0f;
		SetPos(resultpos);

		return true;
	}
	else if (pMouse->GetRelease(0))
	{
		// ステイ遷移
		ChangeState(CCardState::CARD_STAY);
		// 
		m_target = pMouse->GetArea();
	}

	return false;
	
}

//===========================================================================================================
// マウスでカードを選択する
//===========================================================================================================
bool My::CCard::CardSelectToMouse()
{
	if (GetStateNum() == CCardState::CARD_CAST
		|| GetStateNum() == CCardState::CARD_STAY
		|| GetStateNum() == CCardState::CARD_TRIGGER)
		return false;

	// カメラ取得
	CCamera* pCamera = GET_CAMERA(0);
	// マウス取得
	CInputMouse* pMouse = GET_INPUT_MOUSE;

	// マウス座標
	D3DXVECTOR2 mousepos = { pMouse->GetMousePos().x, pMouse->GetMousePos().y };
	// カード座標
	D3DXVECTOR3 pos = GetPos();

	// カードの座標をスクリーン座標変換した座標を格納する変数
	D3DXVECTOR3 screenpos,screenposN;
	
	// カード座標をスクリーン座標変換する
	screenpos = ConvertToScreenPos(pCamera, pos);
	// ニュートラル状態のカード座標をスクリーン座標変換する
	screenposN = ConvertToScreenPos(pCamera, m_NeutralPos);

	// マウスとカードの位置の差
	D3DXVECTOR2 resultpos;
	resultpos.x = mousepos.x - screenpos.x;
	resultpos.y = mousepos.y - screenpos.y;

	// マウスとニュートラル状態のカードの位置の差
	D3DXVECTOR2 resultposN;
	resultposN.x = mousepos.x - screenposN.x;
	resultposN.y = mousepos.y - screenposN.y;

	// 矩形判定
	if (resultpos.x <= 50.0f * GetSize().x && resultpos.x >= -50.0f * GetSize().x &&
		resultpos.y <= 100.0f * GetSize().y && resultpos.y >= -100.0f * GetSize().y ||
		resultposN.x <= 50.0f && resultposN.x >= -50.0f &&
		resultposN.y <= 100.0f && resultposN.y >= -100.0f)
	{
		// カード選択状態にする
		ChangeState(CCardState::CARD_PICKUP);
		return true;
	}
	else
	{
		// カード非選択状態にする
		ChangeState(CCardState::CARD_NEUTRAL);
		return false;
	}

	return false;
}



