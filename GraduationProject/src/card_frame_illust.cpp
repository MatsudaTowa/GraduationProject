//=============================================
//
//カードのタイプイラスト[card_type_illust]
//Author Matsuda Towa
//
//=============================================
#include "card_frame_illust.h"

namespace
{
	const D3DXVECTOR2 ILLUST_SIZE = { 50.0f,35.0f };
	const D3DXVECTOR2 OFFSET = { 0.0f,-80.0f };
}

//=============================================
// コンストラクタ
//=============================================
My::CCardFrameIllust::CCardFrameIllust(int nPriority):CCardFrame(nPriority)
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CCardFrameIllust::Init()
{
	CCardFrame::Init();
	// テクスチャ設定
	CTexture* pTex = GET_TEXTURE;

	std::string get_tex_pass = GetCard()->GetBaseStatus().imagePath;
	const std::string tex_pass = m_FrameInfo[GetType()].pass + get_tex_pass;

	BindTexture(pTex->GetAddress(pTex->Regist(tex_pass)));

	D3DXVECTOR3 offset_pos = GetPos();
	offset_pos.x += OFFSET.x;
	offset_pos.y += OFFSET.y;

	SetPos(offset_pos);

	SetSize(ILLUST_SIZE);

	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CCardFrameIllust::Uninit()
{
	CCardFrame::Uninit();
}

//=============================================
// 更新
//=============================================
void My::CCardFrameIllust::Update()
{
	CCardFrame::Update();

	SetSize(ILLUST_SIZE);
}
