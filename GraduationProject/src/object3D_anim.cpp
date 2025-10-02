//=============================================
//
//アニメーション3D[object3D_anim.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object3D_anim.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 * @param [in]プライオリティ
 */
My::CObject3D_Anim::CObject3D_Anim(int nPriority) :CObject3D(nPriority),
m_pCnt(nullptr)	//アニメーションカウント初期化
{
}

/**
 * @brief デストラクタ
 */
My::CObject3D_Anim::~CObject3D_Anim()
{
}

/**
 * @brief 初期化
 * @return 成功したか
 */
HRESULT My::CObject3D_Anim::Init()
{
	if (m_pCnt == nullptr)
	{
		m_pCnt = new CCount;
	}
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CObject3D_Anim::Uninit()
{
	if (m_pCnt != nullptr)
	{
		delete m_pCnt;
		m_pCnt = nullptr;
	}
	CObject3D::Uninit();
}

/**
 * @brief 更新
 */
void My::CObject3D_Anim::Update()
{
	//テクスチャ座標取得
	D3DXVECTOR2 tex_pos = GetTexPos();
}

/**
 * @brief 描画
 */
void My::CObject3D_Anim::Draw()
{
	CObject3D::Draw();
}

/**
 * @brief アニメーション処理
 * @param [in]テクスチャ座標
 * @param [in]テクスチャ移動量
 */
void My::CObject3D_Anim::AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move)
{
	if (m_pCnt->CountUp())
	{//フレーム数が達したら
		//テクスチャをずらす
		tex_pos.x += tex_move.x;
		m_tex_move = tex_move;

		m_pCnt->SetCnt(INT_ZERO);
	}

	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_3D* pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの座標指定
	pVtx[0].tex = D3DXVECTOR2(tex_pos.x
		, tex_pos.y);
	pVtx[1].tex = D3DXVECTOR2(tex_pos.x + tex_move.x
		, tex_pos.y);
	pVtx[2].tex = D3DXVECTOR2(tex_pos.x
		, tex_pos.y + tex_move.y);
	pVtx[3].tex = D3DXVECTOR2(tex_pos.x + tex_move.x
		, tex_pos.y + tex_move.y);

	//アンロック
	vtxBuff->Unlock();

	SetVtxBuff(vtxBuff);

	//tex設定
	SetTexPos(tex_pos);
}