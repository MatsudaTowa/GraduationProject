//=============================================
//
//カードを選択出来る範囲[selsection_range.cpp]
//Author Matsuda Towa
//
//=============================================
#include "selection_range.h"

//=============================================
// コンストラクタ
//=============================================
My::CSelectionRange::CSelectionRange(int nPriority):CObject2D(nPriority)
{
}

//=============================================
// デストラクタ
//=============================================
My::CSelectionRange::~CSelectionRange()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CSelectionRange::Init()
{
	SetVtx();
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CSelectionRange::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CSelectionRange::Update()
{
	CObject2D::Update();
	SetVtx();
}

//=============================================
// 描画
//=============================================
void My::CSelectionRange::Draw()
{
	CObject2D::Draw();
}

//=============================================
// 生成
//=============================================
My::CSelectionRange* My::CSelectionRange::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CSelectionRange* pObject = new CSelectionRange();

	//nullならnullを返す
	if (pObject == nullptr) { return nullptr; }

	//pos設定
	pObject->SetPos(pos);

	//タイプ設定
	pObject->SetSize(size);

	//初期化
	pObject->Init();

	return pObject;
}
