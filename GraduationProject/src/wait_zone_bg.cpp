//=============================================
//
//待機背景[wait_zone_bg.h]
//Author Kasai Keisuke
//
//=============================================
#include "wait_zone_bg.h"
#include "active_scene_manager.h"
#include "zone_manager.h"
//無名空間
namespace
{
	const D3DXVECTOR3 UI_POS{ 1050.0f, 690.0f, 0.0f };
	const D3DXVECTOR2 UI_SIZE{ 51.0f, 80.0f };	//サイズ
	const float CARD_SHIFT_Y = 30.0f;
	const std::string TEXTURE_PATH =	//テクスチャパス
	{
		{ "data\\TEXTURE\\WatingUI\\wait_bg.png" },
	};
}
//=============================================
// コンストラクタ
//=============================================
My::CWaitZoneBG::CWaitZoneBG(int nPriority) :CButton(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CWaitZoneBG::~CWaitZoneBG()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CWaitZoneBG::Init()
{
	//親クラスの初期化処理を呼ぶ
	CObject2D::Init();

	SetPos(UI_POS);

	for (int i = 0; i < NUM_CARD; ++i)
	{
		D3DXVECTOR3 card_pos = GetPos();
		card_pos.x += (NUM_CARD - 1 - i) * 4.0f;
		card_pos.y += ((NUM_CARD - 1 - i) * 4.0f);

		for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
		{
			m_pPseundCard[i].card_frame[j] =
				CPsendCardFrame::Create(
					(CCardFrame::FRAMETYPE)j,
					card_pos,
					VEC3_RESET_ZERO
				);
		}
	}

	//テクスチャ情報の取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャ設定
	BindTexture(pTexture->GetAddress(pTexture->Regist(TEXTURE_PATH)));

	//自分自身のサイズ取得
	D3DXVECTOR2 size = UI_SIZE;

	//テクスチャ座標設定
	SetTexPos(VEC2_RESET_ONE);

	//サイズを代入
	SetSize(size);

	//頂点設定
	SetVtx();

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CWaitZoneBG::Uninit()
{
	for (int i = 0; i < NUM_CARD; ++i)
	{
		// カードフレーム生成
		for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
		{
			m_pPseundCard[i].card_frame[j]->Uninit();
		}
	}

	// カードフレーム生成
	for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; ++i)
	{
		if (m_pTopCardFrame[i] == nullptr) { continue; }
		m_pTopCardFrame[i]->Uninit();
		m_pTopCardFrame[i] = nullptr;
	}
	//親クラスの終了処理を呼ぶ
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CWaitZoneBG::Update()
{
	CardisView();

	for (int i = 0; i < NUM_CARD; ++i)
	{
		D3DXVECTOR3 card_pos = GetPos();
		card_pos.x += (NUM_CARD - 1 - i) * 4.0f;

		card_pos.y += ((NUM_CARD - 1 - i) * 4.0f);

		for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
		{
			m_pPseundCard[i].card_frame[j]->SetPos(card_pos);
		}
	}

	CActiveSceneCharacterState* state = CActiveSceneManager::GetInstance()->GetPlayer()->GetState();
	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CWaitZone* zone = duel_state->GetZoneManager()->GetWaitZone();
		//準備OKか切り替え
		bool isView = duel_state->GetIsWaitView();
		std::list<CCard*> card_list = zone->GetList();

		if (!card_list.empty())
		{
			CCard* top_card = *card_list.begin();
			//// カードフレーム生成
			//for (int i = 0; i < CCardFrame::FRAMETYPE_MAX; i++)
			//{
			//	if (m_pTopCardFrame[i] == nullptr)
			//	{
			//		m_pTopCardFrame[i] = CCardFrame::Create((CCardFrame::FRAMETYPE)i, top_card);
			//	}
			//	else if(m_pTopCardFrame[i] != nullptr)
			//	{
			//		m_pTopCardFrame[i]->SetParent(top_card);
			//		m_pTopCardFrame[i]->SetPos(UI_POS);
			//	}
			//}
		}
	}

	// 親クラスの更新処理を呼ぶ
	CObject2D::Update();

	//カラー取得
	D3DXCOLOR col = GetColor();

	col = { 0.8f,0.8f,0.8f,0.0f };

	//カラーを代入
	SetColor(col);
	//頂点設定
	//SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CWaitZoneBG::Draw()
{	
}

void My::CWaitZoneBG::ButtonTrigger()
{
	CActiveSceneCharacterState* state = CActiveSceneManager::GetInstance()->GetPlayer()->GetState();

	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CWaitZone* zone = duel_state->GetZoneManager()->GetWaitZone();
		std::list<CCard*> card_list = zone->GetList();

		//準備OKか切り替え
		bool isView = duel_state->GetIsWaitView();
		isView = isView ? false : true;
		duel_state->SetIsWaitView(isView);

		zone->GetSelectionRange()->SetisDraw(isView);
	}
}

bool My::CWaitZoneBG::ProcessMouseEvent()
{
	// 標準の色に設定
	SetColor(COLOR_WHITE);
	// 当たっているかどうか
	bool ishit = CButton::ProcessMouseEvent();

	if (ishit)
	{
		CActiveSceneCharacterState* state = CActiveSceneManager::GetInstance()->GetPlayer()->GetState();

		// ロビーじゃなかったら抜ける
		if (typeid(*state) == typeid(CPlayerDuelState))
		{
			CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
			CSelectionRange* range = duel_state->GetZoneManager()->GetWaitZone()->GetSelectionRange();
			bool is_hit_area = GET_COLISION->Check2DPolygonColision(GET_INPUT_MOUSE->GetMousePos(), { 3.0f,3.0f }, { range->GetPos().x,range->GetPos().y,0.0f }, range->GetSize());

			//墓地を見ているときは
			if (GET_INPUT_MOUSE->GetTrigger(0) && !is_hit_area)
			{
				//押された時の処理の名前
				ButtonTrigger();
			}
		}
		D3DXVECTOR3 pos = GetPos();
		pos.y = UI_POS.y - CARD_SHIFT_Y;
		SetPos(pos);

		return true;
	}
	else if (!ishit)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y = UI_POS.y;
		SetPos(pos);
	}

	return false;
}

void My::CWaitZoneBG::CardisView()
{
	CActiveSceneCharacterState* state = CActiveSceneManager::GetInstance()->GetPlayer()->GetState();
	// ロビーじゃなかったら抜ける
	if (typeid(*state) == typeid(CPlayerDuelState))
	{
		CPlayerDuelState* duel_state = dynamic_cast<CPlayerDuelState*>(state);
		CWaitZone* zone = duel_state->GetZoneManager()->GetWaitZone();
		//準備OKか切り替え
		bool isView = duel_state->GetIsWaitView();
		std::list<CCard*> card_list = zone->GetList();

		for (auto& itr : card_list)
		{
			if (itr == nullptr) { continue; }
			itr->SetisDraw(isView);
		}
	}
}

//=============================================
// 生成
//=============================================
My::CWaitZoneBG* My::CWaitZoneBG::Create()
{
	CWaitZoneBG* pObject = new CWaitZoneBG();

	//nullならnullを返す
	if (pObject == nullptr) { return nullptr; }

	//初期化
	pObject->Init();

	return pObject;
}
