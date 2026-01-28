//=============================================
//
//カウントダウンナンバー表示UI[countdown_start_UI.cpp]
//Author Matsuda Towa
//
//=============================================

//インクルード
#include "countdown_start_UI.h"
#include "duel_manager.h"

//宣言
const std::string My::CCountdownStartUI::TEX_NAME = "data\\TEXTURE\\duel000.png";

//=============================================
// コンストラクタ
//=============================================
My::CCountdownStartUI::CCountdownStartUI(int nPriority) :CCountdownUI(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CCountdownStartUI::~CCountdownStartUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCountdownStartUI::Init()
{
	SetSize({ START_SIZE,START_SIZE });
	SetColor(COLOR_WHITE);
	
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCountdownStartUI::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCountdownStartUI::Update()
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
		SetColor({ 1.0f, 1.0f, 1.0f, GetLife() * 10.0f });
	}

	CCountdownUI::Update();
}

//=============================================
// 描画
//=============================================
void My::CCountdownStartUI::Draw()
{
	//親の描画
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CCountdownStartUI* My::CCountdownStartUI::Create(D3DXVECTOR3 pos)
{
	
	CCountdownStartUI* pUI = new CCountdownStartUI;
	if (pUI == nullptr) { return nullptr; }

	// SE(DUEL)追加
	My::CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_SE_DUEL);

	//TODO : ランダムのアルゴリズムを作る
	//BGMの設定
	CManager::GetInstance()->GetSound()->PlaySound(CSound::SOUND_LABEL_BGM_DUEL);

	pUI->SetPos(pos);
	pUI->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(CCountdownStartUI::TEX_NAME)));
	pUI->Init();
	
	return pUI;
}
