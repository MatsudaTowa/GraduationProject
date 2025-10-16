//===========================================
// 
// ポリゴン2[object_2D.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_
//#include "../../system/pch.h"	// プリコンパイル
#include "../object.h"	// オブジェクト
#include <stdio.h>

namespace My
{

	class CObject2D : public CObject
	{
	public:
		CObject2D();
		CObject2D(int nPriority);
		virtual ~CObject2D()	override;
		virtual HRESULT Init()		override;	// 初期化
		virtual void Uninit()	override;	// 終了
		virtual void Update()	override;	// 更新
		virtual void Draw()		override;	// 描画

		// 設定
		virtual void SetX(X x)		override;
		virtual void SetX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 siz) override;
		virtual void SetPos(const D3DXVECTOR3 pos) override;	// 位置設定
		virtual void SetPosX(const float x) override;	// 位置設定
		virtual void SetPosY(const float y) override;	// 位置設定
		virtual void SetPosZ(const float z) override;	// 位置設定
		virtual void SetRot(const D3DXVECTOR3 rot) override;	// 向き設定
		virtual void SetRotX(const float x) override;	// 向き設定
		virtual void SetRotY(const float y) override;	// 向き設定
		virtual void SetRotZ(const float z) override;	// 向き設定
		virtual void SetScl(const D3DXVECTOR3 siz) override;	// 大きさ(倍率)設定
		virtual void SetSclX(const float x) override;	// 大きさ(倍率)設定
		virtual void SetSclY(const float y) override;	// 大きさ(倍率)設定
		virtual void SetSclZ(const float z) override;	// 大きさ(倍率)設定
		virtual void SetSiz(const D3DXVECTOR3 siz) { m_siz = siz; };	// 大きさ設定
		virtual void SetSizX(const float x) { m_siz.x = x; };	// 大きさ設定
		virtual void SetSizY(const float y) { m_siz.y = y; };	// 大きさ設定
		virtual void SetSizZ(const float z) { m_siz.z = z; };	// 大きさ設定
		virtual void SetUV(D3DXVECTOR4 UV);	// UV設定


		void SetColor(D3DXCOLOR col);	// 色設定
		void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 pVtx) { m_pVtxBuff = pVtx; }	// バーテックスバッファ設定
		void SetTexture(const LPDIRECT3DTEXTURE9 ptex);	// テクスチャ設定
		void SetTexture(std::string aName);				// テクスチャ設定

		LPDIRECT3DVERTEXBUFFER9 GetVtxBuff() { return m_pVtxBuff; }	// バッファ取得
		D3DXCOLOR GetColor() { return m_col; }	// 色取得
		const D3DXVECTOR3 GetSiz() { return m_siz; }	// 大きさ取得

		// 加算
		virtual void AddX(X x) override;	// トランスフォーム設定
		virtual void AddX(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 siz) override;	// トランスフォーム設定
		virtual void AddPos(const D3DXVECTOR3 pos) override;	// 位置設定
		virtual void AddPosX(const float x) override;	// 位置設定
		virtual void AddPosY(const float y) override;	// 位置設定
		virtual void AddPosZ(const float z) override;	// 位置設定
		virtual void AddRot(const D3DXVECTOR3 rot) override;	// 向き設定
		virtual void AddRotX(const float x) override;	// 向き設定
		virtual void AddRotY(const float y) override;	// 向き設定
		virtual void AddRotZ(const float z) override;	// 向き設定
		virtual void AddScl(const D3DXVECTOR3 siz) override;	// 大きさ設定
		virtual void AddSclX(const float x) override;	// 大きさ設定
		virtual void AddSclY(const float y) override;	// 大きさ設定
		virtual void AddSclZ(const float z) override;	// 大きさ設定

		static CObject2D* create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);					// 生成
		static CObject2D* create(int nPriority, D3DXVECTOR3 pos, D3DXVECTOR3 siz);	// 生成
	private:
		// プライベート関数
		void SetVtxPos();	// バーテックスの位置を設定
	private:
		// プライベート変数
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
		LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャ
		D3DXCOLOR m_col;					// 色
		D3DXVECTOR3 m_siz;					// 大きさ
		int m_nNumVertices;	// 超点数


		static const D3DXCOLOR s_colDef;	// 初期色
		static const int s_nNumVertices;	// 初期超点数
	};

}
#endif // !_OBJECT2D_H_