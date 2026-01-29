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
	const D3DXCOLOR OFF_COLOR{ 0.6f,0.6f,0.6f,1.0f };
	const D3DXCOLOR ON_COLOR{ COLOR_WHITE };
	const float CARD_SHIFT_Y = 30.0f;
	const float ILLUST_SHIFT_Y = -32.5f;
	const std::string TEXTURE_PATH =	//テクスチャパス
	{
		{ "data\\TEXTURE\\WatingUI\\wait_bg.png" },
	};
}
//=============================================
// コンストラクタ
//=============================================
My::CWaitZoneBG::CWaitZoneBG(int nPriority) :CButton(nPriority),
m_pNextIllust(nullptr)
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
			if (j != CCardFrame::FRAMETYPE_ILLUST)
			{
				continue;
			}

			if (i != NUM_CARD - 1)
			{
				continue;
			}

			if (m_pNextIllust == nullptr)
			{
				m_pNextIllust = CPaseudoCardIllust::Create({UI_POS.x,UI_POS.y + ILLUST_SHIFT_Y ,UI_POS.z}, { 35.0f,30.0f });
			}
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
	if (m_pNextIllust)
	{
		m_pNextIllust->Uninit();
		m_pNextIllust = nullptr;
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
			m_pPseundCard[i].card_frame[j]->SetColor(GetColor());
		}
	}

	if (m_pNextIllust != nullptr)
	{
		D3DXVECTOR3 card_pos = GetPos();
		m_pNextIllust->SetPos({ card_pos.x,card_pos.y + ILLUST_SHIFT_Y,card_pos.z});
		m_pNextIllust->SetColor(GetColor());
	}

	DrawTop();

	// 親クラスの更新処理を呼ぶ
	CObject2D::Update();
	//頂点設定
	//SetVtx();
}

void My::CWaitZoneBG::DrawTop()
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

		if (!card_list.empty())
		{
			CCard* pCard = *card_list.begin();
			if (m_pNextIllust != nullptr)
			{
				std::string get_tex_pass = pCard->GetBaseStatus().imagePath;
				const std::string illust_folder = "data/TEXTURE/illust/";
				const std::string tex_pass = illust_folder + get_tex_pass;

				m_pNextIllust->SetTexPath(tex_pass);
				m_pNextIllust->SetisDraw(true);


				for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
				{
					if (j == CCardFrame::FRAMETYPE_ILLUST || j == CCardFrame::FRAMETYPE_COST || j == CCardFrame::FRAMETYPE_NAME || j == CCardFrame::FRAMETYPE_TEXT)
					{
						continue;
					}
					m_pPseundCard[NUM_CARD - 1].card_frame[j]->SetColor(COLOR_BLUE);
				}
			}
		}
		else
		{
			if (m_pNextIllust != nullptr)
			{
				//テクスチャを空にして描画もしない
				m_pNextIllust->SetTexPath("");
				m_pNextIllust->SetisDraw(false);
			}
		}
	}
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

	// クリックSEを鳴らす
	My::CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CLICK);
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

		SetColor(ON_COLOR);

		return true;
	}
	else if (!ishit)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.y = UI_POS.y;
		SetPos(pos);

		SetColor(OFF_COLOR);
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
