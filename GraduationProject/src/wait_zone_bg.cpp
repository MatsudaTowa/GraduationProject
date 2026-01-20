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
	const D3DXVECTOR2 UI_SIZE{ 51.0f, 80.0f };	//サイズ
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

	for (int i = 0; i < NUM_CARD; ++i)
	{
		D3DXVECTOR3 pos = GetPos();
		pos.x += (NUM_CARD - 1 - i) * 10.0f;

		for (int j = 0; j < CCardFrame::FRAMETYPE_MAX; ++j)
		{
			m_pPseundCard[i].card_frame[j] =
				CPsendCardFrame::Create(
					(CCardFrame::FRAMETYPE)j,
					pos,
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

	//カラー取得
	D3DXCOLOR col = GetColor();

	col = {0.8f,0.8f,0.8f,1.0f};

	//カラーを代入
	SetColor(col);

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
	//親クラスの終了処理を呼ぶ
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CWaitZoneBG::Update()
{
	// 親クラスの更新処理を呼ぶ
	CObject2D::Update();

	//頂点設定
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CWaitZoneBG::Draw()
{	
	// 親クラスの描画処理を呼ぶ
	CObject2D::Draw();
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
		bool isView = duel_state->GetIsCemeteryView();
		isView = isView ? false : true;
		duel_state->SetIsWaitView(isView);

		zone->GetSelectionRange()->SetisDraw(isView);
	}
}

bool My::CWaitZoneBG::ProcessMouseEvent()
{
	return false;
}

void My::CWaitZoneBG::CardisView()
{
}

//=============================================
// 生成
//=============================================
My::CWaitZoneBG* My::CWaitZoneBG::Create(D3DXVECTOR3 pos)
{
	CWaitZoneBG* pObject = new CWaitZoneBG();

	//nullならnullを返す
	if (pObject == nullptr) { return nullptr; }

	//pos設定
	pObject->SetPos(pos);

	//初期化
	pObject->Init();

	return pObject;
}
