//============================================
//
// ポリゴン３D[object_3D.h]
// Author:Uedakou
// 
//============================================
#ifndef _OBJECT_3D_H_
#define _OBJECT_3D_H_

#include "../object.h"	// オブジェクト基底クラス
#include "../../system/manager.h"	
#include <stdio.h>
namespace My
{
	class CObject3D : public CObject
	{
	public:

		CObject3D();
		CObject3D(const int nPriority);
		virtual ~CObject3D()	override;
		virtual HRESULT Init()		override;
		virtual void Uninit()	override;
		virtual void Update()	override;
		virtual void Draw()		override;

		void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtxBuff) { m_pVtxBuff = pVtxBuff; }

		void SetTexture(const char aFileName[MAX_TXT]);
		void SetTexture(const LPDIRECT3DTEXTURE9 pTex);

		void SetTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1);// テクスチャUV設定(左上右下)
		void SetColor(D3DXCOLOR col);
		void SetBlock(int X, int Y);

		void SetSiz(const D3DXVECTOR3 siz) { m_siz = siz; }
		void SetSizX(const float x) { m_siz.x = x; }
		void SetSizY(const float y) { m_siz.y = y; }
		void SetSizZ(const float z) { m_siz.z = z; }

		LPDIRECT3DVERTEXBUFFER9* GetVtxBuff() { return &m_pVtxBuff; }

		LPDIRECT3DTEXTURE9 GetTexture() { return m_pTexture; }

		D3DXCOLOR GetColor() { return m_col; }

		void AddTexUV(D3DXVECTOR2 tex0, D3DXVECTOR2 tex1);// テクスチャUV設定(左上右下)

		static CObject3D* create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 siz);
	private:
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// バーテックスバッファ
		LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャ
		D3DXMATRIX m_mtxWorld;		// ワールドマトリクス
		D3DXCOLOR m_col;
		int BlockX;
		int BlockY;
		D3DXVECTOR3 m_siz;	// 大きさ

	};

}
#endif // _OBJECT_3D_H_