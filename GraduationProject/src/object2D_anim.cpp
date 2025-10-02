//=============================================
//
//アニメーション2D[object2D_anim.cpp]
//Author Matsuda Towa
//
//=============================================
#include "object2D_anim.h"
#include "manager.h"

/**
 * @brief コンストラクタ
 * @param [in]プライオリティ
 */
My::CObject2D_Anim::CObject2D_Anim(int nPriority):CObject2D(nPriority),
m_nAnimCnt(INT_ZERO)	//アニメーションカウント初期化
{
}

/**
 * @brief デストラクタ
 */
My::CObject2D_Anim::~CObject2D_Anim()
{
}

/**
 * @brief 初期化
 * @return 成功したか
 */
HRESULT My::CObject2D_Anim::Init()
{
	return S_OK;
}

/**
 * @brief 終了
 */
void My::CObject2D_Anim::Uninit()
{
	CObject2D::Uninit();
}

//=============================================
//更新
//=============================================
void My::CObject2D_Anim::Update()
{
	//テクスチャ座標取得
	D3DXVECTOR2 tex_pos = GetTexPos();
}

/**
 * @brief 描画
 */
void My::CObject2D_Anim::Draw()
{
	CObject2D::Draw();
}

/**
 * @brief アニメーション処理
 * @param [in]テクスチャ座標
 * @param [in]テクスチャ移動量
 */
void My::CObject2D_Anim::AnimationTex(D3DXVECTOR2 tex_pos, D3DXVECTOR2 tex_move)
{
	m_nAnimCnt++;

	if (m_nAnimCnt >= m_nAnimFrame)
	{//フレーム数が達したら
		//テクスチャをずらす
		tex_pos.x += tex_move.x;
		m_tex_move = tex_move;

		m_nAnimCnt = INT_ZERO;
	}

	CRenderer* pRender = GET_RENDERER;

	LPDIRECT3DDEVICE9 pDevice = pRender->GetDevice();

	LPDIRECT3DVERTEXBUFFER9 vtxBuff = GetVtxBuff();

	if (vtxBuff == nullptr)
	{
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &vtxBuff, NULL);
	}

	VERTEX_2D* pVtx;

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

	//tex設定
 	SetTexPos(tex_pos);
}