//=============================================
//
//何Pかの表示UI[damage_number_UI.cpp]
//Author Matsuda Towa
//
//=============================================
#include "damage_number_UI.h"
#include "raknet.h"
#include "duel_manager.h"

//宣言
const std::string My::CDamageNumberUI::TEX_NAME = "data\\TEXTURE\\number000.png";

//=============================================
// コンストラクタ
//=============================================
My::CDamageNumberUI::CDamageNumberUI(int nPriority) :CObject2D(nPriority),
	m_nLife(1000)
{
}

//=============================================
// デストラクタ
//=============================================
My::CDamageNumberUI::~CDamageNumberUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CDamageNumberUI::Init()
{
	SetSize({ SIZE,SIZE });
	SetColor(COLOR_WHITE);
	SetDivision(10);

	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CDamageNumberUI::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CDamageNumberUI::Update()
{
	//CObject2D::Update();
	//SetVtx();

	//経過時間を取得
	if (CRakNet::GetInstance()->GetOnline())
	{
		m_nLife -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();
	}
	else
	{
		m_nLife -= 17;	//オフライン用
	}

	//0以下なら破棄
	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================
// 描画
//=============================================
void My::CDamageNumberUI::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CDamageNumberUI* My::CDamageNumberUI::Create(D3DXVECTOR3 pos, int num)
{
	CDamageNumberUI* pUI = new CDamageNumberUI;
	if (pUI == nullptr) { return nullptr; }
	pUI->SetPos(pos);
	pUI->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(CDamageNumberUI::TEX_NAME)));
	pUI->Init();
	pUI->SetTexPos({ num * 0.1f, 1.0f });
	return pUI;
}
