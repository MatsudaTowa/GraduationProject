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
const std::string My::CDamageNumberUI::TEX_NAME = "data\\TEXTURE\\number.png";

//=============================================
// コンストラクタ
//=============================================
My::CDamageNumberUI::CDamageNumberUI(int nPriority) :CObject2D(nPriority),
	m_fLife(1.0f),
	m_pState(nullptr)
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
	SetSize({ 0,0 });
	SetColor(COLOR_RED);
	SetDivision(10);

	SetColor({ GetColor().r,GetColor().g,GetColor().b,0.0f });

	SetVtx();

	m_pState = new My::Start;
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CDamageNumberUI::Uninit()
{
	if(m_pState != nullptr)
	{
		delete m_pState;
		m_pState = nullptr;
	}

	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CDamageNumberUI::Update()
{
	//CObject2D::Update();
	SetTexVtx();

	//-------------------------------------------
	// TODO : UIイメージ
	// シャドウバースみたいなダメージ表記
	// ぶぉんとでてきてぶわっと消える感じ
	//-------------------------------------------

	//// 少し上に移動
	//D3DXVECTOR3 pos = GetPos();
	//pos.y -= 0.5f;
	//SetPos(pos);

	m_pState->Update(this);

	////透明度を上げる
	//D3DXCOLOR col = GetColor();
	//col.a += 0.05f;
	//SetColor(col);

	////経過時間を取得
	//if (CRakNet::GetInstance()->GetOnline())
	//{
	//	m_fLife -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();
	//}
	//else
	//{
	//	m_fLife -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();	//オフライン用
	//}

	////0以下なら破棄
	//if (m_fLife <= 0.0f)
	//{
	//	Uninit();
	//}
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

//=============================================
// UI状態：Start更新
//=============================================
void My::Start::Update(CDamageNumberUI* ui)
{
	// サイズを大きくする
	if (ui->GetSize().x < MAX_SIZE)
	{
		float size = ui->GetSize().x + 5.0f;
		ui->SetSize({ size,size });
	}
	
	//透明度を上げる
	D3DXCOLOR col = ui->GetColor();
	col.a += 0.07f;
	ui->SetColor(col);

	if (ui->GetSize().x >= MAX_SIZE && ui->GetColor().a > 1.0f)
	{
		// サイズが最大になったらWait状態に移行
		ui->SetState(new My::Wait);
	}

}

//=============================================
// UI状態：Wait更新
//=============================================
void My::Wait::Update(CDamageNumberUI* ui)
{
	// 経過時間を取得
	if (CRakNet::GetInstance()->GetOnline())
	{
		m_fWaitTime -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();
	}
	else
	{
		m_fWaitTime -= My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime();	//オフライン用
	}

	// 待機時間が過ぎたらEnd状態に移行
	if (m_fWaitTime <= 0.0f)
	{
		ui->SetState(new My::End);
	}
}

//=============================================
// UI状態：End更新
//=============================================
void My::End::Update(CDamageNumberUI* ui)
{
	// 透明度を下げる
	D3DXCOLOR col = ui->GetColor();
	col.a -= 0.03f;
	ui->SetColor(col);

	// 透明になったら消す
	if(col.a <= 0.0f)
	{
		ui->Uninit();
	}
}
