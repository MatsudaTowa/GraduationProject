//===========================================
// 
// オブジェクトX[renderer.h]
// Auther:UedaKou
// 
//===========================================
#ifndef _OBJECT_X_H_
#define _OBJECT_X_H_

#include "../object.h"

#define MAX_MODEL_X 1024
#define MAX_TEX_X 8	// 1モデルのテクスチャ最大数

namespace My
{
	class CModelX
	{
	public:
		CModelX();
		~CModelX();
		LPD3DXBUFFER GetBuffMtx() { return m_pBuffMat; }
		DWORD GetNumMat() { return m_dwNumMat; }
		LPD3DXMESH GetMesh() { return m_pMesh; }
		LPDIRECT3DTEXTURE9 GetTexture(int nCnt) { return m_pTexture[nCnt]; }

		static void ReleaseAll();
		static int Load(const char* aFileName);
		static CModelX* GetModel(int nID);
	private:
		// リスト
		static CModelX* m_pTop;	// トップ
		static CModelX* m_pCur;	// 最後尾
		CModelX* m_pNext;		// 次
		CModelX* m_pPrev;		// 前

		LPD3DXBUFFER m_pBuffMat;					// マテリアルへのポインタ
		DWORD m_dwNumMat;							// マテリアルの数
		LPD3DXMESH m_pMesh;							// メッシュ(頂点情報)へのポインタ
		LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEX_X];	// プレイヤーのテクスチャ[テクスチャ数]

	};

	class CObjectX :public CObject
	{
	public:
		enum class TYPE {
			TYPE_NULL = 0,
			MOTION_PARTS,	// モーションパーツ
			OBSTACLES,	// 障害物
			MAX,
		};
		CObjectX();
		CObjectX(int nPriority);
		virtual ~CObjectX()		override;
		virtual HRESULT Init()		override;	// 初期化
		virtual void Uninit()	override;	// 終了
		virtual void Update()	override;	// 更新
		virtual void Draw()		override;	// 描画

		void SetType(TYPE type) { m_type = type; }	// 種類設定
		void SetMtxworld(D3DXMATRIX mtx) { m_mtxWorld = mtx; }	// ワールドマトリクス
		void SetID(int nID) { m_nID = nID; }	// モデルID設定



		TYPE GetType() { return m_type; }	// 種類取得
		D3DXMATRIX GetMtx() { return m_mtxWorld; }	// ワールドマトリクス取得
		int GetID() { return m_nID; }	// 取得



		static CObjectX* create(const char* aFileName);	// 生成
		static CObjectX* create(string aFileName);	// 生成
	protected:
	private:
		TYPE m_type;	// 種類
		D3DXMATRIX m_mtxWorld;	// ワールドマトリクス

		static int m_nNum;	// Xオブジェクト数
		int m_nID;	// モデルID


	};

}

#endif // !_OBJECT_X_H_