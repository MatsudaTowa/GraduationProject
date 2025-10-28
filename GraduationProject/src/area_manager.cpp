//================================
//
//area_manager.cppに必要な宣言[area_manager.cpp]
//Author:松田永久
// 
//================================
#include "area_manager.h"
#include "active_manager.h"
#include "raknet.h"

//================================
// コンストラクタ
//================================
My::CAreaManager::CAreaManager():m_isActive(false),
m_pArea(),
m_pCenterArea()
{
}

//================================
// デストラクタ
//================================
My::CAreaManager::~CAreaManager()
{
}

//================================
// 初期化
//================================
HRESULT My::CAreaManager::Init()
{
	return S_OK;
}

//================================
// 終了
//================================
void My::CAreaManager::Uninit()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] == nullptr) { continue; }
		m_pArea[i]->Uninit();
		m_pArea[i] = nullptr;
	}

	if (m_pCenterArea == nullptr) { return; }
	m_pCenterArea->Uninit();
	m_pCenterArea = nullptr;
}


//=============================================
//エリア生成
//=============================================
void My::CAreaManager::CreateArea()
{
	for (int i = 0; i < CInputMouse::AREA::MAX - 1; ++i)
	{
		if (m_pArea[i] != nullptr) { continue; }

		//三角形の頂点座標を指定
		D3DXVECTOR2 triangle_vtx[CObject2D_Triangle::NUM_VERTEX];

		//ウィンドウの中心にを必ず頂点に
		D3DXVECTOR2 center = { SCREEN_WIDTH * HALF,SCREEN_HEIGHT * HALF };
		triangle_vtx[1] = center;

		//それぞれの頂点位置を指定
		switch (i)
		{
		case CInputMouse::AREA::UP:
			triangle_vtx[0] = { SCREEN_WIDTH,FLOAT_ZERO };
			triangle_vtx[2] = { VEC2_RESET_ZERO };
			break;
		case CInputMouse::AREA::DOWN:
			triangle_vtx[0] = { FLOAT_ZERO,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::LEFT:
			triangle_vtx[0] = { FLOAT_ZERO,FLOAT_ZERO };
			triangle_vtx[2] = { FLOAT_ZERO,SCREEN_HEIGHT };
			break;
		case CInputMouse::AREA::RIGHT:
			triangle_vtx[0] = { SCREEN_WIDTH,SCREEN_HEIGHT };
			triangle_vtx[2] = { SCREEN_WIDTH,FLOAT_ZERO };
			break;
		default:
			break;
		}

		m_pArea[i] = CArea::Create(triangle_vtx);
	}

	if (m_pCenterArea != nullptr) { return; }
	m_pCenterArea = CCenterArea::Create();
	m_pCenterArea->Init();
}

//=============================================
//エリア選択
//=============================================
void My::CAreaManager::SelectArea()
{
	CInputMouse::AREA area = GET_INPUT_MOUSE->GetArea();

	if (area == CInputMouse::AREA::CENTER)
	{
		m_pCenterArea->SetSelect(true);

		//TODO:カードを離したらに変更予定
		if (GET_INPUT_MOUSE->GetTrigger(0))
		{
			CGameManager::GetInstance()->ChangeState(new CDuel);
		}
	}
	else
	{
		m_pArea[area]->SetSelect(true);	

		//TODO:ここに選択されたカードの処理を！
		if (GET_INPUT_MOUSE->GetTrigger(0))
		{
			//登録されているキャラクターを取得
			CGamePlayer* player = CGameManager::GetInstance()->GetPlayer();
			std::list<CEnemy*> enemy_list = CGameManager::GetInstance()->GetEnemyManager()->GetList();
			int life;
			player_cast(player, area, life);

			for (auto& itr : enemy_list)
			{
				if (itr == nullptr) { continue; }

				if (itr->GetArea() != area) { continue; }
				
				life = itr->GetLife();
				if (life > INT_ZERO)
				{//TODO:選択できない旨のUI表示
					--life;
				}
				itr->SetLife(life);
			}

			if (!CRakNet::GetInstance()->GetOnline()) return;
			//通信処理
			CRakNet::GetInstance()->SendStatus();
		}
	}

	for (int i = 0; i < CInputMouse::AREA::MAX; ++i)
	{
		if (i != area)
		{//選択されているエリア以外の
			if (i == CInputMouse::AREA::CENTER)
			{//真ん中だったら
				m_pCenterArea->SetSelect(false);
			}
			else
			{//三角形だったら
				m_pArea[i]->SetSelect(false);
			}
		}
	}
}

void My::CAreaManager::player_cast(CGamePlayer* player, My::CInputMouse::AREA area, int& life)
{
	if (player == nullptr)
	{
		return;
	}
	if (player->GetArea() == area)
	{
		life = player->GetLife();
		if (life > INT_ZERO)
		{
			--life;
		}
		player->SetLife(life);
	}
}
