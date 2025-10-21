//=============================================
//
//エリア[area.cpp]
//Author Matsuda Towa
//
//=============================================
#include "area.h"
#include "active_manager.h"
namespace
{
	const D3DXCOLOR DEFAULT_COLOR = { 0.0f,0.0f,0.0f,0.7f };
}

//=============================================
// コンストラクタ
//=============================================
My::CArea::CArea(int nPriority):CObject2D_Triangle(nPriority),
m_isSelect(false),
m_pCharacter(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CArea::~CArea()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CArea::Init()
{
	CObject2D_Triangle::Init();
	SetColor(DEFAULT_COLOR);
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CArea::Uninit()
{
	CObject2D_Triangle::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CArea::Update()
{
	SetVtx();

	if (!CGameManager::GetInstance()->GetAreaManager()->GetActive())
	{
		SetColor(COLOR_NONE);
		return;
	}

	if (m_pCharacter != nullptr)
	{
		if (m_pCharacter->GetIsDelete())
		{
			m_pCharacter = nullptr;
			return;
		}
	}

	if (m_pCharacter == nullptr)
	{
		SetColor(DEFAULT_COLOR);
		return;
	}

	if (!m_isSelect || m_pCharacter->GetLife() <= INT_ZERO)
	{
		SetColor(DEFAULT_COLOR);
		return;
	}

	SetColor(COLOR_NONE);

	//TODO:ここに選択されたカードの処理を！
	if (GET_INPUT_MOUSE->GetTrigger(0))
	{
		int life = m_pCharacter->GetLife();
		if (life > INT_ZERO)
		{
			--life;
		}
		m_pCharacter->SetLife(life);

		//NOTE:キャラクター削除方法の例
		//SetisDeleteにtrueを代入することでobject側でUniit()が呼ばれる
		//エネミーはマネージャーに登録されているリストから削除してあげないと最終の全削除でサイズ数と実際の要素数が合わずに落ちるので必ずRemoveで削除してください
		if (life <= 0)
		{
			m_pCharacter->SetisDelete(true);
			if (typeid(*m_pCharacter) == typeid(CEnemy))
			{
				CEnemy* enemy = dynamic_cast<CEnemy*>(m_pCharacter);
				CGameManager::GetInstance()->GetEnemyManager()->Remove(enemy);
			}

			m_pCharacter = nullptr;
		}
	}
}

//=============================================
// 描画
//=============================================
void My::CArea::Draw()
{
	CObject2D_Triangle::Draw();
}

//=============================================
// 生成
//=============================================
My::CArea* My::CArea::Create(D3DXVECTOR2 triangle_vtx[NUM_VERTEX])
{
	CArea* area = new CArea;
	if (area == nullptr)
	{
		return nullptr;
	}
	for (int i = 0; i < NUM_VERTEX; ++i)
	{
		area->SetTriangleVtx(triangle_vtx[i], i);
	}
	area->Init();
	return area;
}
