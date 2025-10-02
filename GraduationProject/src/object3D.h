//=============================================
//
//3Dオブジェクト[object3D.h]
//Author Matsuda Towa
//
//=============================================
#ifndef _OBJECT3D_H_ //これが定義されてないとき

#define _OBJECT3D_H_
#include "main.h"
#include "object.h"

/** @brief My 名前空間 */
namespace My
{
	/** @brief 3Dオブジェクトクラス */
	class CObject3D : public CObject
	{
	public:
		CObject3D(int nPriority);
		~CObject3D()override;
		HRESULT Init()override;
		void Uninit()override;
		void Update()override;
		void Draw()override;

		void BindTexture(LPDIRECT3DTEXTURE9 pTex);

		//色の取得
		//色の代入
		inline void SetColor(D3DXCOLOR col)
		{
			m_col = col;
		}

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
		inline void SetSize(D3DXVECTOR3 size)
		{
			m_size = size;
		}

		//テクスチャ座標の代入
		inline void SetTexPos(D3DXVECTOR2 tex)
		{
			m_tex_pos = tex;
		}

		//ワールドマトリックスの代入
		inline void SetMtxWorld(D3DXMATRIX mtxWorld)
		{
			m_mtxWorld = mtxWorld;
		}

		//頂点情報の代入
		inline void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 vtxBuff)
		{
			m_pVtxBuff = vtxBuff;
		}

		//頂点座標の代入
		void SetVtx(D3DXVECTOR3 nor);

		//頂点座標の代入
		void SetVtx(D3DXVECTOR3 nor, float fAngle, float fLength);

		//色の取得
		inline D3DXCOLOR& GetColor()
		{
			return m_col;
		}

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
		inline D3DXVECTOR3& GetSize()
		{
			return m_size;
		}

		//テクスチャ座標の取得
		inline D3DXVECTOR2& GetTexPos()
		{
			return m_tex_pos;
		}

		//頂点情報取得
		inline LPDIRECT3DVERTEXBUFFER9& GetVtxBuff()
		{
			return m_pVtxBuff;
		}

		//テクスチャ情報取得
		inline LPDIRECT3DTEXTURE9& GetTexture()
		{
			return m_pTexture;
		}

		//ワールドマトリックス取得
		inline D3DXMATRIX& GetMtxWorld()
		{
			return m_mtxWorld;
		}

	private:

		LPDIRECT3DTEXTURE9 m_pTexture; //テクスチャへのポインタ
		LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //頂点バッファへのポインタ

		D3DXCOLOR m_col; //色
		D3DXVECTOR3 m_pos; //位置
		D3DXVECTOR3 m_rot; //方向
		D3DXVECTOR3 m_size; //サイズ
		D3DXVECTOR2 m_tex_pos; //tex座標

		D3DXMATRIX m_mtxWorld; //ワールドマトリックス
	};
}
#endif