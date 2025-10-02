//=============================================
//
//3DTemplate[field.cpp]
//Author Matsuda Towa
//
//=============================================
#include "field.h"
#include "manager.h"

namespace
{
	std::string TEX_NAME = "data\\TEXTURE\\tatami.jpeg";
}

//=============================================
//コンストラクタ
//=============================================
My::CField::CField(int nPriority) : CObject3D(nPriority)
{
}

//=============================================
//デストラクタ
//=============================================
My::CField::~CField()
{
}

//=============================================
//初期化
//=============================================
HRESULT My::CField::Init()
{
	SetColor(COLOR_GREEN);

	//テクスチャ設定
	CTexture* pTexture = GET_TEXTURE;
	BindTexture(pTexture->GetAddress(pTexture->Regist(&TEX_NAME)));

	//頂点設定
	SetVtx(D3DXVECTOR3(0.0f,1.0f,0.0f));
	return S_OK;
}

//=============================================
//終了
//=============================================
void My::CField::Uninit()
{
	CObject3D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CField::Update()
{

}

//=============================================
//描画
//=============================================
void My::CField::Draw()
{
	// 親クラスの描画
	CObject3D::Draw();
}

//=============================================
//  生成
//=============================================
My::CField* My::CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, CField* field)
{
	CField* pField = field;

	// nullならnullを返す
	if (pField == nullptr) { return nullptr; }

	//テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	pField->SetPos(pos); //pos設定
	pField->SetSize(size); //サイズ設定
	//pField->BindTexture(pTexture->GetAddress(pTexture->Regist(&TEXTURE_NAME))); //テクスチャ設定
	pField->SetType(OBJECT_TYPE_FIELD); //タイプ設定
	pField->Init(); //床の初期化処理

	return pField;
}