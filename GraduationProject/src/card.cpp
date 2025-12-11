//===========================================================================================================================================================
// 
// カードの処理 [card.cpp]
// Author : 梅田　爽真
// 
//===========================================================================================================================================================
#include "card.h"
#include "active_scene.h"
#include "card_state.h"
#include "card_frame.h"
#include "active_scene_manager.h"
#include "duel_manager.h"
#include "zone_manager.h"
#include <typeinfo>
#include "raknet.h"
#include "card_attack.h"

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
m_pCardHolder(nullptr),
m_isUpdate(true),
m_CurrentZone(ZONE::NONE_ZONE),
m_OldZone(ZONE::NONE_ZONE),
m_PreCalculateStrategy(),
m_PostCalculateStrategy(),
m_UserArea(CInputMouse::AREA::CENTER),
m_nUserId(-1)
{
	m_pTargetPlayerList.clear();
	m_PreCalculateStrategy.clear();
	m_PostCalculateStrategy.clear();
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
	m_pTargetPlayerList.clear();
}

//===========================================================================================================
// 初期化処理
//===========================================================================================================
HRESULT My::CCard::Init()
{
	CObject3D::Init();

	// ステートの設定
	m_pState = new CCardStateNeutral();
	m_pState->Init();
	m_StateNum = CCardState::CARD_NEUTRAL;
	m_OldStateNum = CCardState::CARD_NEUTRAL;

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
	for (auto& itr : m_PreCalculateStrategy)
	{
		if (itr == nullptr) { continue; }
		delete itr;
		itr = nullptr;
	}
	m_PreCalculateStrategy.clear();

	for (auto& itr : m_PostCalculateStrategy)
	{
		if (itr == nullptr) { continue; }
		delete itr;
		itr = nullptr;
	}
	m_PostCalculateStrategy.clear();

	CObject3D::Uninit();
}

//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCard::Update()
{

}
//===========================================================================================================
// 更新処理
//===========================================================================================================
void My::CCard::Update(CDuelCharacter* duel)
{
	//更新フラグがないものは更新しない
	if (!m_isUpdate) return;

	// カメラの位置と角度に合わせる
	CCamera* pCamera = CManager::GetInstance()->GetCamera(0);

	D3DXVECTOR3 rot = pCamera->GetRot();
	rot.x += -1.2f;

	//CardCastToMouse();
	
	m_pState->Update(this, duel);

	//if(m_OldStateNum != m_StateNum)
	{
	}

	SetRot(rot);
}

//===========================================================================================================
// 描画処理
//===========================================================================================================
void My::CCard::Draw()
{
	//更新フラグがないものは描画しない
	return;

	//CObjectX::Draw();

#ifdef _DEBUG
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
		, x, y,
		m_StateNum);
	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 0, 0, 255));
#endif // _DEBUG


}

//===========================================================================================================
// 生成処理
//===========================================================================================================
My::CCard* My::CCard::Create(CCard::CARDTYPE_ type)
{
	//CCard* pCard = //new CCard(5);

	//// カードタイプの設定
	//pCard->m_CardType = type;

	//pCard->Init();

	return nullptr;
}

//===========================================================================================================
// ステートを変更する
//===========================================================================================================
void My::CCard::ChangeState(CCardState* /*state*/)
{
	if (m_pState != nullptr)
	{
		//// 型名取得
		//const type_info& cur_state_id = typeid(m_pState);	// 現在のステートのID
		//const type_info& next_state_id = typeid(state);		// 次のステートのID

		//const char* cur_state_name = cur_state_id.name();
		//const char* next_state_name = next_state_id.name();

		/*delete m_pState;
		m_pState = state;

		m_pState->Init(this);
		m_pState->Init();*/
	}
}

//===========================================================================================================
// ステートを変更する
//===========================================================================================================
void My::CCard::ChangeState(CCardState::CARD_STATE state, CDuelCharacter* duel)
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

		case CCardState::CARD_SELECT:
			m_pState = new CCardStateSelect();
			break;

		case CCardState::CARD_CAST:
			m_pState = new CCardStateCast();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::CAST, duel), duel->GetZoneManager()->GetCastPreviewZone(), true);
			break;

		case CCardState::CARD_STAY:
			m_pState = new CCardStateStay();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::CAST, duel), duel->GetZoneManager()->GetCastPreviewZone(), true);

			// ステイ時間のリセット
			ResetStayTime(duel);
			break;

		case CCardState::CARD_WAIT:
			m_pState = new CCardStateWait();
			duel->GetZoneManager()->MoveZone(this, CastToEnumZone(ZONE::WAIT, duel), duel->GetZoneManager()->GetWaitZone(), true);
			break;

		case CCardState::CARD_TRIGGER:
			m_pState = new CCardStateTrigger();
			break;

		case CCardState::CARD_CEMETERY:
			m_pState = new CCardStateCemetery();
			//duel->GetZoneManager()->MoveZone(this, CastToEnumZone(m_CurrentZone, duel), duel->GetZoneManager()->GetCemetery(), true);
			break;

		default:
			assert(1);
			break;
		}

		// ステート番号も保存しておく
		m_StateNum = state;

		// 初期化
		m_pState->Init(this, duel);
		//m_pState->Init();
	}
}

