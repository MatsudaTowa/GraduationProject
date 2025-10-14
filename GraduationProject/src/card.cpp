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
	m_IsChoice(false)
{
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
	scale *= 0.8f;                                                                                                                                             

	SetScale(scale);

	m_pState = new CCardStateNeutral();
	m_pState->Init();

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
	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);
	//SetPos({ pCamera->GetPosV().x,pCamera->GetPosV().y - 100.0f,pCamera->GetPosV().z + 30.0f });

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += 0.3f;

	m_pState->Update(this);

	SetRot(rot);
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CCard::Draw()
{
	CObjectX::Draw();
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
		if (m_StateNum == state)
			return;

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

		case CCardState::CARD_PLAY:
			m_pState = new CCardStatePlay();
			break;

		default:
			assert(1);
			break;
		}

		m_StateNum = state;

		m_pState->Init(this);
		m_pState->Init();
	}
}



