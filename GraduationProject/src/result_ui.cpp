//=============================================
//
//リザルト[result_ui.cpp]
//Author Kasai Keisuke
//
//=============================================
#include "result_ui.h"

//=============================================
//コンストラクタ
//=============================================
My::CResult_Ui::CResult_Ui(int nPriority):CObject2D(nPriority)
{

}

//=============================================
//デストラクタ
//=============================================
My::CResult_Ui::~CResult_Ui()
{

}

//=============================================
//初期化処理
//=============================================
HRESULT My::CResult_Ui::Init()
{
	//親クラスの初期化処理を呼ぶ
	CObject2D::Init();

	//自分自身のサイズ取得
	D3DXVECTOR2 size = GetSize();
	
	size = D3DXVECTOR2(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.1f);

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
void My::CResult_Ui::Uninit()
{
	//親クラスの終了処理を呼ぶ
	CObject2D::Uninit();
}

//=============================================
//更新処理
//=============================================
void My::CResult_Ui::Update()
{
	// 親クラスの更新処理を呼ぶ
	CObject2D::Update();
}

//=============================================
//描画処理
//=============================================
void My::CResult_Ui::Draw()
{
	// 親クラスの描画処理を呼ぶ
	CObject2D::Draw();
}

//=============================================
//生成処理
//=============================================
My::CResult_Ui* My::CResult_Ui::Create(D3DXVECTOR3 pos)
{
	CResult_Ui* pResult_Ui = new CResult_Ui();

	//nullならnullを返す
	if (pResult_Ui == nullptr) { return nullptr; }

	//pos設定
	pResult_Ui->SetPos(pos);

	//タイプ設定
	pResult_Ui->SetType(OBJECT_TYPE_RESULT);

	//初期化
	pResult_Ui->Init();

	return pResult_Ui;
}
