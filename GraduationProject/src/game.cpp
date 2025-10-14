//=============================================
//
//ゲーム[game.cpp]
//Author Matsuda Towa
//
//=============================================
#include "game.h"
#include "game_field.h"
#include "game_player.h"
#include "game_camera.h"
#include "active_manager.h"
#include "enemy.h"
#include "card.h"

namespace
{
	const D3DXVECTOR3 FIELD_SIZE = { 200.0f,0.0f,200.0f };
	const int COOL_TIME = 1;
	const int DELAY_FRAME = 90;
}

//=============================================
//コンストラクタ
//=============================================
My::CGame::CGame():
m_pPauseCnt(nullptr),				//ポーズのキーを有効化するためのカウント
m_pDelayCnt(nullptr)				//リザルトに遷移するまでのディレイカウント
{
	CGameManager::GetInstance()->SetFinish(false);
}

//=============================================
//デストラクタ
//=============================================
My::CGame::~CGame()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CGame::Init()
{
	CREATE_CAMERA(new CGameCamera);

	GET_CAMERA(0)->ChangeCameraState(new CBirdView);

	SET_CAMERA_IDX(0);

	CGameState* current_state = CGameManager::GetInstance()->GetState();
	if (current_state == nullptr)
	{
		current_state = new CNormal;
	}		
	CGameManager::GetInstance()->SetState(current_state);

	if (m_pPauseCnt == nullptr)
	{
		m_pPauseCnt = new CCount;
		m_pPauseCnt->SetFrame(COOL_TIME);
	}

	if (m_pDelayCnt == nullptr)
	{
		m_pDelayCnt = new CCount;
		m_pDelayCnt->SetFrame(DELAY_FRAME);
	}
	//地面生成
	CField::Create(VEC3_RESET_ZERO, FIELD_SIZE,new CGameField);

	//プレイヤー生成
	CPlayer::Create(new CGamePlayer);

	CEnemy::Create({ 300.0f,0.0f,00.0f }, { 0.0f,1.75f,0.f });
	CEnemy::Create({ -300.0f,0.0f,00.0f }, { 0.0f,-1.75f,0.f });
	CEnemy::Create({ 0.0f,0.0f,250.0f }, { 0.0f,0.0f,0.f });

	//CCard::Create();

	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CGame::Uninit()
{
	////ジェイソンの情報削除
	//if (CJson::GetJson() != nullptr)
	//{
	//	delete CJson::GetJson();
	//	CJson::GetJson() = nullptr;
	//}

	if (m_pDelayCnt != nullptr)
	{
		delete m_pDelayCnt;
		m_pDelayCnt = nullptr;
	}

	if (m_pPauseCnt != nullptr)
	{
		delete m_pPauseCnt;
		m_pPauseCnt = nullptr;
	}

	//ステート初期化
	CGameManager::GetInstance()->ChangeState(new CNormal);

	//マネージャーで管理しているオブジェクトをここで削除
	//NOTE: オブジェクトのReleaseAll前に消すことで二重で削除することを防止
	CGameManager::GetInstance()->Uninit();

	CObject::ReleaseAll();
}

//=============================================
//更新
//=============================================
void My::CGame::Update()
{
	if (!CGameManager::GetInstance()->GetFinish())
	{
		//ポーズのカウントアップ
		m_pPauseCnt->CountUp();
//
//#ifdef _DEBUG
		//インプット取得
		CInputKeyboard* pKeyboard = GET_INPUT_KEYBOARD;
		CInputMouse* pMouse = GET_INPUT_MOUSE;

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{
			GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
		}
//#endif // _DEBUG


		CGameManager::GetInstance()->GameStateExecution(this);
		return;
	}

	if (!m_pDelayCnt->CountUp())
	{
		return;
	}
	//ステート初期化
	CGameManager::GetInstance()->ChangeState(new CNormal);
	GET_FADE->SetFade(CScene::MODE::MODE_RESULT);
}

//=============================================
//オブジェクトの更新を行うか決定
//=============================================
void My::CGame::StopObject(bool isStop)
{
	//プライオリティの数だけ周回
	for (int i = 0; i < CObject::PRI_MAX; i++)
	{
		CObject* pObj = CObject::GetTopObject(i);	//先頭取得

		//最大数が不明なのでwhileを使用
		while (pObj != nullptr)
		{
			CObject* pNext = pObj->GetNext();	//次のポインタを取得

			if (pObj->GetStop() != isStop)
			{
				pObj->SetStop(isStop);
			}

			pObj = pNext;					//ポインタを進める
		}
	}
}

//=============================================
//描画
//=============================================
void My::CGame::Draw()
{
	GET_CAMERA(GET_CAMERA_IDX)->SetCamera();
}