//===========================================================================================================
// ターゲットリストの登録
//===========================================================================================================
void My::CCard::RegistTargetList(CActiveSceneCharacter* target_list)
{
	//ターゲットリストの情報を登録
	m_pTargetPlayerList.push_back(target_list);
}

//===========================================================================================================
// ターゲットリストの削除
//===========================================================================================================
void My::CCard::RemoveTargetList(CActiveSceneCharacter* target_list)
{
	//サイズが0なら抜ける
	if (m_pTargetPlayerList.size() == 0)
	{
		return;
	}
	//敵の情報を削除
	m_pTargetPlayerList.remove(target_list);
}

//===========================================================================================================
// カードをマウスでキャスト
//===========================================================================================================
bool My::CCard::CardCastToMouse(CDuelCharacter* duel, CActiveSceneCharacter* player)
{
	// 選択状態とキャスト状態以外は通さない
	if (GetStateNum() != CCardState::CARD_PICKUP &&
		GetStateNum() != CCardState::CARD_CAST &&
		GetStateNum() != CCardState::CARD_SELECT)
	{
		return false;
	}

	// カメラ取得
	//CCamera* pCamera = GET_CAMERA(0);
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
		ChangeState(CCardState::CARD_SELECT, duel);
		CActiveSceneManager::GetInstance()->ChangeState(new CCardCast);

		screenpos = pMouse->GetMousePos();
		CalcScreenToWorld(&resultpos, screenpos.x, screenpos.y, 1.0f, static_cast<int>(width), static_cast<int>(height), &View, &Proj);
		resultpos.y += 20.0f;
		SetPos(resultpos);

		return true;
	}
	else if (pMouse->GetRelease(0))
	{
		//対象のエリア
		m_target = pMouse->GetArea();

		//キャンセルエリアなら解除
		if (m_target == CInputMouse::AREA::CENTER)
		{
			//通常状態にする
			ChangeState(CCardState::CARD_NEUTRAL, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
			return false;
		}

		//エナジーが不足しているなら抜ける
		if (player->GetEnergy() < m_BaseStatus.nCost)
		{
			// 通常状態にする
			ChangeState(CCardState::CARD_NEUTRAL, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
			return false;
		}

		//使用者のエリアの取得
		m_UserArea = player->GetArea();

		if (IsCast(duel))
		{
			// キャスト遷移
			ChangeState(CCardState::CARD_CAST, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);

			//オンライン時なら送信
			if (CRakNet::GetInstance()->GetOnline())
			{//TODO : カードの対象が複数になったら処理の変更の必要があり
				std::list<CActiveSceneCharacter*> list = CActiveSceneManager::GetInstance()->GetCharacterList();

				//対象を見つけて送信
				for (auto& itr : list)
				{
					if (itr == nullptr) { continue; }
					if (itr->GetArea() != m_target) { continue; }

					CRakNet::GetInstance()->SendCastCard(m_BaseStatus.nCardID, CActiveSceneManager::GetInstance()->GetPlayer()->GetPlayerIdx(), itr->GetPlayerIdx());

					break;
				}
			}

			return false;
		}
		else
		{
			//通常状態にする
			ChangeState(CCardState::CARD_NEUTRAL, duel);
			CActiveSceneManager::GetInstance()->ChangeState(new CDuel);
			return false;
		}
	}

	return false;
	
}

//===========================================================================================================
// マウスでカードを選択する
//===========================================================================================================
bool My::CCard::CardSelectToMouse(CDuelCharacter* duel)
{
	if (GetStateNum() == CCardState::CARD_CAST
		|| GetStateNum() == CCardState::CARD_STAY
		|| GetStateNum() == CCardState::CARD_WAIT
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
		ChangeState(CCardState::CARD_PICKUP, duel);
		return true;
	}
	else
	{
		// カード非選択状態にする
		ChangeState(CCardState::CARD_NEUTRAL, duel);
		return false;
	}

	return false;
}

//===========================================================================================================
// カードステータス読み込み
//===========================================================================================================
void My::CCard::LoadCardData()
{
	//TODO:ここに各ステータスの読み込み

	//読み込んだカード情報をリストに登録
	CDuelManager::GetInstance()->RegistUseCardList(this);
}

//===========================================================================================================
// カード情報読み込み
//===========================================================================================================
void My::CCard::LoadInfo(int id)
{
	std::vector<CCard_Client::Param> List = My::CCardManager::GetInstance()->GetUseCardVector();

	//リストから同じ番号を探す
	for (auto iter : List)
	{
		if (iter.nCardID == id)
		{
			//基本ステータスの読み込み
			m_BaseStatus.maintype = (CARDTYPE_)iter.Maintype;
			m_BaseStatus.name = iter.Name;
			m_BaseStatus.nCardID = iter.nCardID;
			m_BaseStatus.nPackID = iter.nPackID;
			m_BaseStatus.nCost = iter.nCost;
			m_BaseStatus.raritytype = (RARITY)iter.Raritytype;
			m_BaseStatus.ruby = iter.Ruby;
			m_BaseStatus.imagePath = iter.ImagePath;

			//固有情報の読み込み
			break;
		}
	}
}

//===========================================================================================================
// ステイ時間のリセット
//===========================================================================================================
void My::CCard::ResetStayTime(CDuelCharacter* duel)
{
	// TODO 2025/12/05 -----------------------------------------------
	//	キャスト時にどのプレイヤーにキャストしても
	//	ステイ時間がリセットされる不具合を修正する
	//----------------------------------------------------------------

	// カードプレビューゾーンのカードリストを取得
	std::list<CCard*> list = duel->GetZoneManager()->GetCastPreviewZone()->GetList();
	std::list<CActiveSceneCharacter*> charalist = CActiveSceneManager::GetInstance()->GetCharacterList();

	// 攻撃カードのステイ時間初期化
	for (auto& itr : list)
	{
		// 攻撃カード以外は通さない
		CCardAttack* attack = dynamic_cast<CCardAttack*>(itr);
		if (attack == nullptr)
			continue;

		// ステイ状態以外は通さない
		if(attack->GetStateNum() != CCardState::CARD_STAY)
			continue;

		for (auto& iter : charalist)
		{
			if (iter->GetArea() == attack->GetTarget())
				break;
		}

		// ステイ時間初期化
		attack->GetState()->Init(attack, duel);
	}
}

//===========================================================================================================
// 列挙からゾーンのポインタを返す
//===========================================================================================================
My::CZone* My::CCard::CastToEnumZone(ZONE zone, CDuelCharacter* duel)
{
	//返す用の変数
	CZone* pZone = nullptr;

	//昔のゾーンを保存
	if (zone != m_CurrentZone)
	{
		m_OldZone = m_CurrentZone;
	}
	else
	{
		return nullptr;
	}

	////ゾーン列挙
	//switch (m_OldZone)
	//{
	//case ZONE::DECK:	//デッキ
	//	pZone = duel->GetZoneManager()->GetDeck();
	//	m_CurrentZone = ZONE::DECK;	//列挙の登録
	//	break;

	//case ZONE::HAND:	//手札
	//	pZone = duel->GetZoneManager()->GetHandZone();
	//	m_CurrentZone = ZONE::HAND;	//列挙の登録
	//	break;

	//case ZONE::CAST:	//キャストプレビュー
	//	pZone = duel->GetZoneManager()->GetCastPreviewZone();
	//	m_CurrentZone = ZONE::CAST;	//列挙の登録
	//	break;

	//case ZONE::WAIT:	//待機
	//	pZone = duel->GetZoneManager()->GetWaitZone();
	//	m_CurrentZone = ZONE::WAIT;	//列挙の登録
	//	break;

	//case ZONE::FIELD:	//フィールド
	//	pZone = duel->GetZoneManager()->GetFieldZone();
	//	m_CurrentZone = ZONE::FIELD;	//列挙の登録
	//	break;

	//case ZONE::CEMETERY:	//墓地
	//	pZone = duel->GetZoneManager()->GetCemetery();
	//	m_CurrentZone = ZONE::CEMETERY;	//列挙の登録
	//	break;
	//}

	//引数の列挙をポインタに変換
	pZone = CastToZone(m_OldZone, duel);

	//現在のゾーンの更新
	m_CurrentZone = zone;

	return pZone;
}

//===========================================================================================================
//列挙に対応したゾーンのポインタを返す
//===========================================================================================================
My::CZone* My::CCard::CastToZone(ZONE zone, CDuelCharacter* duel)
{
	//返す用の変数
	CZone* pZone = nullptr;

	//ゾーン列挙
	switch (zone)
	{
	case ZONE::DECK:	//デッキ
		pZone = duel->GetZoneManager()->GetDeck();
		m_CurrentZone = ZONE::DECK;	//列挙の登録
		break;

	case ZONE::HAND:	//手札
		pZone = duel->GetZoneManager()->GetHandZone();
		m_CurrentZone = ZONE::HAND;	//列挙の登録
		break;

	case ZONE::CAST:	//キャストプレビュー
		pZone = duel->GetZoneManager()->GetCastPreviewZone();
		m_CurrentZone = ZONE::CAST;	//列挙の登録
		break;

	case ZONE::WAIT:	//待機
		pZone = duel->GetZoneManager()->GetWaitZone();
		m_CurrentZone = ZONE::WAIT;	//列挙の登録
		break;

	case ZONE::FIELD:	//フィールド
		pZone = duel->GetZoneManager()->GetFieldZone();
		m_CurrentZone = ZONE::FIELD;	//列挙の登録
		break;

	case ZONE::CEMETERY:	//墓地
		pZone = duel->GetZoneManager()->GetCemetery();
		m_CurrentZone = ZONE::CEMETERY;	//列挙の登録
		break;

	case ZONE::FLIP:	//墓地
		pZone = duel->GetZoneManager()->GetFlipPreviewZone();
		m_CurrentZone = ZONE::FLIP;	//列挙の登録
		break;
	}

	return pZone;
}
