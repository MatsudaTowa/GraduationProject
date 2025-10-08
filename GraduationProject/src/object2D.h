//=============================================
//
//2Dオブジェクト[object2D.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT2D_H_ //これが定義されてないとき

#define _OBJECT2D_H_
#include "main.h"
#include "object.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 2Dオブジェクトクラス*/
	class CObject2D : public CObject
	{
	public:
		CObject2D(int nPriority = 3);
		~CObject2D()override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		void BindTexture(LPDIRECT3DTEXTURE9 pTex);

		//posの代入
		inline void SetPos(D3DXVECTOR3 pos)
		{
			m_pos = pos;
		}

		//rotの代入
		inline void SetRot(D3DXVECTOR3 rot)
		{
			m_rot = rot;
		}

		//サイズの代入
		inline void SetSize(D3DXVECTOR2 size)
		{
			m_size = size;
		}

		//テクスチャ座標の代入
		inline void SetTexPos(D3DXVECTOR2 tex)
		{
			m_tex_pos = tex;
		}

		inline void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 Buff)
		{
			m_pVtxBuff = Buff;
		}

		//色の代入
		inline void SetColor(D3DXCOLOR col)
		{
			m_col = col;
		}

		//頂点座標の代入
		virtual void SetVtx();

		//頂点座標(拡縮、回転)の代入
		void SetVtx(float fAngle, float fLength);

		void SetGaugeVtx();

		//posの取得
		inline D3DXVECTOR3& GetPos()
		{
			return m_pos;
		}

		//rotの取得
		inline D3DXVECTOR3& GetRot()
		{
			return m_rot;
		}

		//サイズの取得
		inline D3DXVECTOR2& GetSize()
		{
			return m_size;
		}

		//テクスチャ座標の取得
		inline D3DXVECTOR2& GetTexPos()
		{
			return m_tex_pos;
		}

		//色の取得
		inline D3DXCOLOR& GetColor()
		{
			return m_col;
		}

		inline LPDIRECT3DVERTEXBUFFER9& GetVtxBuff()
		{
			return m_pVtxBuff;
		}

		//テクスチャ情報取得
		inline LPDIRECT3DTEXTURE9& GetTexture()
		{
			return m_pTexture;
		}


	private:

		LPDIRECT3DTEXTURE9 m_pTexture; //テクスチャへのポインタ
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //頂点バッファへのポインタ

		D3DXVECTOR3 m_pos;
		D3DXVECTOR3 m_rot;
		D3DXVECTOR2 m_size;
		D3DXVECTOR2 m_tex_pos; //tex座標
		D3DXCOLOR m_col;
	};
}

#endif