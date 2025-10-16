//============================================
//
// ビルボード[billbord.h]
// Author:Uedakou
// 
//============================================
#ifndef _OBJECT_BILLBORD_H_
#define _OBJECT_BILLBORD_H_
#include "../object.h"	// オブジェクト基底
#include "../../system/manager.h"	// 全体マネージャー

namespace My
{

	class CObjectBillbord : public CObject
	{
	public:
		CObjectBillbord();
		CObjectBillbord(const int nPriority);
		virtual ~CObjectBillbord();
		virtual HRESULT Init()		override;
		virtual void Uninit()	override;
		virtual void Update()	override;
		virtual void Draw()		override;
		void SetPos(D3DXVECTOR3 pos) { CObject::SetPos(pos); }


		void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtx) { m_pVtxBuff = pVtx; }
		void SetTexture(const char aFileName[MAX_TXT]);
		void SetTexture(const LPDIRECT3DTEXTURE9 pTex);
		void SetTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1);// テクスチャUV設定(左上右下)
		void SetColor(D3DXCOLOR col);


		LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxBuff; }// バーテックスバッファ取得
		LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }	// テクスチャ取得
		D3DXMATRIX GetMaxWorld() { return m_mtxWorld; }	// ワールドマトリクス取得
		D3DXCOLOR GetColor() { return m_col; }	// 色取得

		void AddTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1);// テクスチャUV設定(左上右下)
		static CObjectBillbord* create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);
		static CObjectBillbord* create(int nPriorith, D3DXVECTOR3 pos, D3DXVECTOR3 siz);
	private:
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バーテックスバッファ
		LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ
		D3DXMATRIX m_mtxWorld;		// ワールドマトリクス
		D3DXCOLOR m_col;
	};


}

#endif // !_OBJECT_BILLBORD_H_