//=============================================
//
//数字管理「zone_num.cpp」
// Author松田永久
//
//=============================================
#include "zone_num.h"

My::CZoneNum* My::CZoneNum::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int idx)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	CZoneNum* pNumber = new CZoneNum;

	// nullならnullを返す
	if (pNumber == nullptr) { return nullptr; }

	pNumber->SetPos(pos); //pos設定

	pNumber->SetSize(size); //size設定

	pNumber->SetType(OBJECT_TYPE_NUMBER); //タイプ設定

	pNumber->BindTexture(pTexture->GetAddress(pTexture->Regist(TEXTURE_NAME[idx]))); //テクスチャ設定

	pNumber->Init(); //初期化処理

	return pNumber;
}
