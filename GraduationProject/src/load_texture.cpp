//================================
//
//load_texture.cppに必要な宣言[load_texture.cpp]
//Author:松田永久
// 
//================================
#include "load_texture.h"

//================================
// コンストラクタ
//================================
My::CLoadTexture::CLoadTexture() :
	m_isEndLoad(false),	//読み込みが終わったか
	m_NextLoad(BUTTON)	//次の読み込む
{
	
}

//================================
// デストラクタ
//================================
My::CLoadTexture::~CLoadTexture()
{
	
}

//================================
// 読み込み
//================================
void My::CLoadTexture::Load()
{
	//読み込みが完了していたら抜ける
	if (m_isEndLoad) 
	{
		return;
	}

	// テクスチャ設定
	CTexture* pTex = GET_TEXTURE;
	std::string path;

	//次の読み込み先に応じて処理
	switch (m_NextLoad)
	{
	case BUTTON:

		pTex->Regist(path = "data/TEXTURE/cast_cancel.png");
		pTex->Regist(path = "data/TEXTURE/cementery_icon.png");
		pTex->Regist(path = "data/TEXTURE/check_not.png");
		pTex->Regist(path = "data/TEXTURE/DRAW.png");
		pTex->Regist(path = "data/TEXTURE/cancel.png");
		
		m_NextLoad = ILUSST0;

		break;

	case ILUSST0:

		pTex->Regist(path = "data/TEXTURE/illust/asalt_monkey.png");
		pTex->Regist(path = "data/TEXTURE/illust/demon_becon.png");
		pTex->Regist(path = "data/TEXTURE/illust/drain_train.png");
		pTex->Regist(path = "data/TEXTURE/illust/gabigabikabi.png");
		pTex->Regist(path = "data/TEXTURE/illust/innbiziburuburu.png");

		m_NextLoad = ILUSST1;

		break;

	case ILUSST1:

		pTex->Regist(path = "data/TEXTURE/illust/muscle_castle.png");
		pTex->Regist(path = "data/TEXTURE/illust/pepper_bread.png");
		pTex->Regist(path = "data/TEXTURE/illust/pepper_rapper.png");
		pTex->Regist(path = "data/TEXTURE/illust/sabakarerusaba.png");
		pTex->Regist(path = "data/TEXTURE/illust/sinnpaisuru_nisinnpai.png");

		m_NextLoad = ILUSST2;

		break;

	case ILUSST2:

		pTex->Regist(path = "data/TEXTURE/illust/soul_wall.jpg");
		pTex->Regist(path = "data/TEXTURE/illust/tasikani.png");
		pTex->Regist(path = "data/TEXTURE/illust/tatekannbann.png");
		pTex->Regist(path = "data/TEXTURE/illust/tatetuku_tate.png");
		pTex->Regist(path = "data/TEXTURE/illust/vice_ice.png");
		pTex->Regist(path = "data/TEXTURE/illust/zyakkuzanippa.png");

		m_NextLoad = ICON;

		break;

	case ICON:

		pTex->Regist(path = "data/TEXTURE/icon/assist.png");
		pTex->Regist(path = "data/TEXTURE/icon/attack.png");
		pTex->Regist(path = "data/TEXTURE/icon/counter.png");
		pTex->Regist(path = "data/TEXTURE/icon/deck_icon.png");
		pTex->Regist(path = "data/TEXTURE/icon/player.png");
		pTex->Regist(path = "data/TEXTURE/icon/shield.png");

		m_NextLoad = OTHER;

		break;

	case OTHER:

		pTex->Regist(path = "data/TEXTURE/checker/frame.png");
		pTex->Regist(path = "data/TEXTURE/checker/in.png");
		pTex->Regist(path = "data/TEXTURE/countdown001.png");
		pTex->Regist(path = "data/TEXTURE/death_icon.png");
		pTex->Regist(path = "data/TEXTURE/duel000.png");
		pTex->Regist(path = "data/TEXTURE/number000.png");

		m_isEndLoad = true;

		break;
	}
}
