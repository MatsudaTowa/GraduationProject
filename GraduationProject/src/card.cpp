//===========================================================================================================================================================
// 
// カードの処理 [card.cpp]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#include "card.h"
#include "game.h"
#include "card_state.h"
#include <typeinfo>

//===========================================================================================================
// コンストラクタ
//===========================================================================================================
My::CCard::CCard(int nPriority):CObjectX(nPriority),
	m_pState(nullptr),
	m_IsChoice(false),
	m_outpos({0,0,0}),
	m_StateNum(CCardState::CARD_NEUTRAL)
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
	// モデル設定
	CModel*pModel= CManager::GetInstance()->GetModel();
	int Idx = pModel->Regist("data\\model\\card_traial_002.x");
	CModel::MODEL_INFO mi = pModel->GetModelInfo(Idx);
	BindXFile(mi.pBuffMat, mi.dwNumMat, mi.pMesh);

	CObjectX::Init();

	// スケール
	D3DXVECTOR3 scale = GetScale();

	scale.x *= 1.2f;
	scale *= 1.0f;                                                                                                                                             

	SetScale(scale);

	m_pState = new CCardStateNeutral();
	m_pState->Init();
	m_StateNum = CCardState::CARD_NEUTRAL;

	return S_OK;
}

//===========================================================================================================
// 終了処理
//===========================================================================================================
void My::CCard::Uninit()
{
	CObjectX::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCard::Update()
{
	//ChangeState(CCardState::CARD_NEUTRAL);

	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	//SetPos({ pCamera->GetPosV().x,pCamera->GetPosV().y - 100.0f,pCamera->GetPosV().z + 30.0f });

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += 0.3f;

	// TODO : マウスを実装しようとしました

	//POINT mouse;
	//GetCursorPos(&mouse);
	//ScreenToClient(GET_HWND, &mouse);
	////D3DXVECTOR3 pos = GetPos();

	//D3DXVECTOR3 outpos;
	//float sX=(float)mouse.x, sY=(float)mouse.y;

	////クライアントサイズを取得
	//RECT rc;
	//GetClientRect(GET_HWND, &rc);
	//float width = (float)(rc.right - rc.left);
	//float height = (float)(rc.bottom - rc.top);
	//
	//D3DXMATRIX View, Proj;
	//pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_VIEW, &View);	// ビュー行列の取得
	//pDevice->GetTransform(D3DTRANSFORMSTATETYPE::D3DTS_PROJECTION, &Proj);	// 投影行列の取得

	////D3DXVECTOR3 pos = { GetPos().x,GetPos().z,GetPos().y };
	//D3DXVECTOR3 pos = GetPos();

	//CalcScreenToXZ(&m_outpos, pos, sX, sY, width, height, &View, &Proj);
	////CalcScreenToXZ(&m_outpos, GetPos(), mouse.x, mouse.y, SCREEN_WIDTH, SCREEN_WIDTH, &View, &Proj);

	//if (m_outpos.x >= -50 && m_outpos.x < 50)
	//{
	//	ChangeState(CCardState::CARD_PICKUP);
	//}

	//if (GET_COLISION->CheckColisionCircle({ sX,0.0f,sY}, 20.0f, pos).colision)
	//{
	//	ChangeState(CCardState::CARD_PICKUP);
	//}

	m_pState->Update(this);

	SetRot(rot);
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CCard::Draw()
{
	CObjectX::Draw();

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(GET_HWND, &mouse);
	float x = (float)mouse.x, y = (float)mouse.y;

	LPD3DXFONT pFont = GET_RENDERER->GetFont();
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[512];
	sprintf(&aStr[0], "\n\n\n\n\n\n\n\n\n\noutpos = %f , %f , %f \n"
		" cardpos = %f , %f , %f \n"
		" mousepos = %f , %f \n\n"
		" cardstate : %d\n\n\n\n\n\n"
		" [ 操作方法 ] \n"
		" < ← → > でカード選択 \n"
		" < J > 選択したカードをキャスト \n"
		" < 1,2,3 > でステイ状態にする \n"
		" 時間が経過したらカードがトリガーされ消える \n",
		m_outpos.x, m_outpos.y, m_outpos.z,
		GetPos().x, GetPos().y, GetPos().z
		,x, y,
		m_StateNum);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
}

//===========================================================================================================
// 生成処理
//===========================================================================================================
My::CCard* My::CCard::Create()
{
	CCard* pCard = new CCard(5);
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
		// 同じステートだった場合ここを通らない
		if (m_StateNum == state)
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



