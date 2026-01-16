//=============================================
//
// リザルト用ランキングポリゴン[result_ranking.h]
// Author Ueda Kou
//
//=============================================
#include "result_ranking.h"	// リザルトランキング
//=============================================
// コンストラクタ
//=============================================
My::CResultRanking::CResultRanking()
{
}
//=============================================
// デストラクタ
//=============================================
My::CResultRanking::~CResultRanking()
{
}
//=============================================
// 初期化
//=============================================
HRESULT My::CResultRanking::Init()
{
 	CObject2D::Init();

	SetPos(VEC3_RESET_ZERO);	// 
	SetSize(D3DXVECTOR2(50.0f, 50.0f));
	SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetVtx();

	return S_OK;
}
//=============================================
// 生成
//=============================================
My::CResultRanking* My::CResultRanking::Create()
{
	CResultRanking* pResultRanking = new CResultRanking();

	if (FAILED(pResultRanking->Init()))
	{
		pResultRanking->Release();
		return nullptr;
	}
	
	return pResultRanking;
}
