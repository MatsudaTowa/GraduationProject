//=============================================
//
//カウントダウンナンバー表示UI[countdown_number_UI.cpp]
//Author Matsuda Towa
//
//=============================================

//インクルード
#include "countdown_number_UI.h"
#include "duel_manager.h"

//宣言
const std::string My::CCountdownNumberUI::TEX_NAME = "data\\TEXTURE\\countdown001.png";

//=============================================
// コンストラクタ
//=============================================
My::CCountdownNumberUI::CCountdownNumberUI(int nPriority) :CCountdownUI(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCountdownNumberUI::~CCountdownNumberUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCountdownNumberUI::Init()
{
	SetSize({ START_SIZE,START_SIZE });
	SetColor(COLOR_WHITE);
	SetDivision(3);
	SetVtx();

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCountdownNumberUI::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCountdownNumberUI::Update()
{
	//変数
	float fCurrentSize = GetSize().x;

	//進行時間分サイズを加算
	fCurrentSize += My::CDuel_Manager::GetInstance()->GetDuelTimer().GetdeltaTime() * ADD_SIZE;

	//サイズの設定
	SetSize({ fCurrentSize, fCurrentSize });

	//消え始める寿命になったら徐々に透明にする
	if (GetLife() <= START_DELETE_LIFE)
	{
		SetColor({ 1.0f, 1.0f, 1.0f, GetLife()});
	}

	CCountdownUI::Update();
}

//=============================================
// 描画
//=============================================
void My::CCountdownNumberUI::Draw()
{
	//親の描画
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CCountdownNumberUI* My::CCountdownNumberUI::Create(D3DXVECTOR3 pos, NUMBER num)
{
	//0は無いので飛ばす
	if (num == ZERO) return nullptr;

	CCountdownNumberUI* pUI = new CCountdownNumberUI;
	if (pUI == nullptr) { return nullptr; }

	// SE(3・2・1)追加
	My::CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_COUNT_DOWN);

	pUI->SetPos(pos);
	pUI->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(CCountdownNumberUI::TEX_NAME)));
	pUI->Init();
	pUI->SetTexPos({(1.0f / 3.0f) * (num - 1), 1.0f });

	return pUI;
}
