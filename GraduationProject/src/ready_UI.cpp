//=============================================
//
//準備できてるかのUI「ready_UI.cpp」
// Author松田永久
//
//=============================================
#include "ready_UI.h"
#include "active_scene_player_state.h"
#include "active_scene_manager.h"

const std::string My::CReadyUI::TEX_NAME[My::CReadyUI::NUM_TEX] = 
{ 
	"data\\TEXTURE\\not_leady.png",
	"data\\TEXTURE\\leady.png"
};

//=============================================
// コンストラクタ
//=============================================
My::CReadyUI::CReadyUI():m_pReady_txt(nullptr)
{
}

//=============================================
// デストラクタ
//=============================================
My::CReadyUI::~CReadyUI()
{
}

//=============================================
// 初期化
//=============================================
HRESULT My::CReadyUI::Init(CActiveSceneCharacter* character)
{
	if (m_pReady_txt == nullptr)
	{
		CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
		D3DXVECTOR3 screen_pos = { 1010.0f, 80.0f + (player->GetPlayerIdx() * 120.0f), 0.0f }; //スクリーン座標に変換
		/*D3DXVECTOR3 */screen_pos = ConvertToScreenPos(GET_CAMERA(GET_CAMERA_IDX),character->GetPos()); //スクリーン座標に変換
		m_pReady_txt = CReadyTxt::Create(screen_pos, { 100.0f,40.0f });
	}
	return S_OK;
}

//=============================================
// 終了
//=============================================
void My::CReadyUI::Uninit()
{
	if (m_pReady_txt != nullptr)
	{
		m_pReady_txt->SetisDelete(true);
		m_pReady_txt = nullptr;
	}
}

//=============================================
// 更新
//=============================================
void My::CReadyUI::Update()
{
}

//=============================================
// UI設定
//=============================================
void My::CReadyUI::SetCurrentReady_UI(D3DXVECTOR3 screen_pos, bool isReady)
{
	if (m_pReady_txt != nullptr)
	{
		CActiveScenePlayer* player = CActiveSceneManager::GetInstance()->GetPlayer();
		D3DXVECTOR3 screen_pos = { 1010.0f, 80.0f + (player->GetPlayerIdx() * 120.0f), 0.0f }; //スクリーン座標に変換
		//m_pReady_txt->SetPos(screen_pos);
		m_pReady_txt->BindTexture(GET_TEXTURE->GetAddress(GET_TEXTURE->Regist(TEX_NAME[isReady])));
	}
}

//=============================================
// リセット
//=============================================
void My::CReadyUI::Reset()
{
}
