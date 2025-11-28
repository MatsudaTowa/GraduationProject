//=============================================
//
//待機チェックUI[waitchecker_UI.cpp]
//Author Kasai Keisuke
//
//=============================================
#include "waitchecker_UI.h"

//無名空間
namespace
{
	const D3DXVECTOR2 UI_SIZE{ 20.0f, 20.0f };	//サイズ
	const std::string TEXTURE_PATH[2] =	//テクスチャパス
	{
		{ "data\\TEXTURE\\not_leady.png" },
		{ "data\\TEXTURE\\leady.png" }
	};
	
}

//=============================================
//コンストラクタ
//=============================================
My::CWaitChecker_UI::CWaitChecker_UI(int nPriority) :CObject2D(nPriority),
m_isWaited(false)	//待機フラグ
{

}

//=============================================
//デストラクタ
//=============================================
My::CWaitChecker_UI::~CWaitChecker_UI()
{

}

//=============================================
//初期化処理
//=============================================
HRESULT My::CWaitChecker_UI::Init()
{
	//親クラスの初期化処理を呼ぶ
	CObject2D::Init();

	//テクスチャ情報の取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャ設定
	BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_PATH[m_isWaited])));

	//自分自身のサイズ取得
	D3DXVECTOR2 size = UI_SIZE;

	//テクスチャ座標設定
	SetTexPos(VEC2_RESET_ONE);

	//サイズを代入
	SetSize(size);

	//カラー取得
	D3DXCOLOR col = GetColor();

	col = COLOR_WHITE;

	//カラーを代入
	SetColor(col);

	//頂点設定
	SetVtx();

	return S_OK;
}

//=============================================
//終了処理
//=============================================
void My::CWaitChecker_UI::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CObject2D::Uninit();
}

//=============================================
//更新処理
//=============================================
void My::CWaitChecker_UI::Update()
{
	// 親クラスの更新処理を呼ぶ
	CObject2D::Update();

	SetVtx();
}

//=============================================
//描画処理
//=============================================
void My::CWaitChecker_UI::Draw()
{
	// 親クラスの描画処理を呼ぶ
	CObject2D::Draw();
}

//=============================================
//生成処理
//=============================================
My::CWaitChecker_UI* My::CWaitChecker_UI::Create(D3DXVECTOR3 pos)
{
	CWaitChecker_UI* pObject = new CWaitChecker_UI();

	//nullならnullを返す
	if (pObject == nullptr) { return nullptr; }

	//pos設定
	pObject->SetPos(pos);

	//タイプ設定
	pObject->SetType(OBJECT_TYPE_NUMBER);

	//初期化
	pObject->Init();

	return pObject;
}

//=============================================
//フラグの設定
//=============================================
void My::CWaitChecker_UI::SetIsWaited(bool wait)
{
	//テクスチャ情報の取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//引数を代入
	m_isWaited = wait;

	//テクスチャ設定
	BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_PATH[m_isWaited])));
}